const path = require("path");
const fs = require("fs");
const express = require("express");
const app = express();

let sensorMode1 = 0;    // Normal
let sensorMode2 = 2;    // Laden
let sensorMAC1 = "";
let sensorMAC2 = "";

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

    const buffer = fs.readFileSync(filename);

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

app.get("/index.html", (req, res) =>
{
    const filePath = path.join(__dirname, "..", "data", "index.html");
    fs.readFile(filePath, "utf8", (err, data) =>
    {
        if (err)
        {
            res.status(500).send("Error loading file");
            return;
        }

        // Replace placeholders with dummy values (like in ESP code)
        let processed = data
            .replace("%SENSOR_STATE_1%", "Auf")
            .replace("%SENSOR_STATE_2%", "Zu")
            .replace("%SENSOR_VOLTAGE_1%", "4.0")
            .replace("%SENSOR_VOLTAGE_2%", "3.88")
            .replace("%SENSOR_PERCENTAGE_1%", "75.50")
            .replace("%SENSOR_PERCENTAGE_2%", "60.00")
            .replace("%SENSOR_TIMESTAMP_1%", "01.02.2025 10:11:12")
            .replace("%SENSOR_TIMESTAMP_2%", "10.11.2012 02:03:04");

        res.send(processed);
    });
});

// #########################################################################################

app.get("/system_management.html", (req, res) =>
{
    const filePath = path.join(__dirname, "..", "data", "system_management.html");
    fs.readFile(filePath, "utf8", (err, data) =>
    {
        if (err)
        {
            res.status(500).send("Error loading file");
            return;
        }

        // Replace placeholders with dummy values (like in ESP code)
        let processed = data
            .replace("%INDOOR_STATION_MAC%", "01:02:03:04:05:06")
            .replace("%MEMORY_USAGE%", "100000 von 1024000 Bytes belegt (9.77 %)")
            .replace("%INDOOR_STATION_SW_VERSION%", "v0.0")
            .replace("%SENSOR_SW_VERSION_1%", "v0.0")
            .replace("%SENSOR_SW_VERSION_2%", "v0.0")
            .replace("%NUM_MESSAGES_1%", getNumMessagesPerSensorFromBinFile(0))
            .replace("%NUM_MESSAGES_2%", getNumMessagesPerSensorFromBinFile(1))
            .replace("%SENSOR_MODE_1%", sensorMode1)
            .replace("%SENSOR_MODE_2%", sensorMode2)
            .replace("%MAC_SENSOR_1%", sensorMAC1)
            .replace("%MAC_SENSOR_2%", sensorMAC2);

        res.send(processed);
    });
});

// #########################################################################################

app.get("/set_sensor_mode", (req, res) => 
{
    const sensorIndex = parseInt(req.query.sensorIndex);
    const mode = parseInt(req.query.mode);

    if(sensorIndex == 0)
    {
        sensorMode1 = mode;
    }
    else if(sensorIndex == 1)
    {
        sensorMode2 = mode;
    }
    res.redirect("/system_management.html");
});

// #########################################################################################

app.get("/set_mac_sensor", (req, res) => 
{
    const sensorIndex = parseInt(req.query.sensorIndex);
    const mac = req.query.mac;

    if(sensorIndex == 0)
    {
        sensorMAC1 = mac;
    }
    else if(sensorIndex == 1)
    {
        sensorMAC2 = mac;
    }
    res.redirect("/system_management.html");
});

// #########################################################################################

app.get("/remove_data", (req, res) => 
{
    const sensorIndex = parseInt(req.query.sensorIndex);
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

// provide static files in data folder
app.use(express.static(dataPath));

app.listen(80, () =>
{
    console.log("Mock Server is running (http://localhost)");
});

// Uncomment this line to generate files containing the mock data
//generateDummySensorBinFiles(2, 100, 4);