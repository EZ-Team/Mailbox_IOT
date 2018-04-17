#include "Gsender.h"
#include "wifi.h"

const int LASER = D5;
int valeur = 0;

void sendMailNotification() {
  Serial.begin(115200);
  wifi_connect_loop("ESGI", "Reseau-GES");
  Gsender *sender = Gsender::get_instance();
  sender->server("smtp.gmail.com")
    ->port(465)
    ->login("bWFpbGJveC5pb3QyMDE4QGdtYWlsLmNvbQo=")
    ->password("ZXptYWlsYm94MjAxOAo=")
    ->from("mailbox.iot2018@gmail.com")
    ->subject("mail test");
    if(sender->send("aurelien.duval6@gmail.com", "Vous avez reçu du courrier !")) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(sender->getError());
    }
}

void setup() {
  digitalWrite(LASER, HIGH);
  pinMode(LASER, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  valeur = analogRead(A0);

  if(valeur < 400)
    sendMailNotification();

  Serial.println(valeur);
  delay(100);
}
