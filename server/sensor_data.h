#ifndef sensor_data_h
#define sensor_data_h

class SensorData {
  public:
    SensorData();
    SensorData(String symbol, int timestamp, float temp, float hum);
    SensorData(String symbol, int timestamp, float temp, float hum, float pressure);
    float getDewPoint();
    float getHumidity();
    String getSymbol();
    float getTemperature();
    float getPressure();
    int getTimestamp();
    String getRawData();
    bool isSync();
    bool isValid();
    void print();
    void setSync(bool sync);
  private:
    String _symbol; 
    float calculateDewPoint(float temp, float hum);
    float _dewPoint;
    float _humidity;
    float _pressure = NULL;
    float _temperature;
    int _timestamp;
    bool _synchronized = false;
    bool _valid = false;
};

SensorData::SensorData() {

}

SensorData::SensorData(String symbol, int timestamp, float temp, float hum) {
  _symbol = symbol;
  _timestamp = timestamp;
  _temperature = temp;
  _humidity = hum;
  _dewPoint = calculateDewPoint(temp, hum);
  _valid = true;
}

SensorData::SensorData(String symbol, int timestamp, float temp, float hum, float pressure) {
  _symbol = symbol;
  _timestamp = timestamp;
  _temperature = temp;
  _humidity = hum;
  _pressure = pressure;
  _dewPoint = calculateDewPoint(temp, hum);
  _valid = true;
}

float SensorData::calculateDewPoint(float temp, float hum) {
  float value = sqrt(sqrt(sqrt(hum / 100)));
  return (value * (112 + 0.9 * temp) + (0.1 * temp) - 112);
}

float SensorData::getDewPoint() {
  return _dewPoint;
}

float SensorData::getHumidity() {
  return _humidity;
}

float SensorData::getPressure() {
  return _pressure;
}

String SensorData::getSymbol() {
  return _symbol;
}

float SensorData::getTemperature() {
  return _temperature;
}

int SensorData::getTimestamp() {
  return _timestamp;
}

bool SensorData::isSync() {
  return _synchronized;
}

bool SensorData::isValid() {
  return _valid;
}

void SensorData::print() {
  Serial.print("Symbol:");
  Serial.print(getSymbol());
  Serial.print("\t");
  Serial.print("Time:");
  Serial.print(getTimestamp());
  Serial.print("\t");
  Serial.print("Temp:");
  Serial.print(getTemperature());
  Serial.print("\t\t");
  Serial.print("Hum:");
  Serial.print(getHumidity());
  Serial.print("\t");
  Serial.print("DewPoint:");
  Serial.println(getDewPoint());
  Serial.print("Pressure:");
  Serial.println(getPressure());
}

void SensorData::setSync(bool sync) {
  _synchronized = sync;
}

String SensorData::getRawData() {
  String rawData = "";
  
  rawData += getSymbol();
  rawData += "|";
  rawData += getTimestamp();
  rawData += "|";
  rawData += getTemperature();
  rawData += "|";
  rawData += getHumidity();
  rawData += "|";
  rawData += getDewPoint();
  rawData += "|";
  rawData += getPressure();

  return rawData;
}


#endif
