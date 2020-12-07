#include "Wire.h"
#include "DHT.h"
#include "BME280.h"

#ifndef sensor_h
#define sensor_h

enum SensorType {
  UNDEFINED,
  DHT_11,
  DHT_22,
  BME_280,
};

class Sensor {
  public:
    Sensor();
    Sensor(uint8_t dhtPin, uint8_t dhtType);
    Sensor(uint8_t address);
    void begin();
    void readData();
    float getTemperature();
    float getHumidity();
    float getPressure();
    String getData(String sensorSymbol);
    bool isInitialized();

  private:
    DHT *_dht = NULL;
    SensorType _type;
    BME280 *_bme = NULL;
};

Sensor::Sensor() {
  _type = UNDEFINED;
}

Sensor::Sensor(uint8_t dhtPin, uint8_t dhtType) {
  _type = dhtType == DHT11 ? DHT_11 : DHT_22;
  _dht = new DHT(dhtPin, dhtType);
}

Sensor::Sensor(uint8_t address) {
  _type = BME_280;
  _bme = new BME280(Wire, address);
}

void Sensor::begin() {
  switch (_type) {
    case DHT_11:
    case DHT_22:
      _dht->begin();
      break;
    case BME_280:
      _bme->begin();
      break;
  }
}

void Sensor::readData() {
  if (_type == BME_280) {
    _bme->readSensor();
  }
}

float Sensor::getTemperature() {
  switch (_type) {
    case DHT_11:
    case DHT_22:
      return _dht->readTemperature();
    case BME_280:
      return _bme->getTemperature_C();
    default:
      return NULL;
  }
}

float Sensor::getHumidity() {
  switch (_type) {
    case DHT_11:
    case DHT_22:
      return _dht->readHumidity();
    case BME_280:
      return _bme->getHumidity_RH();
    default:
      return NULL;
  }
}

float Sensor::getPressure() {
  switch (_type) {
    case BME_280:
      return _bme->getPressure_Pa() / 100;
    default:
      return NULL;
  }
}

bool Sensor::isInitialized() {
  return _type != UNDEFINED;
}

String Sensor::getData(String sensorSymbol) {

  float temp = getTemperature();
  float hum = getHumidity();

  switch (_type) {
    case DHT_11:
    case DHT_22:
      return sensorSymbol + "|" + (String)temp + "|" + (String)hum + "|null";
    case BME_280: {
        float pressure = getPressure();
        return sensorSymbol + "|" + (String)temp + "|" + (String)hum + "|" + (String)pressure;
      }
    default:
      return "";
  }
}

#endif
