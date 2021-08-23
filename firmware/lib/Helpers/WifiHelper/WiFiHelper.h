#ifndef WIFIHELPER_H
#define WIFIHELPER_H

#include "Arduino.h"
#include "functional"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFiMulti.h"
#include "WifiClient.h"

#include "./wifi-connect.html.h"

#define WIFI_NOT_CONNECTED "WIFI_NOT_CONNECTED"

ESP8266WebServer server(80);
ESP8266WiFiMulti wiFiMulti;
HTTPClient http;
WiFiClient wifiClient;

class WiFiHelper
{

public:
    static void setWiFiConnectionRoutes(std::function<void()> onConnect)
    {
        server.on("/", HTTP_GET, []()
                  { server.send(200, "text/html", pageWiFiConnect); });

        server.on("/set-wifi", HTTP_POST, [onConnect]()
                  {
                      String ssid = server.arg("ssid");
                      String password = server.arg("password");

                      Serial.println(ssid + ";" + password);

                      String result = connectOnWifi(ssid, password);

                      if (result != WIFI_NOT_CONNECTED)
                      {
                          onConnect();
                      }

                      server.send(200, "text/plain", result.c_str());
                  });
    }

    static void initWiFiApSta(String ssid)
    {
        initWiFiApSta(ssid, "", []() {});
    }

    static void initWiFiApSta(String ssid, String password)
    {
        initWiFiApSta(ssid, password, []() {});
    }

    static void initWiFiApSta(String ssid, String password, std::function<void()> onConnect)
    {
        WiFiHelper::__wiFiHelperLog("Setting soft-AP...");

        WiFi.mode(WIFI_AP_STA);

        IPAddress ip(10, 0, 0, 1);
        IPAddress gateway(10, 0, 0, 254);
        IPAddress subnet(255, 255, 255, 0);
        WiFi.softAPConfig(ip, gateway, subnet);

        bool result = WiFi.softAP(ssid.c_str(), password.c_str());

        if (result == true)
            WiFiHelper::__wiFiHelperLog("Ready");
        else
            WiFiHelper::__wiFiHelperLog("Failed!");

        WiFiHelper::__wiFiHelperLog("IP address: ");
        WiFiHelper::__wiFiHelperLog(IpAddressToString(WiFi.softAPIP()));

        WiFiHelper::setWiFiConnectionRoutes(onConnect);

        server.begin();
    }

    static String connectOnWifi()
    {
        return connectOnWifi("", "");
    }

    static String connectOnWifi(String ssid)
    {
        return connectOnWifi(ssid, "");
    }

    static String connectOnWifi(String ssid, String password)
    {
        return connectOnWifi(ssid, password, 20);
    }

    static String connectOnWifi(String ssid, String password, int timeOut)
    {
        Serial.println("Connecting on wifi...");

        wiFiMulti.addAP(ssid.c_str(), password.c_str());

        int count = 0;
        while (wiFiMulti.run() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(500);

            if (count >= timeOut)
            {
                return WIFI_NOT_CONNECTED;
            }

            count++;
        }

        IPAddress ip = WiFi.localIP();

        Serial.print("\nConnected as ");
        Serial.println(ip);

        return WiFiHelper::IpAddressToString(ip);
    }

    static bool wiFiIsConnected()
    {
        return WiFi.isConnected();
    }

    static void serverHandleClient()
    {
        server.handleClient();
    }

    static void __wiFiHelperLog(String text)
    {
        Serial.println("[WiFi] " + text);
    }

    static String IpAddressToString(const IPAddress &ipAddress)
    {
        return String(ipAddress[0]) + String(".") +
               String(ipAddress[1]) + String(".") +
               String(ipAddress[2]) + String(".") +
               String(ipAddress[3]);
    }
};

#endif