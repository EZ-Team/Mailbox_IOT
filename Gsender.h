/* Gsender class helps send e-mails from Gmail account
*  using Arduino core for ESP8266 WiFi chip
*  by Boris Shobat
*  September 29 2016
*/
#ifndef G_SENDER
#define G_SENDER
#define GS_SERIAL_LOG_1         // Print to Serial only server responce
//#define GS_SERIAL_LOG_2       //  Print to Serial client commands and server responce
#include <WiFiClientSecure.h>

class Gsender
{
    protected:
        Gsender();
    private:
        uint16_t smtp_port;
        String smtp_server;
        String email_login_base64;
        String email_password_base64;
        String sender;
        const char* error;
        String mail_subject;
        String server_response;
        static Gsender* instance;
        bool await_smtp_response(WiFiClientSecure &client, const String &resp = "", uint16_t timeOut = 10000);

    public:
        static Gsender* get_instance();
        Gsender* server(const char* server);
        Gsender* server(const String &server);
        Gsender* port(uint16_t port);
        Gsender* login(const char* login);
        Gsender* login(const String &login);
        Gsender* password(const char* password);
        Gsender* password(const String &password);
        Gsender* from(const char* from);
        Gsender* from(const String &from);
        Gsender* subject(const char* subject);
        Gsender* subject(const String &subject);
        bool send(const String &to, const String &message);
        String get_last_response();
        const char* getError();
};
#endif // G_SENDER
