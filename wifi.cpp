#include "wifi.h"

bool wifi_connect(const char* ssid, const char* password) {
  uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if(ssid) {
      WiFi.begin(ssid, password);
      Serial.println(ssid);
  } else {
    Serial.println("Given SSID is not set");
    return false;
  }

  uint8_t i = 0;
  while(WiFi.status()!= WL_CONNECTED && i++ < 50)
  {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if(i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void wifi_connect_loop(const char* ssid, const char* password, uint16_t reconnect_interval) {
  uint32_t ts = millis();
  bool connection_state = false;
  while(!connection_state)
  {
    connection_state = wifi_connect(ssid, password);
    if(!connection_state) {
      delay(reconnect_interval);
    }
  }
}
