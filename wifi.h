#ifndef MAILBOX_WIFI_H
#define MAILBOX_WIFI_H
  #include <ESP8266WiFi.h>

  void wifi_connect_loop(const char* ssid, const char* password, uint16_t reconnect_interval = 10000);
  bool wifi_connect(const char* ssid, const char* password);

#endif

