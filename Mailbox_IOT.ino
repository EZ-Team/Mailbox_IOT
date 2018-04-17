#include "Gsender.h"
#include "wifi.h"

void setup() {
  Serial.begin(115200);
  wifi_connect_loop("ESGI", "Reseau-GES");
  Gsender *sender = Gsender::get_instance();
  sender->server("smtp.gmail.com")
    ->port(465)
    ->login("bWFpbGJveC5pb3QyMDE4QGdtYWlsLmNvbQo=")
    ->password("ZXptYWlsYm94MjAxOAo=")
    ->from("mailbox.iot2018@gmail.com")
    ->subject("mail test");
    if(sender->send("aurelien.duval6@gmail.com", "Setup test")) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(sender->getError());
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}
