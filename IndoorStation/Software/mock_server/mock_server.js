const path = require("path");
const fs = require("fs");
const express = require("express");
const multer = require("multer");
const app = express();

// Middleware for parsing POST data
app.use(express.urlencoded({ extended: true }));
app.use(express.json());

// Configure multer for file uploads (memory storage)
const upload = multer({ storage: multer.memoryStorage() });

const NUM_SUPPORTED_SENSORS = 2;
const SENSOR_PIN_STATE_OPEN = false;    // LOW
const SENSOR_PIN_STATE_CLOSED = true;   // HIGH

// SensorModes enum from structures.h
const SENSOR_MODE_NORMAL = 0;           // All messages are received, everything is saved, LED on
const SENSOR_MODE_DISABLED = 1;         // All messages are ignored, nothing is saved, LED off
const SENSOR_MODE_CHARGING = 2;         // All messages are ignored, nothing is saved, LED in different color
const SENSOR_MODE_ONLY_DISPLAY = 3;     // Messages are only displayed via the LED, nothing is saved
const SENSOR_MODE_PAIRING = 4;          // Message from Indoor Station to corresponding sensor to configure the MAC address in the sensor, LED flashes red and blue, nothing is saved

let batteryEmptyThreshold = 15;

// Initialize arrays based on NUM_SUPPORTED_SENSORS
let sensorModes = new Array(NUM_SUPPORTED_SENSORS).fill(SENSOR_MODE_NORMAL);
let sensorMACs = [];
for (let i = 0; i < NUM_SUPPORTED_SENSORS; i++) {
    sensorMACs.push(`AA:BB:CC:DD:EE:${(i + 1).toString(16).toUpperCase().padStart(2, '0')}`);
}
let sensorNames = [];
for (let i = 0; i < NUM_SUPPORTED_SENSORS; i++) {
    sensorNames.push(`Sensor ${(i + 1).toString()}`);
}
let sensorsPaired = new Array(NUM_SUPPORTED_SENSORS).fill(true);
let sensorsEncrypted = new Array(NUM_SUPPORTED_SENSORS).fill(true);

// Set specific modes for testing
sensorModes[0] = SENSOR_MODE_CHARGING;
sensorMACs[0] = "00:00:00:00:00:00";

// Path to data folder
const dataPath = path.join(__dirname, "..", "data");
const FILENAME_HISTORY_SENSOR_FORMAT = "mockDataSensor%d.bin";  // located beside this .js file

// #########################################################################################
// #########################################################################################

const BINFILE_RECORD_SIZE = 13;

function getNumMessagesPerSensorFromBinFile(sensorIndex)
{
    const filename = FILENAME_HISTORY_SENSOR_FORMAT.replace("%d", sensorIndex);
    const filepath = path.join(__dirname, filename);
    if(!fs.existsSync(filepath))
    {
        return 0;
    }
    else
    {
        const stats = fs.statSync(filepath);
        return stats.size / BINFILE_RECORD_SIZE;
    }
}

function readSensorBinFile(sensorIndex)
{
    const messages = [];
    const filename = FILENAME_HISTORY_SENSOR_FORMAT.replace("%d", sensorIndex);
    const filepath = path.join(__dirname, filename);
    if(!fs.existsSync(filepath))
    {
        return messages;
    }

    const buffer = fs.readFileSync(filepath);

    for(let offset = 0; offset < buffer.length; offset += BINFILE_RECORD_SIZE)
    {
        const pinState = buffer.readUInt8(offset) !== 0;
        const batteryVoltage_mV = buffer.readUInt16LE(offset + 1);
        const numberSendLoops = buffer.readUInt8(offset + 3);
        const sensor_sw_version = buffer.readUInt8(offset + 4);
        const timestamp = Number(buffer.readBigInt64LE(offset + 5));

        messages.push({
            pinState,
            batteryVoltage_mV,
            numberSendLoops,
            sensor_sw_version,
            timestamp
        });
    }

    return messages;
}

function writeSensorBinFile(sensorIndex, messages)
{
    const buffer = Buffer.alloc(messages.length * BINFILE_RECORD_SIZE);

    messages.forEach((msg, i) =>
    {
        const offset = i * BINFILE_RECORD_SIZE;

        buffer.writeUInt8(msg.pinState ? 1 : 0, offset);
        buffer.writeUInt16LE(msg.batteryVoltage_mV, offset + 1);
        buffer.writeUInt8(msg.numberSendLoops, offset + 3);
        buffer.writeUInt8(msg.sensor_sw_version, offset + 4);
        buffer.writeBigInt64LE(BigInt(msg.timestamp), offset + 5);
    });

    const filename = FILENAME_HISTORY_SENSOR_FORMAT.replace("%d", sensorIndex);
    const filepath = path.join(__dirname, filename);
    fs.writeFileSync(filepath, buffer);
}

function generateDummySensorBinFileMessages(fileIndex, numMessages, daysBetweenMessages=3)
{
    const messages = [];
    let now = Math.floor(Date.now()/1000);
    let timestepPerMessage = daysBetweenMessages*86400; // 86400 seconds in one day
    let currentTime = now - numMessages*timestepPerMessage;
    let voltage = 4200; // full accu

    for(let i=0;i<numMessages;i++)
    {
        // basic drop per message
        const drop = ((fileIndex+1)*2) + Math.random() * 10;   // fileIndex=0 -> 2..12 mV, fileIndex=1 -> 4..14mV
        // some noise
        const noise = (Math.random() - 0.5) * 20; // ±10 mV
        voltage -= drop;
        const batteryVoltage = voltage + noise;

        messages.push({
            pinState: Math.random() > 0.5,
            batteryVoltage_mV: Math.round(batteryVoltage),
            numberSendLoops: 1,
            sensor_sw_version: 0x00,    // Major=0, Minor=0
            timestamp: currentTime
        });
        currentTime += timestepPerMessage;
    }
    return messages;
}

function generateDummySensorBinFiles(numFiles, numMessages, daysBetweenMessages)
{
    for(let i=0;i<numFiles;i++)
    {
        const messages = generateDummySensorBinFileMessages(i, numMessages, daysBetweenMessages);
        writeSensorBinFile(i, messages);
    }
    console.log("Dummy sensor .bin files generated");
}

const batteryLookupTable = [
    { percent: 0, voltage: 3000 },
    { percent: 10, voltage: 3200 },
    { percent: 20, voltage: 3400 },
    { percent: 30, voltage: 3560 },
    { percent: 40, voltage: 3680 },
    { percent: 50, voltage: 3800 },
    { percent: 60, voltage: 3880 },
    { percent: 70, voltage: 3960 },
    { percent: 80, voltage: 4040 },
    { percent: 90, voltage: 4120 },
    { percent: 100, voltage: 4200 }
];
function mapValue(x, in_min, in_max, out_min, out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
function battery_voltageToPercent(batteryVoltage_mV, numberFractionalDigits = 1)
{
    const n = batteryLookupTable.length;

    if(batteryVoltage_mV <= batteryLookupTable[0].voltage)
        return batteryLookupTable[0].percent;

    if(batteryVoltage_mV >= batteryLookupTable[n - 1].voltage)
        return batteryLookupTable[n - 1].percent;

    for(let i = 0; i < n - 1; i++)
    {
        const entry1 = batteryLookupTable[i];
        const entry2 = batteryLookupTable[i + 1];

        if(batteryVoltage_mV > entry1.voltage && batteryVoltage_mV <= entry2.voltage)
        {
            const multiplier = Math.pow(10, numberFractionalDigits);

            const percent = mapValue(
                batteryVoltage_mV,
                entry1.voltage,
                entry2.voltage,
                entry1.percent * multiplier,
                entry2.percent * multiplier
            ) / multiplier;

            return percent;
        }
    }
    return 0;
}

// #########################################################################################
// #########################################################################################

app.get("/num_sensors", (req, res) =>
{
    res.send(String(NUM_SUPPORTED_SENSORS));
});

// #########################################################################################

app.get("/get_sensor_status", (req, res) =>
{
    const sensors = [];
    for(let i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        const messages = readSensorBinFile(i);
        const lastMessage = messages.length > 0 ? messages[messages.length - 1] : null;

        const voltage_mV = lastMessage ? lastMessage.batteryVoltage_mV : (3700 + i * 100);
        const pinState = lastMessage ? lastMessage.pinState : (i === 0 ? SENSOR_PIN_STATE_OPEN : SENSOR_PIN_STATE_CLOSED);
        const timestamp = lastMessage ? new Date(lastMessage.timestamp * 1000).toLocaleString('de-DE') : new Date().toLocaleString('de-DE');
        const percentage = battery_voltageToPercent(voltage_mV);

        const sensor = {
            index: i,
            name: sensorNames[i],
            mac: sensorMACs[i],
            // Status data
            state: pinState,
            voltage_mV: voltage_mV,
            percentage: percentage.toFixed(2),
            timestamp: timestamp,
            // Management data
            mode: sensorModes[i],
            numMessages: getNumMessagesPerSensorFromBinFile(i),
            swVersion: "v0.0",
            isPaired: sensorsPaired[i],
            useEncryption: sensorsEncrypted[i]
        };
        sensors.push(sensor);
    }
    res.json({ sensors });
});

// #########################################################################################

app.get("/get_indoor_station_info", (req, res) =>
{
    const info = {
        mac: "01:02:03:04:05:06",
        swVersion: "v0.0",
        memoryUsage: "9.77 %",
        batteryEmptyThreshold: batteryEmptyThreshold
    };
    res.json(info);
});

// #########################################################################################

app.get("/get_system_time", (req, res) =>
{
    const systemTime = new Date().toLocaleString('de-DE');
    res.type("text/plain").send(systemTime);
});

// #########################################################################################

app.get("/set_sensor_mode", (req, res) => 
{
    const sensorIndex = parseInt(req.query.sensorIndex);
    const mode = parseInt(req.query.mode);

    if(sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS)
    {
        sensorModes[sensorIndex] = mode;
    }
    res.redirect("/system_management.html");
});

// #########################################################################################

app.get("/set_mac_sensor", (req, res) => 
{
    const sensorIndex = parseInt(req.query.sensorIndex);
    const mac = req.query.mac;

    sensorMACs[sensorIndex] = mac;
    sensorsPaired[sensorIndex] = true;
    res.redirect("/system_management.html");
});

// #########################################################################################

app.get("/set_sensor_name", (req, res) => 
{
    const sensorIndex = parseInt(req.query.sensorIndex);
    const name = req.query.name;

    sensorNames[sensorIndex] = name;
    res.redirect("/system_management.html");
});

// #########################################################################################

app.get("/set_battery_empty_threshold", (req, res) => 
{
    const threshold = parseInt(req.query.threshold);
    if(threshold >= 0 && threshold <= 100) // Ensure threshold is a valid percentage
    {
        batteryEmptyThreshold = threshold;
    }
    res.send("OK");
});

// #########################################################################################

app.post("/remove_data", (req, res) => 
{
    const sensorIndex = parseInt(req.body.sensorIndex);
    
    if(sensorIndex < 0)
    {
        // Remove all sensor history files
        for(let i = 0; i < NUM_SUPPORTED_SENSORS; i++)
        {
            const filename = FILENAME_HISTORY_SENSOR_FORMAT.replace("%d", i);
            const filepath = path.join(__dirname, filename);
            if(fs.existsSync(filepath))
            {
                fs.unlinkSync(filepath);    // delete the file
            }
        }
    }
    else if(sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS)
    {
        // Remove specific sensor history file
        const filename = FILENAME_HISTORY_SENSOR_FORMAT.replace("%d", sensorIndex);
        const filepath = path.join(__dirname, filename);
        if(fs.existsSync(filepath))
        {
            fs.unlinkSync(filepath);        // delete the file
        }
    }
    
    res.redirect("/system_management.html");
});

// #########################################################################################

app.post("/upload_data", upload.single('data'), (req, res) =>
{
    const sensorIndex = parseInt(req.body.sensorIndex);
    
    if(isNaN(sensorIndex) || sensorIndex < 0 || sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        res.status(400).send("Upload failed: Invalid sensorIndex or file could not be written!");
        return;
    }

    if(!req.file)
    {
        res.status(400).send("Upload failed: Invalid sensorIndex or file could not be written!");
        return;
    }

    const buffer = req.file.buffer;
    const filename = FILENAME_HISTORY_SENSOR_FORMAT.replace("%d", sensorIndex);
    const filepath = path.join(__dirname, filename);
    
    fs.writeFileSync(filepath, buffer);
    res.redirect("/system_management.html");
});

// #########################################################################################

app.get("/download_data", (req, res) =>
{
    let sensorIndex = parseInt(req.query.sensorIndex);
    if(isNaN(sensorIndex))
    {
        res.status(400).send("sensorIndex parameter not set");
        return;
    }

    const filename = FILENAME_HISTORY_SENSOR_FORMAT.replace("%d", sensorIndex);
    const filepath = path.join(__dirname, filename);

    if(fs.existsSync(filepath))
    {
        res.download(filepath, filename);
    }
    else
    {
        res.send("No file with this name found: " + filename);
    }
});

// #########################################################################################

app.get("/get_data", (req, res) => 
{
    const sensorIndex = parseInt(req.query.sensorIndex);
    if(isNaN(sensorIndex))
    {
        res.send("sensorIndex parameter not set");
        return;
    }
    const from = parseInt(req.query.from) || 0;
    const to = parseInt(req.query.to) || Number.MAX_SAFE_INTEGER;

    const messages = readSensorBinFile(sensorIndex);
    if(messages.length == 0)
    {
        res.send("No file for sensor " + sensorIndex + " found");
        return;
    }

    res.setHeader("Content-Type", "text/plain");

    let index = 0;
    const interval = setInterval(() =>
    {
        if(index >= messages.length)
        {
            clearInterval(interval);
            res.end();
            return;
        }

        const m = messages[index];
        
        if(m.timestamp < from || m.timestamp > to)
        {
            index++;
            return;
        }

        const obj =
        {
            time: m.timestamp,
            pin: m.pinState,
            batP: battery_voltageToPercent(m.batteryVoltage_mV)
        };

        // send JSON
        const json = JSON.stringify(obj);
        res.write(json);
        index++;

    }, 2000 / messages.length); // simulate chunked transfer like ESP
});

// #########################################################################################

app.get("/get_pairing_info", (req, res) => 
{
    const info = {
        isPairingApActive: false
    };
    res.json(info);
});

// #########################################################################################

app.post("/remove_sensor", (req, res) => 
{
    const sensorIndex = parseInt(req.body.sensorIndex);
    
    sensorMACs[sensorIndex] = "";
    sensorsPaired[sensorIndex] = false;
    sensorsEncrypted[sensorIndex] = false;

    res.redirect("/system_management.html");
});

// #########################################################################################

app.get("/events", (req, res) =>
{
    res.setHeader("Content-Type", "text/event-stream");
    res.setHeader("Cache-Control", "no-cache");
    res.setHeader("Connection", "keep-alive");
    res.flushHeaders();

    console.log("SSE client connected");

    req.on("close", () =>
    {
        console.log("SSE client disconnected");
        res.end();
    });
});

// #########################################################################################

// provide static files in data folder
app.use(express.static(dataPath));

app.listen(80, () =>
{
    console.log("Mock Server is running (http://localhost)");
});

// Uncomment this line to generate files containing the mock data
//generateDummySensorBinFiles(NUM_SUPPORTED_SENSORS, 100, 4);