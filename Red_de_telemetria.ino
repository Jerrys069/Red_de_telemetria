//Proyecto estadía
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SGP30.h>

// Definir credenciales WiFi
const char* ssid = "UTEZ";
const char* password = "";

// Definir credenciales de ThingsBoard
const char* thingsboardServer = "thingsboard.utez.edu.mx";
  const char* accessToken = "Knid6DSDUtISvhASAWj6";

// Pines y variables para el sensor MQ135
#define MQ135_PIN A0
float mq135Value = 0;

Adafruit_SGP30 sgp;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(115200);

  if (!sgp.begin()) {
    Serial.println("Sensor SGP30 no encontrado :(");
    while (1);
  }

  // El sensor necesita 15 segundos de calentamiento antes de dar lecturas correctas
  delay(500);

  connectWiFi();
  client.setServer(thingsboardServer, 1883);
  connectThingsboard();
}

void loop() {
  if (!client.connected()) {
    connectThingsboard();
  }
  client.loop();

  if (!sgp.IAQmeasure()) {
    Serial.println("Error leyendo el sensor SGP30");
    return;
  }

  if (!sgp.IAQmeasureRaw()) {
    Serial.println("Error leyendo datos crudos del sensor SGP30");
    return;
  }

  float CO2 = sgp.eCO2;
  float TVOC = sgp.TVOC;

  // Leer el valor del sensor MQ135
  mq135Value = analogRead(MQ135_PIN);
 
  String payload = "{";
  payload += "\"eCO2 Dispositivo 2\":"; payload += CO2+230; payload += ",";
  payload += "\"TVOC Dispositivo 2\":"; payload += TVOC; payload += ",";
  payload += "\"MQ135 Dispositivo 2\":"; payload += mq135Value+250;
  payload += "}";

  Serial.print("Sending payload: ");
  Serial.println(payload);

  client.publish("v1/devices/me/telemetry", (char*) payload.c_str());

  delay(2000); // Publicar datos cada 60 segundos
}

void connectWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectThingsboard() {
  while (!client.connected()) {
    Serial.print("Connecting to Thingsboard...");
    if (client.connect("ESP8266", accessToken, NULL)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
