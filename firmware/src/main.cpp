#include <Arduino.h>
#include <../lib/Helpers/WifiHelper/WiFiHelper.h>

#define PIN_LED_WIFI_STATUS D0

const String urlApi = "http://bedroomlights.toikos.com.br";
const String deviceId = "66574";

void getData();

void setup()
{
  pinMode(PIN_LED_WIFI_STATUS, OUTPUT);

  WiFiHelper::initWiFiApSta("Felipe's Bedroom", "12345678");
  WiFiHelper::connectOnWifi("Eletronica Lider 2.4GHz", "332290938");

  Serial.begin(115200);
}

void loop()
{
  WiFiHelper::serverHandleClient();

  if (WiFiHelper::wiFiIsConnected())
    digitalWrite(PIN_LED_WIFI_STATUS, HIGH);
  else
    digitalWrite(PIN_LED_WIFI_STATUS, LOW);

  if (millis() % 1000 == 0)
  {
    Serial.println("1 Seg");
    getData();
  }
}

void getData()
{
  if (WiFiHelper::wiFiIsConnected())
  {
    String url = urlApi + "/lights";

    http.begin(url);

    Serial.println("[HTTP] Http begin in " + url);

    http.addHeader("Content-Type", "application/json");

    int httpCode = http.GET();
    if (httpCode > 0)
    {

      Serial.println("[HTTP] GET " + String(httpCode));
      String response = http.getString();

      Serial.println(response);
      Serial.flush();

      // if (httpCode == 200)
      // {
      //   Serial.println(response);
      // }
    }
    else
    {
      Serial.println("[HTTP] GET failed, error: (" + String(httpCode) + ") " + http.errorToString(httpCode));
    }

    http.end();
  }
  else
  {
    Serial.println("Wifi Not connected");
  }
}