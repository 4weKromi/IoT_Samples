/*	Using Access Point (AP)
	Two Relay Switches
	Web Interface to toggle switches
	by 4wekromi		*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

void OnConnect();
void SwOneOn();
void SwOneOff();
void SwTwoOn();
void SwTwoOff();
void NotFound();
String sendHTML(uint8_t stat1, uint8_t stat2);

/* Put your SSID & Password */
const char* ssid = "kromi_home";  
const char* password = "qwer1234";

/* your static IP */
IPAddress local_ip(192, 168, 0, 2);
IPAddress gateway(192, 168, 0, 1);
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
  swOneState = LOW;
  swTwoState = LOW;
  server.on("/", OnConnect);
  server.on("/swone_on", SwOneOn);
  server.on("/swone_off", SwOneOff);
  server.on("/swtwo_on", SwTwoOn);
  server.on("/swtwo_off", SwTwoOff);
  server.onNotFound(NotFound);
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

void OnConnect() {
  Serial.println("IP address: ");
  Serial.println(local_ip);
  server.send(200, "text/html", SendHTML(false, false));
}
//switch 1
void SwOneOn() {
  swOneState = HIGH;
  server.send(200, "text/html", SendHTML(true, swTwoState));
}
void SwOneOff() {
  swOneState = LOW;
  server.send(200, "text/html", SendHTML(false, swTwoState));
}
//switch 2
void SwTwoOn() {
  swTwoState = HIGH;
  server.send(200, "text/html", SendHTML(swOneState, true));
}
void SwTwoOff() {
  swTwoState = LOW;
  server.send(200, "text/html", SendHTML(swOneState, false));
}

void NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t swOne, uint8_t swTwo) {
  String ptr = "<!DOCTYPE html>\n<html>\n";
  ptr +="<head><meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=yes'>\n";
  ptr += "<title>Home Control (IoT)</title>\n";
  ptr += "<style>input[type=button]{width: 50px;  height: 40px;} body{text-align: center;}</style>\n";
  ptr += "<meta http-equiv='refresh' content='10'/></head>\n<body>\n";
  ptr += "<h2 style='color:#1f5ed3'>Welcome Home</h2><br>\n";
  ptr += "<form method=\"get\">\n";
  if (swOne||swOneState)
    ptr += "<p>Switch #1 : Status: OFF</p><input type='button' value='ON' onclick=\"window.location.href='/swone_off'\">\n";
  else
    ptr += "<p>Switch #1 : Status: ON</p><input type='button' value='OFF' onclick=\"window.location.href='/swone_on'\">\n";
  ptr += " <br><br>";
  if (swTwo||swTwoState)
    ptr += "<p>Switch #2 : Status: OFF</p><input type='button' value='ON' onclick=\"window.location.href='/swtwo_off'\">\n";
  else
    ptr += "<p>Switch #2 : Status: ON</p><input type='button' value='OFF' onclick=\"window.location.href='/swtwo_on'\">\n";
  ptr += "</form>\n</body>\n</html>\n";
  return ptr;
}
