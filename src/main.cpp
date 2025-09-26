#include <WiFi.h>

// --- Configuración de Red ---
const char* ssid = "Telecentro-da0d";    // CAMBIAR por tu red
const char* password = "H4JTRX4NGL9L";   // CAMBIAR por tu clave

// --- Configuración del Servidor y Hardware ---
WiFiServer server(80); // Puerto 80
const int ledrojo   = 2;
const int ledamarillo = 4;
const int ledverde  = 14;
String modo = "Ninguno";

// --- Página Web ---
const char pagina_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Semáforo ESP32</title>
  <style>
    body {
      background-color: white;
      text-align: center;
      font-family: sans-serif;
    }
    .titulo {
      font-family: Impact;
      font-size: 45px;
      font-weight: bold;
      text-decoration: underline;
    }
    button {
      width: 200px;
      height: 200px;
      font-size: 23px;
      margin: 10px;
      border: 3px solid black;
      border-radius: 100px;
      cursor: pointer;
      color: black;
    }
    .lento { background-color: red; }
    .normal { background-color: yellow; }
    .rapido { background-color: green; }
    .emergencia { background-color: orange; }
  </style>
</head>
<body>
  <div class="titulo">
    <h1>SEMÁFORO</h1>
    <p>Seleccione el modo del semáforo:</p>
  </div>
  <p><a href="/lento"><button class="lento">Lento</button></a></p>
  <p><a href="/normal"><button class="normal">Normal</button></a></p>
  <p><a href="/rapido"><button class="rapido">Rápido</button></a></p>
  <p><a href="/emergencia"><button class="emergencia">Emergencia</button></a></p>
  <p><span>Modo:modo actual:xxx</span></p>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(ledrojo, OUTPUT);
  pinMode(ledamarillo, OUTPUT);
  pinMode(ledverde, OUTPUT);
  digitalWrite(ledrojo, LOW);
  digitalWrite(ledamarillo, LOW);
  digitalWrite(ledverde, LOW);

  // --- Conexión a la red Wi-Fi ---
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado a Wi-Fi");
  Serial.print("IP del servidor: http://");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("\n[Nuevo Cliente]");
  String header = "";
  String currentLine = "";

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      header += c;

      if (c == '\n') {
        if (currentLine.length() == 0) {
          // --- Acciones según la URL ---
          if (header.indexOf("GET /lento") >= 0) {
            modo = "Lento";
            Serial.println("Modo lento activado");
            digitalWrite(ledverde, HIGH);
            delay(7000);
            digitalWrite(ledverde, LOW);
            digitalWrite(ledamarillo, HIGH);
            delay(3000);
            digitalWrite(ledamarillo, LOW);
            digitalWrite(ledrojo, HIGH);
             delay(5000);
            digitalWrite(ledrojo, LOW);
          } 
          else if (header.indexOf("GET /normal") >= 0) {
            modo = "Normal";
            Serial.println("Modo normal activado");
            digitalWrite(ledverde, HIGH);
            delay(4500);
            digitalWrite(ledverde, LOW);
            digitalWrite(ledamarillo, HIGH);
             delay(2000);
            digitalWrite(ledamarillo, LOW);
            digitalWrite(ledrojo, HIGH);
             delay(6500);
            digitalWrite(ledrojo, LOW);
          } 
          else if (header.indexOf("GET /rapido") >= 0) {
            modo = "Rápido";
            Serial.println("Modo rápido activado");
            digitalWrite(ledverde, HIGH);
            delay(2000);
            digitalWrite(ledverde, LOW);
            digitalWrite(ledamarillo, HIGH);
            delay(1000);
            digitalWrite(ledamarillo, LOW);
            digitalWrite(ledrojo, HIGH);
            delay(4000);
            digitalWrite(ledrojo, LOW);
          } 
          else if (header.indexOf("GET /emergencia") >= 0) {
            modo = "Emergencia";
            Serial.println("Modo emergencia activado");
            for(int i=0 ; i<6 ; i++){
            digitalWrite(ledrojo, HIGH);
            digitalWrite(ledamarillo, HIGH);
            digitalWrite(ledverde, HIGH);
                 delay(1000);
            digitalWrite(ledrojo, LOW);
            digitalWrite(ledamarillo, LOW);
            digitalWrite(ledverde, LOW);
          }
        }
          // --- Construir página reemplazando XXX ---
          String pagina = String(pagina_html);
          pagina.replace("XXX", "<b>" + modo + "</b>");

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          client.print(pagina);
          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }

  client.stop();
  Serial.println("[Cliente Desconectado]");
}