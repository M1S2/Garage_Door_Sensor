#include <Arduino.h>
#include <LittleFS.h>
#include "wifimanager.h"

bool configuration_AP_open;
bool wifiInit_inProgress;
AsyncWebServer webServer(80);
IPAddress configurationAcessPoint_localIP(192, 168, 4, 1);
DNSServer dnsServer;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";

// https://github.com/HerrRiebmann/Caravan_Leveler/blob/main/Webserver.ino

void redirect(AsyncWebServerRequest *request);
bool captivePortal(AsyncWebServerRequest *request);
String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);

bool WiFiBegin(bool startWithConfigurationAcessPoint)
{
  wifiInit_inProgress = true;

  bool connectSuccess = false;

  //Manually change between WiFi and Accesspoint. AP will be used as a fallback, after 5 seconds
  if (startWithConfigurationAcessPoint)
  {
    CreateConfigurationAccessPoint();
  }
  else
  {
    connectSuccess = ConnectToAccessPoint();
  }

  if(configuration_AP_open)
  {
    // Web Server Root URL
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      Serial.println("wifimanager.html request");
      request->send(LittleFS, "/wifimanager.html", "text/html");
    });

    //Always redirect to captive portal. Request comes with IP (8.8.8.8) or URL (connectivitycheck.XXX / captive.apple / etc.)  
    //Android captive portal.
    webServer.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request)    
    {
      Serial.println("Android captive portal");
      redirect(request);
    }).setFilter(ON_AP_FILTER);
    //Microsoft captive portal.
    webServer.on("/fwlink", [](AsyncWebServerRequest *request)    
    {
      Serial.println("Microsoft captive portal");
      redirect(request);
    }).setFilter(ON_AP_FILTER);
    //www.msftconnecttest.com
    webServer.on("/connecttest.txt", [](AsyncWebServerRequest *request)    
    {
      Serial.println("connecttest.txt");
      redirect(request);
    });
    //captive.apple.com
    webServer.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)    
    {
      redirect(request);
    });
    //detectportal.firefox.com/sucess.txt
    webServer.on("/success.txt", HTTP_GET, [](AsyncWebServerRequest *request)    
    {
      request->send(200, "text/plain", "success"); 
    });

    webServer.on("/", HTTP_POST, [](AsyncWebServerRequest *request) 
    {
      String ssid;
      String password;

      int numParams = request->params();
      for(int i=0; i<numParams; i++)
      {
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost())
        {
          // HTTP POST ssid value
          if (p->name() == "ssid") 
          {
            ssid = p->value();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            //writeFile(LittleFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == "pass") 
          {
            password = p->value();
            Serial.print("Password set to: ");
            Serial.println(password);
            // Write file to save value
            //writeFile(LittleFS, passPath, pass.c_str());
          }
          //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }

      writeFile(LittleFS, ssidPath, ssid.c_str());
      writeFile(LittleFS, passPath, password.c_str());

      //request->send(200, "text/plain", "Credentials received. Trying to connect again.");
      //ConnectToAccessPoint(5000, false);
      
      //request->send(200, "text/plain", "Done. ESP will restart and try to connect to your router");
      delay(1000);
      ESP.restart();
    });

    //const char* Headers[] = {"If-None-Match"};
    //webServer.collectHeaders(Headers, sizeof(Headers) / sizeof(Headers[0]));
  }
  else if(connectSuccess)
  {
    // Route for root index.html
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/index.html", "text/html"); 
    });
    webServer.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/index.html", "text/html"); 
    });

    // Route for root accu_chart.html
    webServer.on("/accu_chart.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/accu_chart.html", "text/html"); 
    });

    // Route for root sensor_info.html
    webServer.on("/sensor_info.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/sensor_info.html", "text/html"); 
    });

    // Route for root common.js
    webServer.on("/common.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/common.js", "text/javascript"); 
    });

    // Route for root index.js
    webServer.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/index.js", "text/javascript"); 
    });

    // Route for root accu_chart.js
    webServer.on("/accu_chart.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/accu_chart.js", "text/javascript"); 
    });

    // Route for root sensor_info.js
    webServer.on("/sensor_info.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/sensor_info.js", "text/javascript"); 
    });
  }

  if(connectSuccess || configuration_AP_open)
  {
    // Route for root style.css
    webServer.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/style.css", "text/css"); 
    });

    webServer.serveStatic("/", LittleFS, "/");

    webServer.onNotFound([](AsyncWebServerRequest *request)
    {
      Serial.println("onNotFound");
      if (captivePortal(request))
      {
        // if captive portal redirect instead of displaying the error page
        return;
      }

      request->send(404, "text/plain", "Not found");
    });

    webServer.begin();
    Serial.println(F("HTTP webServer started"));
    delay(100);
  }

  return connectSuccess;
}

bool ConnectToAccessPoint(uint timeout_ms, bool openConfigurationAPOnFailure) 
{
  Serial.println("Begin connection");

  configuration_AP_open = false;

  // Load values saved in LittleFS
  Serial.println("Read credentials");
  String ssid = readFile(LittleFS, ssidPath);
  String password = readFile(LittleFS, passPath);
  Serial.println("\"" + ssid + "\"");
  Serial.println("\"" + password + "\"");

  if(ssid.isEmpty())
  {
    Serial.println("Undefined SSID. Starting configuration AP.");
    if(openConfigurationAPOnFailure) { CreateConfigurationAccessPoint(); }
    return false;
  }

  WiFi.mode(WIFI_AP_STA);
  Serial.println("Wifi mode set");
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("After WiFi.begin()");

  long start = millis();
  Serial.println("After millis()");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
    if (millis() - start > timeout_ms) 
    {
      Serial.println("Wifi not found!");
      wifiInit_inProgress = false;
      if(openConfigurationAPOnFailure) { CreateConfigurationAccessPoint(); }
      return false;
    }
  }
  Serial.println();
  Serial.println(F("WiFi connected successfully"));
  Serial.print(F("Got IP: "));
  Serial.println(WiFi.localIP());  //Show ESP IP on serial

  wifiInit_inProgress = false;

  return true;
}

void CreateConfigurationAccessPoint() 
{
  configuration_AP_open = true;
  wifiInit_inProgress = false;

  WiFi.disconnect();
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(DEVICE_NAME);
  delay(500);
  WiFi.softAPConfig(configurationAcessPoint_localIP, gateway, subnet);
  delay(500);
  Serial.print(F("AP IP address: "));
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS webServer redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  if(!dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()))
  {
    Serial.println("Failed to start DNS server");
  }
}

/*boolean captivePortal() 
{
  Serial.print(F("Captive Check: "));
  Serial.println(webServer.hostHeader());
  if (!isIp(webServer.hostHeader())) 
  {
    Serial.println("-Request redirected to captive portal");
    redirect();
    return true;
  }
  return false;
}*/

// is this an IP?
boolean isIp(String str)
{
  for (unsigned int i = 0; i < str.length(); i++)
  {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9'))
    {
      return false;
    }
  }
  return true;
}

/** Redirect to captive portal if we got a request for another domain.
 * Return true in that case so the page handler do not try to handle the request again. */
bool captivePortal(AsyncWebServerRequest *request)
{
  Serial.println("captivePortal: " + request->host());

  if (!isIp(request->host()))
  {
    Serial.println("Request redirected to captive portal");
    AsyncWebServerResponse* response = request->beginResponse(302, "text/plain", "");
    response->addHeader("Location", String("http://") + request->client()->localIP().toString());
    request->send(response);
    return true;
  }
  return false;
}

void redirect(AsyncWebServerRequest *request)
{
  if (captivePortal(request))
  {
    // if captive portal redirect instead of displaying the page
    return;
  }

  request->send(LittleFS, "/wifimanager.html", "text/html");

  //AsyncWebServerResponse* response = request->beginResponse(302, "text/plain", "");
  //response->addHeader("Location", String("http://") + configurationAcessPoint_localIP.toString());
  //request->send(response);

  //request->sendHeader("Location", String("http://") + toStringIp(webServer.client().localIP()), true);
  //request->send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
  //request->client().stop(); // Stop is needed because we sent no content length
}


// Read File from LittleFS
String readFile(fs::FS &fs, const char * path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}

// Write file to LittleFS
void writeFile(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w+");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}


void clearWifiCredentials()
{
  LittleFS.remove(ssidPath);
  LittleFS.remove(passPath);
}