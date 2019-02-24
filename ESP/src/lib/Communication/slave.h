//
// Created by Scott Zhao on 2019-02-18.
//

#ifndef ESP_SLAVE_H
#define ESP_SLAVE_H

#include <Arduino.h>
#include "ESPNow.h"

class slave : public ESPNow {
public:
    slave(uint8_t *myMacAddr, uint8_t *masterMacAddr, bool debug);

    void send(uint8_t *msg, int len) {
        ESPNow::send(masterMacAddr, msg, len);
    }

    static void messageHandler(uint8_t *mac, uint8_t *data, uint8_t len);

    void setMsgCallback() override {
        esp_now_register_recv_cb(messageHandler);
    }

protected:
    uint8_t *masterMacAddr;
};


#endif //ESP_SLAVE_H
