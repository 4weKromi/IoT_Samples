
/*
	Using Access Point (AP)
	Two Relay Switches
  Web Interface to toggle switches
  
  created by 4wekromi
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "kromi_home";  
const char* password = "qwer1234";

/* your static IP */
IPAddress local_ip(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

uint8_t swOnePin = D2;
bool swOneState = LOW;
uint8_t swTwoPin = D3;
bool swTwoState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(swOnePin, OUTPUT);
  pinMode(swTwoPin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/swone_on", handle_swone_on);
  server.on("/swone_off", handle_swone_off);
  server.on("/swtwo_on", handle_swtwo_on);
  server.on("/swtwo_off", handle_swtwo_off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if (swOneState)
    digitalWrite(swOnePin, HIGH);
  else
    digitalWrite(swOnePin, LOW);

  if (swTwoState)
    digitalWrite(swTwoPin, HIGH);
  else
    digitalWrite(swTwoPin, LOW);
}

void handle_OnConnect() {
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  swOneState = LOW;
  swTwoState = LOW;
  server.send(200, "text/html", SendHTML(false, false));
}
//switch 1
void handle_swone_on() {
  swOneState = HIGH;
  server.send(200, "text/html", SendHTML(true, swTwoState));
}
void handle_swone_off() {
  swOneState = LOW;
  server.send(200, "text/html", SendHTML(false, swTwoState));
}
//switch 2
void handle_swtwo_on() {
  swTwoState = HIGH;
  server.send(200, "text/html", SendHTML(swOneState, true));
}
void handle_swtwo_off() {
  swTwoState = LOW;
  server.send(200, "text/html", SendHTML(swOneState, false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t swOne, uint8_t swTwo) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">\n";
  ptr += "<head>\n";
  ptr += "<title>Home Control (IoT)</title>\n";
  ptr += "<style>input[type=button]{width: 50px;  height: 40px;} body{text-align: center;}</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h2 style='color:#1f5ed3'>Welcome Home</h2><br>\n";
  ptr += "<form method=\"get\">\n";
  if (swOne) {
    ptr += "<p>Switch #1 : Status: ON</p><input type=\"button\" value=\"OFF\" onclick=\"window.location.href='/swone_off'\">\n";
  }
  else {
    ptr += "<p>Switch #1 : Status: OFF</p><input type=\"button\" value=\"ON\" onclick=\"window.location.href='/swone_on'\">\n";
  }
  ptr += " <br><br>";
  if (swTwo) {
    ptr += "<p>Switch #2 : Status: ON</p><input type=\"button\" value=\"OFF\" onclick=\"window.location.href='/swtwo_off'\">\n";
  }
  else {
    ptr += "<p>Switch #2 : Status: OFF</p><input type=\"button\" value=\"ON\" onclick=\"window.location.href='/swtwo_on'\">\n";
  }
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
