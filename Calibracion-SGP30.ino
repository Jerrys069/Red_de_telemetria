//Calibración sensor SGP-30
//-------------------------------
#include <Wire.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Inicializando SGP30...");

  if (!sgp.begin()) {
    Serial.println("Error al detectar el sensor SGP30. Verifica conexiones.");
    while (1);
  }

  Serial.print("SGP30 serial #:");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  // Espera inicial según la hoja de datos
  sgp.IAQinit();
}

void loop() {
  if (!sgp.IAQmeasure()) {
    Serial.println("Error al leer valores del SGP30");
    return;
  }

  Serial.print("eCO2: ");
  Serial.print(sgp.eCO2);
  Serial.print(" ppm\tTVOC: ");
  Serial.print(sgp.TVOC);
  Serial.println(" ppb");

  delay(1000);
}
