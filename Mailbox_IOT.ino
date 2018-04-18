#include "Gsender.h"
#include "wifi.h"

const int LASER = D5;
const int letter_max_value = 430;
const int package_max_value = 200;

int valeur = 0;
int cpt = 0;

Gsender* setMailParams(String smtp_server, int port, String login, String password, String mail_sender, String subject) {
  Gsender *sender = Gsender::get_instance();
  sender->server(smtp_server)
    ->port(port)
    ->login(login)
    ->password(password)
    ->from(mail_sender)
    ->subject(subject);

  return sender;
}

void sendMailNotification(String recipient, String body) {
  Serial.begin(115200);
  wifi_connect_loop("ESGI", "Reseau-GES");

  Gsender *sender = setMailParams("smtp.gmail.com", 465, "bWFpbGJveC5pb3QyMDE4QGdtYWlsLmNvbQo=", "ZXptYWlsYm94MjAxOAo=", "mailbox.iot2018@gmail.com", "Vous avez reçu du courrier !");

    if(sender->send(recipient, body)) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
//        Serial.println(sender->getError());
    }
}

void setup() {
  cpt = 0;
  
  digitalWrite(LASER, HIGH);
  pinMode(LASER, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  valeur = analogRead(A0);

 if(cpt != 1) {
   if(valeur < letter_max_value && valeur > package_max_value) {
     sendMailNotification("mailbox.iot2018@gmail.com", "Vous avez reçu une lettre. Pensez à aller vérifier votre boîte aux lettres ! ;)");
      cpt++;
   } else if (valeur < package_max_value) {
     sendMailNotification("mailbox.iot2018@gmail.com", "Vous avez reçu un colis. Pensez à aller vérifier votre boîte aux lettres ! ;)");
     cpt++;
   }
 }

  Serial.print("0 600 ");
  Serial.println(valeur);
  delay(25);
}
