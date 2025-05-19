//Calibración de sensor MQ135
//--------------------------------------
#define MQ135_PIN A0         // Pin analógico donde se conecta el MQ135
#define RL_VALUE 10.0        // Resistencia de carga en kΩ (según tu módulo)
#define RO_CLEAN_AIR_FACTOR 3.6 // Relación RS/R0 en aire limpio (específica del MQ135)

void setup() {
  Serial.begin(115200);
  delay(2000); // Esperar que el sensor se estabilice (ideal: varias horas antes de calibrar)
  Serial.println("Calibrando MQ135...");

  float Ro = calibrateSensor();
  Serial.print("Valor estimado de R0: ");
  Serial.println(Ro);
}

void loop() {
  // No se hace nada en loop, solo calibración en setup
}

float calibrateSensor() {
  int samples = 50;
  float rs = 0;

  for (int i = 0; i < samples; i++) {
    int adc = analogRead(MQ135_PIN);
    float voltage = adc / 1023.0 * 3.3;
    float resistance = (3.3 - voltage) * RL_VALUE / voltage;
    rs += resistance;
    delay(100);
  }

  rs = rs / samples;
  float Ro = rs / RO_CLEAN_AIR_FACTOR; // Cálculo de R0 en aire limpio
  return Ro;
}
