// v3_sensor_lm35.ino
// Issue #3 — Sensor LM35 — temperatura
// S'afegeix: lectura real del LM35 i print per Serial

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

float g_temp = 0.0;

// GPIO 34, ADC 12 bits, ref 3.3V
// Fórmula: raw × (3.3 / 4095) × 100 = °C
float llegirTemp() {
  int raw = analogRead(PIN_LM35);
  float v = raw * (3.3 / 4095.0);
  return v * 100.0;
}

void handleRoot() {
  String msg = "Temperatura: " + String(g_temp, 1) + " C";
  server.send(200, "text/plain", msg);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LM35, INPUT);

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
  g_temp = llegirTemp();
  Serial.print("Temp: ");
  Serial.print(g_temp, 1);
  Serial.println(" C");
  delay(1000);
}
