// v2_arquitectura.ino
// Issue #2 — Arquitectura del sistema
// S'afegeix: connexió WiFi + WebServer buit + rutes definides

#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "iPhone de Arturo";
const char* password = "arturo12";

#define PIN_LM35    34
#define PIN_PIR     27
#define PIN_LUZ     35
#define PIN_CRASH   26
#define PIN_LED     23

#define TEMP_MAX  26.0
#define LUZ_MIN   20.0

WebServer server(80);

// Rutes web (buides per ara)
void handleRoot()   { server.send(200, "text/plain", "Dashboard — en construccio"); }
void handleLedOn()  { server.send(200, "text/plain", "LED ON — pendent"); }
void handleLedOff() { server.send(200, "text/plain", "LED OFF — pendent"); }
void handleReset()  { server.send(200, "text/plain", "Reset — pendent"); }

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connectant al WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connectat! IP: ");
  Serial.println(WiFi.localIP());

  server.on("/",       handleRoot);
  server.on("/ledon",  handleLedOn);
  server.on("/ledoff", handleLedOff);
  server.on("/reset",  handleReset);
  server.begin();
  Serial.println("WebServer iniciat al port 80");
}

void loop() {
  server.handleClient();
}
