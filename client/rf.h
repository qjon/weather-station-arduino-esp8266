#include "configuration_prod.h"
#include "rf_encoder.h"

#ifndef rf_connection_h
#define rf_connection_h

byte buffer[61] = {0};

class RfConnection {
  public:
    RfConnection();
    void begin();
    String waitForReceive(int maxIterations);
    String receive();
    void sendSensorData(String deviceId, String msg);
    void txMode();
    void rxMode();
    void sleepMode();
    void wakeUp();
  private :
    String decode(String input);
    String encode(String deviceId, String input);
    RfEncoder encoder;
};

RfConnection::RfConnection() {}

void RfConnection::begin() {
  ELECHOUSE_cc1101.Init();              
  ELECHOUSE_cc1101.setCCMode(RF_CCMODE);
  ELECHOUSE_cc1101.setModulation(RF_MODULATION);  
  ELECHOUSE_cc1101.setMHZ(RF_FREQUENCY);   
  ELECHOUSE_cc1101.setSyncMode(RF_SYNC_MODE);
  ELECHOUSE_cc1101.setPA(RF_POWER);      
  ELECHOUSE_cc1101.setCrc(RF_CRC);
}

String RfConnection::waitForReceive(int maxIterations) {
  String msg = receive();

  int iterator = 0;
  while (msg == "" && iterator < maxIterations) {

    if (msg == "") {
      msg = receive();
    }

    iterator++;
  }

  return msg;
}

String RfConnection::receive() {
  String msg;

  if (ELECHOUSE_cc1101.CheckRxFifo(100)) {
    if (ELECHOUSE_cc1101.CheckCRC()) {
      int len = ELECHOUSE_cc1101.ReceiveData(buffer);
      buffer[len] = '\0';

      msg = (char*)buffer;
      msg = decode(msg);
    }
  }

  return msg;
}


void RfConnection::sendSensorData(String deviceId, String msg) {
  String message = encode(deviceId, msg);
  int length = message.length() + 1;

  char messageChar[length];
  message.toCharArray(messageChar, length);

  ELECHOUSE_cc1101.SendData(messageChar, 100);

  Serial.print("Sensor data send: ");
  Serial.print(msg);
  Serial.print(" (");
  Serial.print(message);
  Serial.println(")");
}

void RfConnection::txMode() {
  ELECHOUSE_cc1101.SetTx();
}

void RfConnection::rxMode() {
  ELECHOUSE_cc1101.SetRx();
}

String RfConnection::decode(String input) {
  return encoder.decode(input);
}

String RfConnection::encode(String deviceId, String input) {
  return encoder.encode(deviceId, input);
}

void RfConnection::sleepMode() {
  ELECHOUSE_cc1101.SpiStrobe(0x39);
}

void RfConnection::wakeUp() {
  rxMode();
}

#endif
