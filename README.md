# weather-station-arduino-esp8266

## Installation

### Message encoder
* copy directory _rf_encoder_ to your Arduino library folder and implement there _encode_ and _decode_ functions

        include "rf_encoder.h"

        String RfEncoder::encode(String deviceId, String message) {
          // put some your logic here
          return deviceId + "|" + message;
        }

        String RfEncoder::decode(String message) {
          // put some your logic here
          return message;
        }

### Client

* copy _configuration.h_ file as _configuration_prod.h_ and set some parameters in proper way 
* setup RF configuration

        #define RF_CCMODE 1
        #define RF_MODULATION 0
        #define RF_FREQUENCY 433.92
        #define RF_SYNC_MODE 2
        #define RF_POWER 12
        #define RF_CRC 1

* setup DHT data pin if needed 

        #define PIN_DHT11 9

* setup data reading interval

        #define SLEEP_INTERVAL_SEC 300

* setup device unique ID (most important unique in whole your system)

        #define DEVICE_UNIQUE_ID "ABC123"

* in _client.ino_ file initialize your sensors

        Sensor sensorOne(0x76);              // BME 280
        Sensor sensorTwo;                    // none


        Sensor sensorOne(0x76);              // BME 280
        Sensor sensorTwo(DHT11_PIN, DHT11);  // DHT 11


### Bridge

* copy _configuration.h_ file as _configuration_prod.h_ and set some parameters in proper way 
* enter your network data 

        #define SSID "YOUR_WIFI_SESSION_ID"
        #define PASSWORD "YOUR_WIFI_PASSWORD"

* setup RF configuration (the same way as in _Client_)

        #define RF_CCMODE 1
        #define RF_MODULATION 0
        #define RF_FREQUENCY 433.92
        #define RF_SYNC_MODE 2
        #define RF_POWER 12
        #define RF_CRC 1

* setup MQTT server connection

        #define MQTT_SERVER "MQTT_SERVER_IP"
        #define MQTT_PORT 1883
        #define MQTT_USER "MQTT_SERVER_USERNAME"
        #define MQTT_PASS "MQTT_SERVER_PASSWORD"

* setup additional configuration

        #define MAX_NUMBER_OF_DEVICES 10      // max 10
        #define SENSOR_DATA_LIST_LENGTH 20    // max 20


## Communication

### Client -> Bridge

Data send from _Client_ to _Bridge_ can be encoded or not but at the end the _decode_ function should return string in such way:

        ABC123|0|21.19|39.25|997.96

where we have few information seperates by the "pipe":

* device unique id - device id contains 6 chars (letters and digits)
* sensor number - digit which represent sensor connected to device, by default 0 means first sensor, 1 - second sensor, if you want more it is possible to add more
* temperature - temperature in Celsius degrees (rounded to two decimal places)
* humidity - in percents (rounded to two decimal places)
* pressure - in hPa (rounded to two decimal places), if device do not have such sensor it returns null

### Bridge -> MQTT

_Bridge_ try to decode data from _Client_ and add to it additional values _timestamp_ and _dew point_. All that data is sent to MQTT on topic _ws/DEVICE_UNIQUE_ID/SENSOR_ where _DEVICE_UNIQUE_ID_ is symbol of the device.   Message that is send looks like below.

        {
            "uniqId": "ABC123",
            "sensor": "0",
            "payload": {
                "time": 1607006383,
                "temp": 23,
                "hum": 60,
                "pressure": 999.72,
                "dewPoint": 14.79
            }
        }

## Bridge HTTP

Bridge has additional option. It expose HTTP server on port 80, where you can find all data that it receive and decode grouped by device.

## Summary

More info about this project you can find on my [blog](https://ignaszewski.pl/en/weather-station-code-description).

