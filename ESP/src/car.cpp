#include "lib/Util/AutoParkConfig.h"
#include "lib/Module/AutoParkCar.h"

AutoParkCar *APCar = nullptr;
int SERIAL_BAUD_RATE = 115200;


void setup() {
    delay(200);
    APCar = new AutoParkCar(CAR1, true);
}

void loop() {
    APCar->messageHandler();
    APCar->autoStop();
    yield();
}