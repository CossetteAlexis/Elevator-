#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

SoftwareSerial Esp(4,0);

const char *ssid = "Cossette";  
const char *password = "12345678";

ESP8266WebServer server(80);



void setup() {
  Serial.begin(115200);
  Esp.begin(9600);


  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
  }
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.on("/4floor", handle_4floor);
  server.on("/3floor", handle_3floor);
  server.on("/2floor", handle_2floor);
  server.on("/1floor", handle_1floor);
  server.begin();
}

void loop() {

if(Esp.available())
    {
      char Read = Esp.read();
      Esp.write(Read);
    }
    server.handleClient();


if(Serial.available())
    {
      char Read = Serial.read();
      Serial.write(Read);
    }
    server.handleClient();
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_4floor() {
Serial.write('d');
  server.send(200, "text/html", SendHTML()); 
}

void handle_3floor() {
  Serial.write('c');
  server.send(200, "text/html", SendHTML()); 
}

void handle_2floor() {
Serial.write('b');
  server.send(200, "text/html", SendHTML()); 
}

void handle_1floor() {
Serial.write('a');
  server.send(200, "text/html", SendHTML()); 
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Elevator Web Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Elevator Web Control</h1>\n";
  ptr +="<p>4floor</p><a class=\"button button-off\" href=\"/4floor\">4</a>\n";
  ptr +="<p>3floor</p><a class=\"button button-off\" href=\"/3floor\">3</a>\n";
  ptr +="<p>2floor</p><a class=\"button button-off\" href=\"/2floor\">2</a>\n";
  ptr +="<p>1floor</p><a class=\"button button-off\" href=\"/1floor\">1</a>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
