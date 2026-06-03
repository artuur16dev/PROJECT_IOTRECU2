// v7_webserver_dashboard.ino
// Issue #7 — Web Server embegut — dashboard HTML complet
// S'afegeix: pàgina HTML amb auto-refresh, dades en temps real i botons

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
bool  llegirCrash() { return digitalRead(PIN_CRASH) == HIGH; }

void setLED(bool on) {
  digitalWrite(PIN_LED, on ? HIGH : LOW);
  g_ledOn = on;
  g_estat = on ? "encès" : "apagat";
}

void actualitzarLED() {
  static unsigned long t = 0;
  static bool blinkState = false;
  if (g_emergencia) {
    if (millis() - t > 300) { blinkState = !blinkState; setLED(blinkState); t = millis(); }
  } else if (g_temp > TEMP_MAX)            { setLED(true);
  } else if (g_ocupat && g_llum < LUZ_MIN) { setLED(true);
  } else                                   { setLED(false); }
}

String paginaHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="refresh" content="2">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Hospital Comarcal IoT</title>
  <style>
    body { font-family: Arial, sans-serif; background:#f4f6f8; margin:0; padding:20px; }
    .card { max-width:700px; margin:auto; background:white; padding:20px; border-radius:12px; box-shadow:0 4px 12px rgba(0,0,0,.1); }
    h1 { margin-top:0; color:#1f2937; }
    .dato { font-size:18px; margin:10px 0; }
    button { padding:10px 16px; margin:6px; border:none; border-radius:8px; cursor:pointer; }
    .btn3 { background:#16a34a; color:white; }
    .btn4 { background:#6b7280; color:white; }
    .btn2 { background:#dc2626; color:white; }
  </style>
</head>
<body>
  <div class="card">
    <h1>Hospital Comarcal IoT</h1>
)rawliteral";

  html += "<div class='dato'>Temperatura: <b>" + String(g_temp, 1) + " °C</b></div>";
  html += "<div class='dato'>Llum: <b>"        + String(g_llum, 1) + " %</b></div>";
  html += "<div class='dato'>Ocupació (PIR): <b>" + String(g_ocupat ? "Detectada" : "No") + "</b></div>";
  html += "<div class='dato'>Emergència (Crash): <b>" + String(g_emergencia ? "SI" : "NO") + "</b></div>";
  html += "<div class='dato'>LED groc: <b>" + g_estat + "</b></div>";
  html += "<p>";
  html += "<a href='/ledon'><button class='btn3'>Encendre LED</button></a>";
  html += "<a href='/ledoff'><button class='btn4'>Apagar LED</button></a>";
  html += "<a href='/reset'><button class='btn2'>Reset Emergència</button></a>";
  html += "</p>";
  html += R"rawliteral(
  </div>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot()   { server.send(200, "text/html", paginaHTML()); }
void handleReset()  { g_emergencia = false; server.sendHeader("Location", "/"); server.send(303); }
void handleLedOn()  { setLED(true);         server.sendHeader("Location", "/"); server.send(303); }
void handleLedOff() { setLED(false);        server.sendHeader("Location", "/"); server.send(303); }

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

  server.on("/",       handleRoot);
  server.on("/reset",  handleReset);
  server.on("/ledon",  handleLedOn);
  server.on("/ledoff", handleLedOff);
  server.begin();
  Serial.println("WebServer iniciat — Dashboard HTML llest");
}

void loop() {
  server.handleClient();
  g_temp   = llegirTemp();
  g_llum   = llegirLlum();
  g_ocupat = llegirPIR();
  if (llegirCrash()) g_emergencia = true;
  actualitzarLED();
  delay(100);
}
