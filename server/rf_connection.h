#ifndef rf_connection_h
#define rf_connection_h

#include "configuration_prod.h"
#include "rf_encoder.h"

byte buffer[61] = {0};

class RfConnection {
  public:
    RfConnection();
    void begin();
    String receive();
    void sendConfirmation(String deviceId, String recordId, String timestamp);
  private :
    String decode(String input);
    String encode(String deviceId, String input);
    RfEncoder encoder;
};

RfConnection::RfConnection() {}

void RfConnection::begin() {
  ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
  ELECHOUSE_cc1101.setCCMode(RF_CCMODE);       // set config for internal transmission mode.
  ELECHOUSE_cc1101.setModulation(RF_MODULATION);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
  ELECHOUSE_cc1101.setMHZ(RF_FREQUENCY);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.setSyncMode(RF_SYNC_MODE);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
  ELECHOUSE_cc1101.setPA(RF_POWER);      // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
  ELECHOUSE_cc1101.setCrc(RF_CRC);     // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
}

String RfConnection::receive() {
  String msg;

  if (ELECHOUSE_cc1101.CheckRxFifo(100)) {
    if (ELECHOUSE_cc1101.CheckCRC()) {
      int len = ELECHOUSE_cc1101.ReceiveData(buffer);
      buffer[len] = '\0';

      msg = (char*)buffer;
      msg = decode(msg);
      Serial.println();
    }
  }

  return msg;
}

void RfConnection::sendConfirmation(String deviceId, String recordId, String timestamp) {
  String msg = recordId + "|" + timestamp;
  String message = encode(deviceId, msg);
  int length = message.length() + 1;

  char messageChar[length];
  message.toCharArray(messageChar, length);

  ELECHOUSE_cc1101.SendData(messageChar, 200);

  Serial.print("message Send: ");
  Serial.print(msg);
  Serial.print(" (");
  Serial.print(message);
  Serial.println(")");
}

String RfConnection::decode(String input) {
  return encoder.decode(input);
}

String RfConnection::encode(String deviceId, String input) {
  return encoder.encode(deviceId, input);
}

#endif
