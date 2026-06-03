// v1_requisits.ino
// Issue #1 — Definir requisits del sistema
// Primera versió: definició de pins i llindars, sense lògica

#include <WiFi.h>
#include <WebServer.h>

// ── WIFI ──────────────────────────────────────────
const char* ssid     = "iPhone de Arturo";
const char* password = "arturo12";

// ── PINS ──────────────────────────────────────────
#define PIN_LM35    34   // LM35  → temperatura analògica
#define PIN_PIR     27   // PIR   → presència digital
#define PIN_LUZ     35   // TEMT6000 → llum analògica
#define PIN_CRASH   26   // Crash sensor → emergència HIGH actiu
#define PIN_LED     23   // LED groc ON/OFF

// ── LLINDARS ──────────────────────────────────────
#define TEMP_MAX  26.0   // °C
#define LUZ_MIN   20.0   // %

void setup() {
  Serial.begin(115200);
  Serial.println("Requisits del sistema definits:");
  Serial.println("- LM35   → GPIO 34");
  Serial.println("- PIR    → GPIO 27");
  Serial.println("- TEMT6000 → GPIO 35");
  Serial.println("- Crash  → GPIO 26");
  Serial.println("- LED    → GPIO 23");
  Serial.print("- TEMP_MAX: "); Serial.println(TEMP_MAX);
  Serial.print("- LUZ_MIN:  "); Serial.println(LUZ_MIN);
}

void loop() {
  // res encara
}
