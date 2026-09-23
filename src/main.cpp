#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

const char* ssid = "AFFA_2.4G";
const char* password = "Affa@2307";

WebServer server(80);


// ======================================
// Página principal
// ======================================

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


// ======================================
// CSS
// ======================================

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


// ======================================
// JavaScript
// ======================================

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


// ======================================
// SETUP
// ======================================

void setup()
{
    Serial.begin(115200);

    // Inicializa LittleFS
    if (!LittleFS.begin(true))
    {
        Serial.println("Erro ao inicializar LittleFS!");
        return;
    }

    Serial.println("LittleFS inicializado!");


    // ==================================
    // Verifica se o index.html existe
    // ==================================

    File file = LittleFS.open("/index.html", "r");

    if (file)
    {
        Serial.println("index.html encontrado!");

        Serial.print("Tamanho: ");
        Serial.println(file.size());

        file.close();
    }
    else
    {
        Serial.println("ERRO: index.html NAO encontrado!");
    }


    // ==================================
    // Conecta ao Wi-Fi
    // ==================================

    Serial.println();
    Serial.print("Conectando ao Wi-Fi");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    Serial.println("Wi-Fi conectado!");

    Serial.print("IP do ESP32: ");
    Serial.println(WiFi.localIP());


    // ==================================
    // Rotas
    // ==================================

    server.on("/", HTTP_GET, handleRoot);

    server.on("/style.css", HTTP_GET, handleCSS);

    server.on("/script.js", HTTP_GET, handleJS);


    // ==================================
    // Inicia servidor
    // ==================================

    server.begin();

    Serial.println("Servidor HTTP iniciado!");
}


// ======================================
// LOOP
// ======================================

void loop()
{
    server.handleClient();
}