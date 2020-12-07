#include "rf_encoder.h"

String RfEncoder::encode(String deviceId, String message) {
    // put some your logic here
    return deviceId + "|" + message;
}

String RfEncoder::decode(String message) {
    // put some your logic here
    return message;
}
