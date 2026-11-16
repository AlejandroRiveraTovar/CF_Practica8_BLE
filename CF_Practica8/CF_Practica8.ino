// Importa la librerías necesarias para trabajar con BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


const int ANALOG_IN_PIN = 13; // Selecciona el pin para la lectura analógica
const int SPEED_TRANSMISSION = 9600; // selecciona la velocidad de transmisión


const String DEVICE_NAME = String("CF-RiveraTovar"); // Define el nombre del dispositivo
// Usar https://www.uuidgenerator.net/ para generarlos
const String SERVICE_UUID = String("6e3cfbdc-d9c8-4e49-a7af-6d6cf512febe"); // Define el identificador de servicio
const String CHARACTERISTIC_UUID = String("6ed2fd5f-02f1-46a6-a0f8-b9ebc595c0f0"); // Define el identificador de característica

int value; // variable para almacenar el valor leído

BLECharacteristic *pCharacteristic; // Puntero para la característica a reportar


void setup() {

  // inicializa la comunicación serial al PC para ver los datos en el terminal
  Serial.begin(SPEED_TRANSMISSION);

  // Inicia el dispositivo indicando un nombre
  BLEDevice::init(DEVICE_NAME.c_str());
  // Crea un servidor BLE
  BLEServer *pServer = BLEDevice::createServer();
  // Crea y configura el servicio
  BLEService *pService = pServer->createService(SERVICE_UUID.c_str());
  // Crea y configura la característica
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID.c_str(),
                      BLECharacteristic::PROPERTY_READ
                    );

  // Inicia el servicio
  pService->start();
  // Configura e inicia la difusión del servicio
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID.c_str());
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void loop() {
  // Lee el valor del pin analógico
  value = analogRead(ANALOG_IN_PIN);
  // Envía el valor por el puerto serial
  Serial.println(value);
  // Modifica el valor de la característica
  String characteristicValue = String(value);
  pCharacteristic->setValue(characteristicValue.c_str());
  delay(500);

}
