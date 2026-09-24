# ESP32 Web Server

Projeto de um servidor web embarcado utilizando **ESP32**, com interface web em **HTML, CSS e JavaScript** armazenada no sistema de arquivos **LittleFS**.

O ESP32 se conecta a uma rede Wi-Fi e disponibiliza uma página web que permite interagir com o dispositivo através de uma **API HTTP**.

Atualmente, o projeto permite controlar o LED integrado da placa ESP32 diretamente pelo navegador.

---

## 🚀 Funcionalidades

* Conexão do ESP32 à rede Wi-Fi
* Servidor HTTP executando diretamente no ESP32
* Página web armazenada no LittleFS
* HTML, CSS e JavaScript separados
* Comunicação entre JavaScript e ESP32 através de API
* API utilizando requisições HTTP `GET` e `POST`
* Controle do LED integrado da placa pelo navegador
* Consulta do estado atual do LED
* Comunicação utilizando JSON
* Estrutura preparada para futuras funcionalidades IoT

---

## 🛠️ Tecnologias utilizadas

* **ESP32**
* **C++ / Arduino Framework**
* **PlatformIO**
* **VS Code**
* **Wi-Fi**
* **WebServer**
* **LittleFS**
* **ArduinoJson**
* **HTML5**
* **CSS3**
* **JavaScript**
* **HTTP / JSON**

---

## 📁 Estrutura do projeto

```text
ESP32WebServer/
│
├── platformio.ini
│
├── src/
│   └── main.cpp
│
└── data/
    ├── index.html
    ├── style.css
    └── script.js
```

### `src/main.cpp`

Contém o código principal do ESP32, incluindo:

* Inicialização do Wi-Fi
* Inicialização do LittleFS
* Configuração do servidor HTTP
* Definição das rotas
* Processamento das requisições da API
* Controle do LED

### `data/index.html`

Interface principal da aplicação web.

### `data/style.css`

Estilos da interface.

### `data/script.js`

Responsável pela interação da página com a API do ESP32.

---

# 🌐 Funcionamento

O ESP32 se conecta à rede Wi-Fi configurada no código.

Após a conexão, o monitor serial apresenta o endereço IP atribuído ao dispositivo:

```text
Wi-Fi conectado!

IP do ESP32: 192.168.x.x

Servidor HTTP iniciado!
```

O endereço IP pode então ser acessado pelo navegador:

```text
http://192.168.x.x
```

A página é carregada diretamente do **LittleFS**.

---

# 🔌 API

O projeto possui endpoints HTTP para comunicação entre o navegador e o ESP32.

## GET `/api/status`

Consulta o estado atual do ESP32 e do LED.

### Requisição

```http
GET /api/status
```

### Resposta

```json
{
  "status": "online",
  "led": "ligado"
}
```

ou:

```json
{
  "status": "online",
  "led": "desligado"
}
```

---

## POST `/api/ledState`

Altera o estado do LED.

### Requisição

```http
POST /api/ledState
Content-Type: application/json
```

### Body

Para ligar:

```json
{
  "state": true
}
```

Para desligar:

```json
{
  "state": false
}
```

### Resposta

```json
{
  "status": "ok",
  "led": "ligado"
}
```

ou:

```json
{
  "status": "ok",
  "led": "desligado"
}
```

---

# 🔄 Fluxo da aplicação

A comunicação acontece da seguinte forma:

```text
┌──────────────────────┐
│      Navegador       │
│                      │
│ HTML + CSS + JS      │
└──────────┬───────────┘
           │
           │ HTTP
           ▼
┌──────────────────────┐
│        ESP32         │
│                      │
│    WebServer         │
│                      │
│       API            │
└──────────┬───────────┘
           │
           │ GPIO
           ▼
      ┌─────────┐
      │   LED   │
      └─────────┘
```

Quando o usuário pressiona o botão:

```text
JavaScript
    │
    │ POST /api/ledState
    │ {"state":true}
    ▼
ESP32
    │
    ├── recebe JSON
    │
    ├── interpreta com ArduinoJson
    │
    ├── atualiza ledState
    │
    └── digitalWrite()
         │
         ▼
       LED
```

---

# 💻 Exemplo de comunicação JavaScript

O JavaScript utiliza `fetch()` para enviar comandos ao ESP32.

```javascript
const response = await fetch("/api/ledState", {
    method: "POST",
    headers: {
        "Content-Type": "application/json"
    },
    body: JSON.stringify({
        state: true
    })
});

const data = await response.json();

console.log(data);
```

Para consultar o estado:

```javascript
const response = await fetch("/api/status");

const data = await response.json();

console.log(data);
```

---

# 📦 Dependências

O projeto utiliza a biblioteca:

**ArduinoJson**

No `platformio.ini`:

```ini
lib_deps =
    bblanchon/ArduinoJson
```

Exemplo completo:

```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino

monitor_speed = 115200

board_build.filesystem = littlefs

lib_deps =
    bblanchon/ArduinoJson
```

---

# ⚙️ Como executar

## 1. Pré-requisitos

Instale:

* VS Code
* PlatformIO
* Driver USB da placa ESP32, se necessário

Conecte o ESP32 ao computador através do cabo USB.

---

## 2. Configurar o Wi-Fi

No `main.cpp`, configure:

```cpp
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";
```

Não publique suas credenciais reais no GitHub.

Uma alternativa é criar um arquivo de configuração local que fique fora do controle de versão.

---

## 3. Compilar o projeto

No PlatformIO:

```text
Build
```

ou através do terminal:

```bash
pio run
```

---

## 4. Enviar o programa para o ESP32

```bash
pio run -t upload
```

ou utilize o botão **Upload** do PlatformIO.

---

## 5. Enviar os arquivos do site

Os arquivos:

```text
data/index.html
data/style.css
data/script.js
```

são gravados no LittleFS.

Execute:

```bash
pio run -t buildfs
pio run -t uploadfs
```

No PlatformIO também é possível utilizar:

```text
Upload Filesystem Image
```

Dependendo da placa/configuração, pode ser necessário pressionar o botão **BOOT** durante o início do processo de gravação do filesystem.

---

## 6. Abrir o Monitor Serial

Utilize:

```bash
pio device monitor
```

ou configure o monitor do PlatformIO para:

```text
115200 baud
```

O ESP32 deverá mostrar o endereço IP:

```text
Wi-Fi conectado!

IP do ESP32: 192.168.1.24

Servidor HTTP iniciado!
```

Digite esse endereço no navegador:

```text
http://192.168.1.24
```

O IP pode ser diferente na sua rede.

---

# 📂 LittleFS

O LittleFS é utilizado para armazenar os arquivos da aplicação web.

```text
data/
├── index.html
├── style.css
└── script.js
```

Esses arquivos não ficam compilados diretamente dentro do `main.cpp`.

Isso permite desenvolver o frontend de maneira independente do código C++ do ESP32.

---

# 🔐 Segurança

Este projeto foi desenvolvido inicialmente para uso em uma **rede local**.

Não é recomendado expor diretamente o servidor do ESP32 à Internet sem implementar mecanismos adicionais de segurança.

Principalmente:

* Autenticação
* Autorização
* HTTPS/TLS
* Validação de entrada
* Controle de acesso
* Proteção contra requisições maliciosas

Também não publique no GitHub:

```text
SSID
Senha do Wi-Fi
Tokens
Chaves de API
Credenciais
```

---

# 🚧 Próximos passos

O projeto pode evoluir para uma aplicação IoT mais completa.

Algumas possibilidades:

* [ ] Controle de outros GPIOs
* [ ] Controle de relés
* [ ] Leitura de sensores
* [ ] API para sensores
* [ ] Dashboard em tempo real
* [ ] Atualização automática dos dados
* [ ] WebSocket
* [ ] Controle de luminosidade
* [ ] Controle de temperatura
* [ ] Configuração do ESP32 pela interface web
* [ ] Persistência de configurações
* [ ] Autenticação na API
* [ ] OTA (Over-The-Air)
* [ ] Histórico de dados
* [ ] Integração com banco de dados
* [ ] Integração com MQTT

---

# 🎯 Objetivo do projeto

O objetivo é estudar e demonstrar como utilizar um **ESP32 como servidor web**, permitindo que uma aplicação frontend desenvolvida com tecnologias web se comunique diretamente com o hardware através de uma API HTTP.

A arquitetura utilizada separa claramente:

```text
Frontend
HTML
CSS
JavaScript
     │
     │ HTTP / JSON
     ▼
Backend embarcado
C++ / ESP32
     │
     ▼
Hardware
GPIO / Sensores / Atuadores
```

Esse modelo permite utilizar conceitos comuns do desenvolvimento web, como **API REST, HTTP, JSON e JavaScript `fetch()`**, diretamente em um dispositivo embarcado.

---

## 📄 Licença

Este projeto pode ser utilizado para fins de estudo e desenvolvimento.

Adicione aqui a licença escolhida para o projeto, por exemplo:

```text
MIT License
```
