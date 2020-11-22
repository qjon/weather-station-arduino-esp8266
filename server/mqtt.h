#ifndef mqtt_h
#define mqtt_h

#include "sensor_data.h"
#include <ArduinoJson.h>

class Mqtt {
  public:
    Mqtt(char* url, uint16_t port, char* user, char* password, String uniqId);
    virtual bool send(String sensor, String message);
    void sendInfo(String ip, String sensor0neName, String sensorTwoName);
    void setClient(PubSubClient *client);
  private:
    PubSubClient* _client;
    bool connect();
    String getDeviceTopic(String topic);
    String getDeviceTopic(String topic, String sensor);
    char* _url;
    uint16_t _port;
    char* _user;
    char* _password;
    String _uniqId;
    int maxNumberOfConnectionTries = 10;
};

Mqtt::Mqtt(char* url, uint16_t port, char* user, char* password, String uniqId) {
  _url = url;
  _port = port;
  _user = user;
  _password = password;
  _uniqId = uniqId;
}

void Mqtt::sendInfo(String ip, String sensor0neName, String sensorTwoName) {
  if (connect()) {
    String message;
    const int capacity = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 80;

    DynamicJsonDocument root(capacity);

    root["ip"] = ip;
    root["uniqId"] = _uniqId;

    JsonArray sensors = root.createNestedArray("sensors");

    JsonObject s0 = sensors.createNestedObject();

    s0["symbol"] = 0;
    s0["name"] = sensor0neName;

    if (sensorTwoName != "") {
      JsonObject s1 = sensors.createNestedObject();

      s1["symbol"] = 1;
      s1["name"] = sensorTwoName;
    }

    serializeJson(root, message);

    String topic = getDeviceTopic("INFO");

    _client->publish(topic.c_str(), message.c_str());
  }
}

bool Mqtt::send(String sensor, String message) {
  if (connect()) {
    String topic = getDeviceTopic("SENSOR", sensor);
    Serial.println(topic);

    _client->publish(topic.c_str(), message.c_str());

    return true;
  }
  else {
    return false;
  }
}


void Mqtt::setClient(PubSubClient *client) {
  _client = client;
}

bool Mqtt::connect() {
  int connectionTries = 0;

  _client->setServer(_url, _port);

  while (!_client->connected() && connectionTries < maxNumberOfConnectionTries) {
    Serial.println("Connecting to MQTT...");
    if (_client->connect("WS", _user, _password)) {

      Serial.println("connected");

    } else {
      Serial.print("failed with state: ");
      Serial.println(_client->state());
      delay(500);
      connectionTries++;
    }
  }

  return _client->connected();
}

String Mqtt::getDeviceTopic(String topic) {
  return "ws/" + _uniqId + "/" + topic;
}

String Mqtt::getDeviceTopic(String topic, String sensor) {
  return "ws/" + _uniqId + "_" + (String)sensor + "/" + topic;
}

#endif
