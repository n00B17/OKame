//WiFiConfig.h
#ifndef WiFiConfig_h
#define WiFiConfig_h

#include <Arduino.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include "WiFi.h"
#include <ESPAsyncWebServer.h> 


class WiFiConfig {
    public:
        WiFiConfig();   //constructor

        //call for wifi settings page
        //const char* getWiFiHtml() const;

        void WiFiSetup();
        void WiFiWebHandler(AsyncWebServer& server);
        
    private:
        Preferences preferences;  

        String ssid;
        String password;
        // //create a boolean sta_mode = true;
        static boolean sta_mode;

        //wifi settings page
        const char* wifi_html;
        static void handleWifiSetting(AsyncWebServerRequest *request, WiFiConfig* instance);
        void handleWifiSubmit(AsyncWebServerRequest *request);
        

};
#endif