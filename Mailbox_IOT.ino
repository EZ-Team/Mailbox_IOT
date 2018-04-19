#include <ESP8266WebServer.h>
#include "Gsender.h"
#include "wifi.h"

const int LASER = D5;
const int letter_max_value = 430;

int value = 0;
int nb_letter = 0;
int nb_package = 0;
String mail_receiver = "mailbox.iot2018@gmail.com";
String mail_subject = "mailbox.iot2018@gmail.com";
long timer = 0;

ESP8266WebServer server(80);
int led_value = HIGH;

String rootHTML = "\
  <!doctype html>\n\
  <html>\n\
    <head>\n\
      <title>IOT</title>\n\
      <meta charest=\"UTF-8\">\n\
    </head>\n\
    <body>\n\
      <div>\n\
        <h1>Mailbox configurator</h1>\n\
      </div>\n\
      <div>\n\
        <form method=\"post\" enctype=\"multipart/form-data\" action=\"/configure\">\n\
          Subject : <input type=\"text\" name=\"subject\">\n\
          Receiver : <input type=\"text\" name=\"receiver\">\n\
          <input class=\"button\" type=\"submit\" value=\"Submit\">\n\
        </form>\n\
      </div>\n\
    </body>\n\
  </html>\n\
";

void handle_server_root() {
  server.send(200, "text/html", rootHTML);
}

void handle_config_change() {
  Gsender* sender = Gsender::get_instance();
  String subject = server.arg("subject");
  String receiver = server.arg("receiver");

  mail_subject = subject;
  mail_receiver = receiver;
  Serial.printf("%s is receiver\n", receiver.c_str());
  Serial.printf("%s is subject\n", subject.c_str());

  server.send(200, "text/plain", "OK");
}

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

  Gsender *sender = setMailParams("smtp.gmail.com", 465, "bWFpbGJveC5pb3QyMDE4QGdtYWlsLmNvbQo=", "ZXptYWlsYm94MjAxOAo=", "mailbox.iot2018@gmail.com", mail_subject);

    if(sender->send(recipient, body)) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
//        Serial.println(sender->getError());
    }
}

void setup() {
  Serial.begin(115200);
  wifi_connect_loop("", "");
  nb_letter = 0;
  nb_package = 0;
  
  server.on("/", handle_server_root);
  server.on("/configure", HTTP_POST,                       // if the client posts to the upload page
    handle_config_change
  );
  server.begin();

  digitalWrite(LASER, HIGH);
  pinMode(LASER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  value = analogRead(A0);
  Serial.println(value);

  if((nb_letter == 0 || nb_package == 0) && value < letter_max_value) {
    delay(5000);
    value = analogRead(A0);
    Serial.println(value);

    if(value < letter_max_value && nb_package == 0) {
      sendMailNotification(mail_receiver, "You have received a package. Remember to go check your mailbox ! ;)");
      nb_package++;
    } else if(nb_package == 0) {
      sendMailNotification(mail_receiver, "You have received a letter. Remember to go check your mailbox ! ;)");
      nb_letter++;
    }
  }

//  Serial.print("0 600 ");
//  Serial.println(value);
  delay(25);
}

