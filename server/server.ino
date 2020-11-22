#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Base64.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>

#include "configuration_prod.h"
#include "mqtt.h"
#include "rf_connection.h"
#include "sensor_data_store.h"
#include "sensor_data_list.h"
#include "sensor_data.h"

RfConnection rfConnection;

// Initialize Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.pl.pool.ntp.org");

WiFiClient espClient;
PubSubClient client(espClient);

ESP8266WebServer server(80);


SensorDataStore store;

void setup() {

  Serial.begin(57600);

  rfConnection.begin();

  Serial.println("");

  connectToWiFi();
  
  server.on("/", handleLogs);
  server.begin();

  timeClient.begin();

  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

}

void loop() {
  server.handleClient();

  
  String msg = rfConnection.receive();

  if (msg != "") {
    Serial.println(msg);

    String data[5];

    convertString2Array(msg, data);

    if ((data[3] == "" || data[3] == NULL) && (data[4] == "" || data[4] == NULL)) {
      // old sensor with DHT
      SensorData sensorData("0", timeClient.getEpochTime(), data[1].toFloat(), data[2].toFloat());

      sendData(data[0], sensorData);

      sensorData.print();

      store.get(data[0]).add(sensorData);
    } else {
      float pressure = data[4] == "null" ? NULL : data[4].toFloat();
      SensorData sensorData(data[1], timeClient.getEpochTime(), data[2].toFloat(), data[3].toFloat(), pressure);


      sendData(data[0], sensorData);

      sensorData.print();

      store.get(data[0]).add(sensorData);
    }
  }
}


// deviceID, recordID, Timestamp, Temp, Hum, DewPoint
void convertString2Array(String data, String* arr) {

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

void connectToWiFi() {
  Serial.println("Connecting to ");
  Serial.println(SSID);

  //connect to your local wi-fi network
  WiFi.begin(SSID, PASSWORD);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Got MAC: ");
  Serial.println(WiFi.macAddress());
}

void sendData(String uniqId, SensorData sd) {
  Mqtt mqttTransporter(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASS, uniqId);
  mqttTransporter.setClient(&client);

  String message = dataToMqttString(uniqId, sd);

  mqttTransporter.send(sd.getSymbol(), message);
}



String dataToMqttString(String uniqId, SensorData sd) {
  String message;
  const int capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 80;

  DynamicJsonDocument root(capacity);

  root["uniqId"] = uniqId;
  root["sensor"] = sd.getSymbol();

  JsonObject data = root.createNestedObject("payload");

  data["time"] = sd.getTimestamp();
  data["temp"] = sd.getTemperature();
  data["hum"] = sd.getHumidity();
  data["pressure"] = sd.getPressure();
  data["dewPoint"] = sd.getDewPoint();

  serializeJson(root, message);

  return message;
}



void handleLogs() {
  server.send(200, "text/plain", store.log());
}
