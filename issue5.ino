// v5_crash_led.ino
// Issue #5 — Crash sensor + LED groc — emergència
// S'afegeix: lectura crash sensor i control del LED

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

float  g_temp       = 0.0;
float  g_llum       = 0.0;
bool   g_ocupat     = false;
bool   g_emergencia = false;
bool   g_ledOn      = false;
String g_estat      = "apagat";

float llegirTemp()  { int r = analogRead(PIN_LM35); return r * (3.3/4095.0) * 100.0; }
float llegirLlum()  { return (analogRead(PIN_LUZ) / 4095.0) * 100.0; }
bool  llegirPIR()   { return digitalRead(PIN_PIR)   == HIGH; }

// Crash sensor: HIGH actiu (sense INPUT_PULLUP)
// ⚠️ Verificar al hardware físic
bool llegirCrash() {
  return digitalRead(PIN_CRASH) == HIGH;
}

void setLED(bool on) {
  digitalWrite(PIN_LED, on ? HIGH : LOW);
  g_ledOn = on;
  g_estat = on ? "ences" : "apagat";
}

void handleRoot() {
  String msg  = "Emergencia: " + String(g_emergencia ? "SI" : "NO") + "\n";
         msg += "LED: " + g_estat;
  server.send(200, "text/plain", msg);
}

void handleReset() {
  g_emergencia = false;
  setLED(false);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PIR,   INPUT);
  pinMode(PIN_CRASH, INPUT);
  pinMode(PIN_LM35,  INPUT);
  pinMode(PIN_LUZ,   INPUT);
  pinMode(PIN_LED,   OUTPUT);
  setLED(false);

  WiFi.begin(ssid, password);
  Serial.print("Connectant al WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/",      handleRoot);
  server.on("/reset", handleReset);
  server.begin();
}

void loop() {
  server.handleClient();

  g_temp   = llegirTemp();
  g_llum   = llegirLlum();
  g_ocupat = llegirPIR();

  if (llegirCrash()) {
    g_emergencia = true;
    setLED(true);
    Serial.println("EMERGENCIA activada!");
  }

  delay(100);
}
