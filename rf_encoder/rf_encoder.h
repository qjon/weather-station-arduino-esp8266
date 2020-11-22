#ifndef rf_encoder_h
#define rf_encoder_h

class RfEncoder {
    public:
        String decode(String message);
        String encode(String deviceId, String message);
};

#endif
