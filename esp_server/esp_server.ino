
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define AP_SSID "esp_server_01"
#define AP_PSK  "thisisnotserver"


const char* ssid = AP_SSID;
const char* password = AP_PSK;

ESP8266WebServer httpServer(80);

IPAddress hostIp(192,168,4,2);

IPAddress gateway(192,168,4,1);

IPAddress subnet(255,255,255,0);

const String indexPage = "<html>\n\
  <head>\n\
    <title>ESP8266 Server</title>\n\
  </head>\n\
  <body>\n\
    <h1>Welcome to ESP Server!</h1><br>\n\
    <a href=\"get\">Click to try GET</a><br>\n\
    <a href=\"post\">Click to try POST</a><br>\n\
  </body>\n\
</html>\n";

const String getForm = "<html>\n\
  <head>\n\
    <title>ESP8266 Server</title>\n\
  </head>\n\
  <body>\n\
    <h1>GET form test</h1><br>\n\
    <form method=\"get\" enctype=\"text/plain\" action=\"/getform\">\n\
      <label>Nom :</label>\
      <input type=\"text\" name=\"name\"><br>\n\
      <input type=\"submit\" value=\"Submit\">\n\
    </form>\n\
  </body>\n\
</html>\n";

const String postForm = "<html>\n\
  <head>\n\
    <title>ESP8266 Server</title>\n\
  </head>\n\
  <body>\n\
    <h1>POST form test</h1><br>\n\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postform\">\n\
      <label>Nom :</label>\
      <input type=\"text\" name=\"name\"><br>\n\
      <input type=\"submit\" value=\"Submit\">\n\
    </form>\n\
  </body>\n\
</html>\n";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Booting Sketch...");
  Serial.println("Start wifi as SoftAP...");
  WiFi.mode(WIFI_AP);
  delay(10);
  WiFi.softAPConfig(hostIp, gateway, subnet);
  delay(10);
  boolean result = WiFi.softAP(AP_SSID, AP_PSK);
  if(result == true)
  {
    Serial.println("AP is ready");
  }
  else
  {
    Serial.println("AP starting is failed");
  }
  delay(10);
  hostIp = WiFi.softAPIP();

  httpServer.on("", handleRoot);
  httpServer.on("/", handleRoot);
  httpServer.onNotFound(handleNotFound);
  httpServer.on("/get", handleGet);
  httpServer.on("/get/", handleGet);
  httpServer.on("/getform", handleGetForm);
  httpServer.on("/post", handlePost);
  httpServer.on("/post/", handlePost);
  httpServer.on("/postform", handlePostForm);
  
  httpServer.begin();

  Serial.printf("HTTP Server is ready, please access http://%d.%d.%d.%d in your browser\n", hostIp[0], hostIp[1], hostIp[2], hostIp[3]);
}

void loop() {
  // put your main code here, to run repeatedly:
  httpServer.handleClient();   // Listen to HTTP requests
}

void handleRoot() {
  httpServer.send(200, "text/html", indexPage);   // Send HTTP status 200 (Ok)
}

void handleNotFound(){
  httpServer.send(404, "text/html", "404, You cannot fly to the moon now!"); // Send HTTP status 404 (Not Found)
}

void handleGet() {
  httpServer.send(200, "text/html", getForm);   // Send HTTP status 200 (Ok)
}

void handleGetForm(){
    String message = "GET arguments were:\n";
    for (uint8_t i = 0; i < httpServer.args(); i++) {
      message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
    }
    httpServer.send(200, "text/plain", message);
}

void handlePost() {
  httpServer.send(200, "text/html", postForm);   // Send HTTP status 200 (Ok)
}

void handlePostForm(){
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < httpServer.args(); i++) {
      message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
    }
    httpServer.send(200, "text/plain", message);
}
