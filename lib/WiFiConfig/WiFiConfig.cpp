//WiFiConfig.cpp
#include "WiFiConfig.h"

boolean WiFiConfig::sta_mode = true;  // Initialize sta_mode
WiFiConfig::WiFiConfig() {
    wifi_html = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>WiFi Configuration</title>
            <style>
                body {
                    font-size: 300%;
                }
                form {
                    font-size: 150%;
                }
                input[type="text"],
                input[type="password"] {
                    width: 100%;
                    padding: 30px;
                    margin: 10px;
                }
                input[type="submit"] {
                    width: 100%;
                    padding: 10px;
                    margin: 10px;
                    font-size: 150%; /* Adjust the font size here */
                }
            </style>
        </head>
        <body>
            <h1>WiFi Configuration</h1>
            <form action="/wifi-submit" method="POST">
                SSID: <input type="text" name="ssid"><br>
                Password: <input type="password" name="password"><br>
                <input type="submit" value="Save and Connect">
            </form>
        </body>
        </html>
    )rawliteral";
}




/**
 * WiFiSetup handles connecting to WiFi in station mode or starting an access point.
 * It first checks for saved credentials, and attempts to connect in station mode using them.
 * If that fails, it falls back to access point mode with a default SSID.
 */
void WiFiConfig::WiFiSetup()
{

    preferences.begin("credentials", false);

    // if credentials does not exist, set sta_mode to false to allow AP mode

    // message "Starting WiFiSetup"
    Serial.println("Starting WiFiSetup");

    ssid = preferences.getString("ssid", "");
    //  ssid = ""; //for testing purposes
    password = preferences.getString("password", "");
    // password = "test"; //for testing purposes

    // if ssid or password is empty, set sta_mode to false to allow AP mode
    if (ssid == "" || password == "")
    {
        Serial.println("No values saved for ssid or password");
        // start in access point mode with ssid=kame
        sta_mode = false;
    }

    // if sta_mode is true, then we are attempting to connect to wifi 3 times
    if (sta_mode)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        Serial.print("Connecting to WiFi ..");
        // do 3 connection attempts, after that set sta_mode to false
        int connection_attempts = 0;
        while (WiFi.status() != WL_CONNECTED && connection_attempts < 3)
        {
            Serial.print('.');
            delay(1000);
            connection_attempts++;
        }
    }
    // if still not connected in sta mode, then we failed to connect and set sta_mode to false
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Failed to connect to WiFi");
        sta_mode = false;
    }

    // if sta_mode is false, then we are in access point mode
    if (!sta_mode)
    {
        Serial.println("Access point mode started");
        WiFi.softAP("kame");
        Serial.println(WiFi.softAPIP());
    }

    // Start mDNS ... so that it could be found via kame.local ... on android only works with additional app, e.g. BonjourBrowser
    Serial.println("Starting mDNS");
    MDNS.begin("kame");
    MDNS.addService("http", "tcp", 80);
}


void WiFiConfig::handleWifiSetting(AsyncWebServerRequest *request, WiFiConfig* instance){
    request->send_P(200, "text/html", instance->wifi_html);
}

void WiFiConfig::handleWifiSubmit(AsyncWebServerRequest *request) {
    // Retrieve values from the submitted form
    if (request->hasArg("ssid")) {
        ssid = request->arg("ssid");
    }
    if (request->hasArg("password")) {
        password = request->arg("password");
    }

    // Save the values to preferences
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();  // End the preferences session before reboot

    // Reboot the device
    ESP.restart();
}


void WiFiConfig::WiFiWebHandler(AsyncWebServer& server){
    server.on("/wifi-setting", HTTP_GET, [this](AsyncWebServerRequest *request){
        handleWifiSetting(request, this);
    });

    server.on("/wifi-submit", HTTP_POST, [this](AsyncWebServerRequest *request){
        request->send(200, "text/html", "Processing form...");
        handleWifiSubmit(request);
    });
}

