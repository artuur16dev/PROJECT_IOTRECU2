// v4_sensors_pir_llum.ino
// Issue #4 — Sensors PIR + TEMT6000 — presència i llum
// S'afegeix: lectura PIR (presència) i TEMT6000 (llum %)

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

float g_temp   = 0.0;
float g_llum   = 0.0;
bool  g_ocupat = false;

float llegirTemp() {
  int raw = analogRead(PIN_LM35);
  float v = raw * (3.3 / 4095.0);
  return v * 100.0;
}

// PIR: HIGH = presència detectada
bool llegirPIR() {
  return digitalRead(PIN_PIR) == HIGH;
}

// TEMT6000: 0–4095 → 0–100%
float llegirLlum() {
  return (analogRead(PIN_LUZ) / 4095.0) * 100.0;
}

void handleRoot() {
  String msg  = "Temperatura: " + String(g_temp, 1) + " C\n";
         msg += "Llum: "        + String(g_llum, 1) + " %\n";
         msg += "Ocupat (PIR): "+ String(g_ocupat ? "Si" : "No");
  server.send(200, "text/plain", msg);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LM35, INPUT);
  pinMode(PIN_LUZ,  INPUT);
  pinMode(PIN_PIR,  INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connectant al WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  g_temp   = llegirTemp();
  g_llum   = llegirLlum();
  g_ocupat = llegirPIR();

  Serial.print("Temp: ");   Serial.print(g_temp, 1);   Serial.print(" C | ");
  Serial.print("Llum: ");   Serial.print(g_llum, 1);   Serial.print(" % | ");
  Serial.print("Ocupat: "); Serial.println(g_ocupat ? "Si" : "No");

  delay(1000);
}
