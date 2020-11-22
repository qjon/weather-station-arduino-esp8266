#include <LowPower.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>

#include "rf.h"
#include "rf_encoder.h"
#include "sensor.h"

RfConnection rfConnection;

Sensor sensorOne(0x76);
Sensor sensorTwo;             // if sensor two is not available change this line to: Senosr sensorTwo

int wakeUpIntervalNumber = round(SLEEP_INTERVAL_SEC / 8);
int currentIntervalNumber = 0;

bool makeReading = false;

void setup() {
  Serial.begin(57600);

  setupPin();

  sensorOne.begin();
  sensorTwo.begin();

  rfConnection.begin();
}

void loop() {
  afterWakeUp();

  if (makeReading) {
    if (sensorOne.isInitialized()) {
      sensorOne.readData();
      rfConnection.sendSensorData(DEVICE_UNIQUE_ID, sensorOne.getData("0"));
    }

    if (sensorTwo.isInitialized()) {
      sensorTwo.readData();
      rfConnection.sendSensorData(DEVICE_UNIQUE_ID, sensorTwo.getData("1"));
    }
  }


  beforeSleep();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}


// deviceID, recordID, Timestamp
void convertMsg2Array(String data, String* arr) {

  const char delimiter = '|';
  int index = 0;
  int length = data.length();

  String buffer;

  for (int i = 0; i < length; i++) {
    if (data[i] != delimiter) {
      buffer += data[i];
    } else {
      arr[index++] = buffer;
      buffer = "";
    }
  }

  if (buffer != "") {
    arr[index++] = buffer;
  }
}


bool isCurrentDeviceMessage(String msgDeviceId) {
  return msgDeviceId == DEVICE_UNIQUE_ID;
}


void beforeSleep() {
  makeReading = false;
  currentIntervalNumber++;

  rfConnection.sleepMode();

  Serial.println("");
  Serial.println("Going sleep...");

  delay(100);
}

void afterWakeUp() {
  Serial.println("");
  Serial.println("Wake Up...");
  Serial.println("");

  if (currentIntervalNumber % wakeUpIntervalNumber == 0) {
    makeReading = true;
    currentIntervalNumber = 0;

    rfConnection.wakeUp();
    delay(100);
  }
}

void setupPin() {
  pinMode(PIN_DHT11, INPUT);
}
