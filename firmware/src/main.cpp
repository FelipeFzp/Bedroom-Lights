#include <Arduino.h>
#include <../lib/Helpers/WifiHelper/WiFiHelper.h>

#define PIN_LED_WIFI_STATUS D4
#define PIN_RELAY_CENTER_LIGHTS D1
#define PIN_RELAY_SIDE_LIGHTS D2

const String urlApi = "http://bedroomlights.toikos.com.br";

void getData();
void setLights(bool, bool);
String splitString(String data, char separator, int index);

void setup()
{
  pinMode(PIN_LED_WIFI_STATUS, OUTPUT);
  pinMode(PIN_RELAY_CENTER_LIGHTS, OUTPUT);
  pinMode(PIN_RELAY_SIDE_LIGHTS, OUTPUT);

  WiFiHelper::initWiFiApSta("Felipe's Bedroom", "12345678");
  WiFiHelper::connectOnWifi("Eletronica Lider 2.4GHz", "332290938");

  Serial.begin(115200);
}

void loop()
{
  WiFiHelper::serverHandleClient();

  if (millis() % 1000 == 0)
  {
    digitalWrite(PIN_LED_WIFI_STATUS, HIGH);
    getData();
    digitalWrite(PIN_LED_WIFI_STATUS, LOW);
  }
}

void getData()
{
  if (WiFiHelper::wiFiIsConnected())
  {
    String url = urlApi + "/lights/state";

    http.begin(url);

    Serial.println("[HTTP] Http begin in " + url);

    http.addHeader("Content-Type", "application/json");

    int httpCode = http.GET();
    if (httpCode > 0)
    {
      Serial.println("[HTTP] GET State: " + String(httpCode));
      String response = http.getString();

      Serial.println(response);
      Serial.flush();

      if (httpCode == 200)
      {
        String centerLightExp = splitString(response, ';', 0);
        String sideLightExp = splitString(response, ';', 1);
        String centerLightValue = splitString(centerLightExp, ':', 1);
        String sideLightValue = splitString(sideLightExp, ':', 1);
        centerLightValue.replace("\"", "");
        sideLightValue.replace("\"", "");
        setLights(centerLightValue == "true", sideLightValue == "true");
      }
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

void setLights(bool centerLight, bool sideLight)
{
  digitalWrite(PIN_RELAY_CENTER_LIGHTS, centerLight);
  digitalWrite(PIN_RELAY_SIDE_LIGHTS, sideLight);
}

String splitString(String data, char separator, int index = 0)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length();

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}