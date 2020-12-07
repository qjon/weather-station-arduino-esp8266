#ifndef sensor_data_store_h
#define sensor_data_store_h

#include "configuration_prod.h"
#include "sensor_data_list.h"

class SensorDataStore {
  public:
    SensorDataStore();
    SensorDataList& get(String deviceId);
    String log();
    void print();
  private:
    void add(SensorDataList &data);
    int nextListIndex = 0;
    SensorDataList list[MAX_NUMBER_OF_DEVICES];
};

SensorDataStore::SensorDataStore() {}

SensorDataList& SensorDataStore::get(String deviceId) {
  SensorDataList sensorDataList(deviceId);
  bool isFound = false;
  int foundedIndex = -1;

  for (int i = 0; i < nextListIndex; i++) {
    if (!isFound && list[i].getDeviceId() == deviceId) {
      isFound = true;
      foundedIndex = i;
    }
  }

  //  sensorDataList.print();

  if (!isFound) {
    Serial.println("NOT FOUND LIST - CREATION");
    add(sensorDataList);

    return list[nextListIndex - 1];
  } else {
    return list[foundedIndex];
  }
}

void SensorDataStore::add(SensorDataList &data) {
  if (nextListIndex < MAX_NUMBER_OF_DEVICES) {

    list[nextListIndex] = data;

    nextListIndex++;

    Serial.println("Create new list: " + data.getDeviceId());
  } else {
    Serial.println("Max number of devices");
  }
}

void SensorDataStore::print() {
  for (int i = 0; i < nextListIndex; i++) {
    list[i].print();
  }
}

String SensorDataStore::log() {
  String message = "";

  for (int i = 0; i < nextListIndex; i++) {
    SensorDataList sdl = list[i];

    if (sdl.getDeviceId() != "") {
      message += sdl.getDeviceId();
      message += "\n";
      message += sdl.getRawData();
      message += "\n";
      message += "\n";
    }
  }

  return message;
}

#endif
