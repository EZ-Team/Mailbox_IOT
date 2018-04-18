#include "Gsender.h"
#include "wifi.h"

const int LASER = D5;
const int letter_max_value = 430;

int valeur = 0;
int nb_letter = 0;
int nb_package = 0;

long timer = 0;

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
  nb_letter = 0;
  nb_package = 0;
  
  digitalWrite(LASER, HIGH);
  pinMode(LASER, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  valeur = analogRead(A0);
  Serial.println(valeur);

  if((nb_letter == 0 || nb_package == 0) && valeur < letter_max_value) {
    delay(5000);
    valeur = analogRead(A0);
    Serial.println(valeur);

    if(valeur < letter_max_value && nb_package == 0) {
      sendMailNotification("mailbox.iot2018@gmail.com", "Vous avez reçu un colis. Pensez à aller vérifier votre boîte aux lettres ! ;)");
      nb_package++;
    } else if(nb_package == 0) {
      sendMailNotification("mailbox.iot2018@gmail.com", "Vous avez reçu une lettre. Pensez à aller vérifier votre boîte aux lettres ! ;)");
      nb_letter++;
    }
  }

//  Serial.print("0 600 ");
//  Serial.println(valeur);
  delay(25);
}
