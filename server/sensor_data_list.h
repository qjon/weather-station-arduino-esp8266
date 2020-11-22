#ifndef sensor_data_list_h
#define sensor_data_list_h

#include "configuration_prod.h"
#include "sensor_data.h";
class SensorDataList {
  public:
    SensorDataList();
    SensorDataList(String deviceId);
    void add(SensorData data);
    String getDeviceId();
    String getRawData();
    void print();
  private:
    String _deviceId = "";
    int nextRowIndex = 0;
    SensorData list[SENSOR_DATA_LIST_LENGTH];
};

SensorDataList::SensorDataList() {

}

SensorDataList::SensorDataList(String deviceId) {
  _deviceId = deviceId;
}

void SensorDataList::add(SensorData data) {

  list[nextRowIndex] = data;

  // todo: skip not sync data before override it
  nextRowIndex++;
  nextRowIndex = nextRowIndex % SENSOR_DATA_LIST_LENGTH;
  Serial.print("Next row inddex: ");
  Serial.println(nextRowIndex);
}

String SensorDataList::getDeviceId() {
  return _deviceId;
}

void SensorDataList::print() {
  Serial.print("Device list: ");
  Serial.println(getDeviceId());

  //  Serial.print("Index: ");
  //  Serial.println(nextRowIndex);

  Serial.println("");
  Serial.println("Data:");

  for (int i = 0; i < SENSOR_DATA_LIST_LENGTH; i++) {
    if (list[i].isValid()) {

      list[i].print();
    }
  }
}

String SensorDataList::getRawData() {
  String rawData = "";

  for (int i = 0; i < SENSOR_DATA_LIST_LENGTH; i++) {
    if (list[i].isValid()) {
      rawData += list[i].getRawData();
      rawData += "\n";
    }
  }

  return rawData;
}

#endif;
