#include <UbiConstants.h>
#include <UbiTypes.h>
#include <UbidotsEsp32Mqtt.h>
#include <Arduino.h>
#include <DHT.h>

// Configuración de los sensores de humedad y temperatura del aire (DHT)
#define DHTPIN1  14   // Pin de datos del sensor DHT 1
#define DHTPIN2  25   // Pin de datos del sensor DHT 2
#define DHTPIN3  26   // Pin de datos del sensor DHT 3
#define DHTPIN4  27   // Pin de datos del sensor DHT 4
#define DHTTYPE DHT11   // Tipo de sensor DHT (DHT11 o DHT22)
const int bomba1 = 13; // Pin de la bomba de agua 1

// Inicialización de los sensores DHT
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);

// Pin analógico para el sensor de humedad del suelo
const int sensorHumedadSueloPin35 = 35; // Pin para el sensor de humedad del suelo
const int sensorHumedadSueloPin32 = 32; // Pin para el sensor de humedad del suelo
const int sensorHumedadSueloPin34 = 34; // Pin para el sensor de humedad del suelo
const int sensorHumedadSueloPin33 = 33; // Pin para el sensor de humedad del suelo

// Credenciales de WiFi
const char *WIFI_SSID = "HpLaser Lab-Multimedia";
const char *WIFI_PASSWORD = "F3br3ro.2024";
const char *UBIDOTS_TOKEN = "BBUS-iFxynnSMNGGebSMYrdXEm2ihqGXycc"; // Token de Ubidots

Ubidots ubidots(UBIDOTS_TOKEN);

void setup() {
  Serial.begin(115200); // Cambié la velocidad de baudios a 115200 para mayor compatibilidad con ESP32
  Serial.println();
  Serial.println("Device initialization...");

  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();

  pinMode(bomba1, OUTPUT); // Configuramos el pin de la bomba1 como salida
  digitalWrite(bomba1, LOW); // Aseguramos que la bomba1 esté inicialmente apagada
  delay(2000);  // Esperamos 2 segundos para que los sensores DHT se inicialicen correctamente

  // Conexión a WiFi
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASSWORD);

  // Configuración de Ubidots
  ubidots.setDebug(true); // Activa los mensajes de depuración
  ubidots.setup();
  ubidots.reconnect();
  
  // Suscripción a los valores en Ubidots
  ubidots.subscribeLastValue("esp32", "humedad_suelo_1");
  ubidots.subscribeLastValue("esp32", "humedad_suelo_2");
  ubidots.subscribeLastValue("esp32", "humedad_suelo_3");
  ubidots.subscribeLastValue("esp32", "humedad_suelo_4");
  ubidots.subscribeLastValue("esp32", "temperatura_1");
  ubidots.subscribeLastValue("esp32", "humedad_aire_1");
  ubidots.subscribeLastValue("esp32", "temperatura_2");
  ubidots.subscribeLastValue("esp32", "humedad_aire_2");
  ubidots.subscribeLastValue("esp32", "temperatura_3");
  ubidots.subscribeLastValue("esp32", "humedad_aire_3");
  ubidots.subscribeLastValue("esp32", "temperatura_4");
  ubidots.subscribeLastValue("esp32", "humedad_aire_4");
}

void loop() {
  if (!ubidots.connected()) {
    ubidots.reconnect();
    ubidots.subscribeLastValue("esp32", "humedad_suelo_1");
    ubidots.subscribeLastValue("esp32", "humedad_suelo_2");
    ubidots.subscribeLastValue("esp32", "humedad_suelo_3");
    ubidots.subscribeLastValue("esp32", "humedad_suelo_4");
    ubidots.subscribeLastValue("esp32", "temperatura_1");
    ubidots.subscribeLastValue("esp32", "humedad_aire_1");
    ubidots.subscribeLastValue("esp32", "temperatura_2");
    ubidots.subscribeLastValue("esp32", "humedad_aire_2");
    ubidots.subscribeLastValue("esp32", "temperatura_3");
    ubidots.subscribeLastValue("esp32", "humedad_aire_3");
    ubidots.subscribeLastValue("esp32", "temperatura_4");
    ubidots.subscribeLastValue("esp32", "humedad_aire_4");
    return;
  }

  ubidots.loop();

  // Lectura del sensor de humedad del suelo (analogRead)
  int humedadSuelo4 = analogRead(sensorHumedadSueloPin35);
  int humedadS4 = 100 - ((humedadSuelo4 * 100) / 4095); // ESP32 tiene resolución ADC de 12 bits (0-4095)
  int humedadSuelo1 = analogRead(sensorHumedadSueloPin32);
  int humedadS1 = 100 - ((humedadSuelo1 * 100) / 4095); // ESP32 tiene resolución ADC de 12 bits (0-4095)
  int humedadSuelo3 = analogRead(sensorHumedadSueloPin34);
  int humedadS3 = 100 - ((humedadSuelo3 * 100) / 4095); // ESP32 tiene resolución ADC de 12 bits (0-4095)
  int humedadSuelo2 = analogRead(sensorHumedadSueloPin33);
  int humedadS2 = 100 - ((humedadSuelo2 * 100) / 4095); // ESP32 tiene resolución ADC de 12 bits (0-4095)

  Serial.print("Humedad del suelo 4: ");
  Serial.println(humedadS4);
  Serial.print("Humedad del suelo 1: ");
  Serial.println(humedadS1);
  Serial.print("Humedad del suelo 3: ");
  Serial.println(humedadS3);
  Serial.print("Humedad del suelo 2: ");
  Serial.println(humedadS2);

  // Lectura de los sensores DHT
  leerYEnviarDatos(dht1, "temperatura_1", "humedad_aire_1");
  leerYEnviarDatos(dht2, "temperatura_2", "humedad_aire_2");
  leerYEnviarDatos(dht3, "temperatura_3", "humedad_aire_3");
  leerYEnviarDatos(dht4, "temperatura_4", "humedad_aire_4");

  // Envío de datos de humedad del suelo a Ubidots
  ubidots.add("humedad_suelo_4", humedadS4);
  ubidots.add("humedad_suelo_1", humedadS1);
  ubidots.add("humedad_suelo_3", humedadS3);
  ubidots.add("humedad_suelo_2", humedadS2);
  ubidots.publish("esp32");

  // Lógica para activar la bomba si la humedad del suelo es menor a 45%
  if (humedadS4 < 65) {
    Serial.println("Humedad del suelo baja, activando bomba...");
    digitalWrite(bomba1, HIGH); // Activar bomba1
  } else {
    Serial.println("Humedad del suelo adecuada, desactivando bomba...");
    digitalWrite(bomba1, LOW); // Desactivar bomba1
  }

  delay(2000); // Esperamos 2 segundos entre cada lectura
}

void leerYEnviarDatos(DHT &dht, const char* tempVarName, const char* humVarName) {
  Serial.println("Leyendo el sensor DHT...");
  float humedadAire = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Verificamos si la lectura fue exitosa
  if (isnan(humedadAire) || isnan(temperatura)) {
    Serial.println("Error al leer el sensor DHT");
  } else {
    Serial.print("Humedad del aire: ");
    Serial.print(humedadAire);
    Serial.print("%\t");
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println("°C");

    // Envío de datos a Ubidots
    ubidots.add(tempVarName, temperatura);
    ubidots.add(humVarName, humedadAire);
    ubidots.publish("esp32");
  }
}