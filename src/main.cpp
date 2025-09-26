#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

WebServer server(80);
const int rojo= 2;
const int amarillo= 4;
const int verde= 5;
String modo = "Ninguno";

// Funciones para los modos
void lento() {
  modo = "Lento";
  digitalWrite(rojo, HIGH); delay(7000);
  digitalWrite(rojo, LOW);
  digitalWrite(amarillo, HIGH); delay(3000);
  digitalWrite(amarillo, LOW);
  digitalWrite(verde, HIGH); delay(5000);
  digitalWrite(verde, LOW);
  server.sendHeader("Location", "/");
  server.send(200);
}

void normal() {
  modo = "Normal";
  digitalWrite(rojo, HIGH); delay(4500);
  digitalWrite(rojo, LOW);
  digitalWrite(amarillo, HIGH); delay(2000);
  digitalWrite(amarillo, LOW);
  digitalWrite(verde, HIGH); delay(6500);
  digitalWrite(verde, LOW);
  server.sendHeader("Location", "/");
  server.send(200);
}

void rapido() {
  modo = "Rápido";
  digitalWrite(rojo, HIGH); delay(2000);
  digitalWrite(rojo, LOW);
  digitalWrite(amarillo, HIGH); delay(1000);
  digitalWrite(amarillo, LOW);
  digitalWrite(verde, HIGH); delay(4000);
  digitalWrite(verde, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void emergencia() {
  modo = "Emergencia";
  for(int i=0;i<5;i++){
    digitalWrite(rojo, HIGH);
    digitalWrite(amarillo, HIGH);
    delay(1000);
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, LOW);
    delay(1000);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  // Página principal
  server.on("/", []() {
    String pagina = R"rawliteral(
      <html>
      <body>
      <head>
    <meta charset="utf-8">
    <title>SEMÁFORO</title>
    <style>
        body {
            display: flex;
            flex-direction: column; 
            align-items: center;      
        }
        .titulo{
            font-family: Impact;
            font-size: 45px;
        }
        .boton{
            width: 200px; 
            height: 200px;
            border: 3px solid black;
            border-radius: 100px;
            font-size: 23px;
            justify-content: center;
            align-items: center;

        }
        .lento{
             background-color: #ff0000;
        }
        .normal{
             background-color: #ffff00;
        }
        .rapido{
             background-color: #00ff00;
        }
        .emergencia{
             background-color: #ff8000;
        }
    </style>
</head>
        <h1>SEMÁFORO</h1>
        <a href="/lento"><button>Lento</button></a>
        <a href="/normal"><button>Normal</button></a>
        <a href="/rapido"><button>Rápido</button></a>
        <a href="/emergencia"><button>Emergencia</button></a>
        <p>Modo actual: XXX</p>
      </body>
      </html>
    )rawliteral";

    pagina.replace("XXX", modo);
    server.send(200, "text/html", pagina);
  });

  // Rutas de control
  server.on("/lento", lento);
  server.on("/normal", normal);
  server.on("/rapido", rapido);
  server.on("/emergencia", emergencia);

  server.begin();
}

void loop() {
  server.handleClient();
}
