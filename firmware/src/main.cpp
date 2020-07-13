#include <Arduino.h>
#include <../lib/Helpers/WifiHelper/WiFiHelper.h>

#define PIN_LED_WIFI_STATUS D4
#define PIN_RELAY_CENTER_LIGHTS D1
#define PIN_RELAY_SIDE_LIGHTS D2

const String urlApi = "http://bedroomlights.toikos.com.br";

void getData();
void setLights(bool, bool);

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
        if (response == "\"full\"")
        {
          Serial.println("Totalmente ligada");
          setLights(1, 1);
        }
        else if (response == "\"side\"")
        {
          Serial.println("Laterais ligadas");
          setLights(0, 1);
        }
        else if (response == "\"center\"")
        {
          Serial.println("Laterais ligadas");
          setLights(1, 0);
        }
        else if (response == "\"off\"")
        {
          Serial.println("Desligado");
          setLights(0, 0);
        }
        else {
          Serial.println("ERRO: Estado Desconhecido !!!" + String(response));
          setLights(0, 0);
        }
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