#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define LED_PIN 2

const char *ssid = "AFFA_2.4G";
const char *password = "Affa@2307";

WebServer server(80);

bool ledState = false;

// Declarar as funções antes de usá-las
void handleLedState();

// =============================
// HTML
// =============================
void handleRoot()
{
  File file = LittleFS.open("/index.html", "r");

  if (!file)
  {
    server.send(404, "text/plain", "index.html nao encontrado");
    return;
  }

  server.streamFile(file, "text/html");
  file.close();
}

// =============================.
// CSS
// =============================
void handleCSS()
{
  File file = LittleFS.open("/style.css", "r");

  if (!file)
  {
    server.send(404, "text/plain", "style.css nao encontrado");
    return;
  }

  server.streamFile(file, "text/css");
  file.close();
}

// =============================
// JavaScript
// =============================
void handleJS()
{
  File file = LittleFS.open("/script.js", "r");

  if (!file)
  {
    server.send(404, "text/plain", "script.js nao encontrado");
    return;
  }

  server.streamFile(file, "application/javascript");
  file.close();
}

// =============================
// API STATUS
// =============================
void handleStatus()
{
  String json = "{";
  json += "\"status\":\"online\",";
  json += "\"led\":";
  json += ledState ? "\"ligado\"" : "\"desligado\"";
  json += "}";

  server.send(200, "application/json", json);
}

// =============================
// FUNÇÕES AUXILIARES
// =============================
void handleLedState()
{
  String body = server.arg("plain");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error)
  {
    server.send(400, "application/json", "{\"erro\":\"JSON invalido\"}");
    return;
  }

  ledState = doc["state"];

  digitalWrite(LED_PIN, ledState ? HIGH : LOW);

  Serial.print(ledState ? "Led Ligado" : "Led Desligado");

  // IMPORTANTE: responder ao navegador
  String json = "{";
  json += "\"status\":\"ok\",";
  json += "\"led\":";
  json += ledState ? "\"ligado\"" : "\"desligado\"";
  json += "}";

  server.send(200, "application/json", json);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // LittleFS
  if (!LittleFS.begin(true))
  {
    Serial.println("Erro ao inicializar LittleFS!");
    return;
  }

  // Wi-Fi
  Serial.println();
  Serial.print("Conectando ao Wi-Fi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wi-Fi conectado!");

  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());

  // Rotas da página
  server.on("/", HTTP_GET, handleRoot);
  server.on("/style.css", HTTP_GET, handleCSS);
  server.on("/script.js", HTTP_GET, handleJS);

  // Rotas da API
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/ledState", HTTP_POST, handleLedState);

  server.begin();

  Serial.println("Servidor HTTP iniciado!");
}

void loop()
{
  server.handleClient();
}
