#include "Gsender.h"
Gsender* Gsender::instance = 0;
Gsender::Gsender(){}
Gsender* Gsender::get_instance()
{
    if (instance == 0)
        instance = new Gsender;
    return instance;
}

Gsender* Gsender::server(const char* server) {
  this->smtp_server = String(server);
  return this->instance;
}

Gsender* Gsender::server(const String &server) {
  return this->server(server.c_str());
}

Gsender* Gsender::port(uint16_t port){
  this->smtp_port = port;
  return this->instance;
}

Gsender* Gsender::login(const char* login){
  this->email_login_base64 = String(login);
  return this->instance;
}

Gsender* Gsender::login(const String &login){
  return this->login(login.c_str());
}

Gsender* Gsender::password(const char* password) {
  this->email_password_base64 = String(password);
  return this->instance;
}

Gsender* Gsender::password(const String &password) {
  return this->password(password.c_str());
}

Gsender* Gsender::from(const char* from) {
  this->sender = String(from);
  return this->instance;
}

Gsender* Gsender::from(const String &from) {
  return this->from(from.c_str());
}

Gsender* Gsender::subject(const char* subject)
{
  this->mail_subject = String(subject);
  return this->instance;
}
Gsender* Gsender::subject(const String &subject)
{
  return this->subject(subject.c_str());
}

bool Gsender::await_smtp_response(WiFiClientSecure &client, const String &resp, uint16_t timeOut)
{
  uint32_t ts = millis();
  while (!client.available())
  {
    if(millis() > (ts + timeOut)) {
      this->error = "SMTP Response TIMEOUT!";
      return false;
    }
  }
  this->server_response = client.readStringUntil('\n');
#if defined(GS_SERIAL_LOG_1) || defined(GS_SERIAL_LOG_2)
  Serial.println(this->server_response);
#endif
  if (resp && this->server_response.indexOf(resp) == -1) return false;
  return true;
}

String Gsender::get_last_response()
{
  return this->server_response;
}

const char* Gsender::getError()
{
  return this->error;
}

bool Gsender::send(const String &to, const String &message)
{
  WiFiClientSecure client;
#if defined(GS_SERIAL_LOG_2)
  Serial.print("Connecting to :");
  Serial.println(this->smtp_server);
#endif
  if(!client.connect(this->smtp_server.c_str(), this->smtp_port)) {
    this->error = "Could not connect to mail server";
    return false;
  }
  if(!this->await_smtp_response(client, "220")) {
    this->error = "Connection Error";
    return false;
  }

#if defined(GS_SERIAL_LOG_2)
  Serial.println("HELO friend:");
#endif
  client.println("HELO friend");
  if(!this->await_smtp_response(client, "250")){
    this->error = "identification error";
    return false;
  }

#if defined(GS_SERIAL_LOG_2)
  Serial.println("AUTH LOGIN:");
#endif
  client.println("AUTH LOGIN");
  this->await_smtp_response(client);

#if defined(GS_SERIAL_LOG_2)
  Serial.println("EMAILBASE64_LOGIN:");
#endif
  client.println(this->email_login_base64);
  this->await_smtp_response(client);

#if defined(GS_SERIAL_LOG_2)
  Serial.println("EMAILBASE64_PASSWORD:");
#endif
  client.println(this->email_password_base64);
  if (!this->await_smtp_response(client, "235")) {
    this->error = "SMTP AUTH error";
    return false;
  }

  String mailfrom = "MAIL from: <" + String(this->sender) + '>';
#if defined(GS_SERIAL_LOG_2)
  Serial.println(mailfrom);
#endif
  client.println(mailfrom);
  this->await_smtp_response(client);

  String rcpt = "RCPT TO: <" + to + '>';
#if defined(GS_SERIAL_LOG_2)
  Serial.println(rcpt);
#endif
  client.println(rcpt);
  this->await_smtp_response(client);

#if defined(GS_SERIAL_LOG_2)
  Serial.println("DATA:");
#endif
  client.println("DATA");
  if(!this->await_smtp_response(client, "354")) {
    this->error = "SMTP DATA error";
    return false;
  }

  client.println("From: <" + String(this->sender) + '>');
  client.println("To: <" + to + '>');

  client.print("Subject: ");
  client.println(this->mail_subject);

  client.println("Mime-Version: 1.0");
  client.println("Content-Type: text/html; charset=\"UTF-8\"");
  client.println("Content-Transfer-Encoding: 7bit");
  client.println();
  String body = "<!DOCTYPE html><html lang=\"en\">" + message + "</html>";
  client.println(body);
  client.println(".");
  if (!this->await_smtp_response(client, "250")) {
    error = "Sending message error";
    return false;
  }
  client.println("QUIT");
  if (!this->await_smtp_response(client, "221")) {
    this->error = "SMTP QUIT error";
    return false;
  }
  return true;
}
