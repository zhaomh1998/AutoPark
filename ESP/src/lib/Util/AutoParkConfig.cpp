#include "AutoParkConfig.h"

uint8_t AutoParkConfig::theCmd[4];

uint8_t *AutoParkConfig::Ack(bool trueOrFalse) {
    theCmd[0] = TARGET_ACK;
    theCmd[1] = trueOrFalse ? ACK_TRUE : ACK_FALSE;
    return theCmd;
}

uint8_t *AutoParkConfig::FloorCommand(AutoParkConfig::FloorOperation anOp, AutoParkConfig::FloorArg2 anArg2,
                                      AutoParkConfig::FloorCarDestination aDestination) {
    theCmd[0] = TARGET_FLOOR;
    switch(anOp) {
        case FloorOperation::statusUpdate:  theCmd[1] = FLOOR_STATUS_UPDATE_CMD; break;
        case FloorOperation::calibration:   theCmd[1] = FLOOR_CALIBRATE_CMD; break;
        case FloorOperation::moveCart:      theCmd[1] = FLOOR_MOVE_CART_CMD; break;
        case FloorOperation::moveCar:       theCmd[1] = FLOOR_MOVE_CAR_CMD; break;
    }
    switch(anArg2) {
        // For Status Update
        case FloorArg2::NoArg:    theCmd[2] = FLOOR_NO_ARG; break;
        case FloorArg2::request:  theCmd[2] = FLOOR_STATUS_REQUEST; break;
        case FloorArg2::ready:    theCmd[2] = FLOOR_STATUS_READY; break;
        case FloorArg2::working:  theCmd[2] = FLOOR_STATUS_BUSY; break;
        case FloorArg2::error:    theCmd[2] = FLOOR_STATUS_ERROR; break;
        case FloorArg2::getElevatorLaser: theCmd[2] = FLOOR_STATUS_GET_ELEV_LASER; break;
        case FloorArg2::turnOnStepper:    theCmd[2] = FLOOR_STATUS_TURN_ON_STEPPER; break;
        case FloorArg2::turnOffStepper:   theCmd[2] = FLOOR_STATUS_TURN_OFF_STEPPER; break;
        // For MoveCart
        case FloorArg2::elevator: theCmd[2] = FLOOR_ELEVATOR; break;
        case FloorArg2::lot1:     theCmd[2] = FLOOR_LOT1; break;
        case FloorArg2::lot2:     theCmd[2] = FLOOR_LOT2; break;
        case FloorArg2::lot3:     theCmd[2] = FLOOR_LOT3; break;
        // For MoveCar
        case FloorArg2::car1:     theCmd[2] = FLOOR_CAR1; break;
        case FloorArg2::car2:     theCmd[2] = FLOOR_CAR2; break;
        case FloorArg2::car3:     theCmd[2] = FLOOR_CAR3; break;
    }
    switch(aDestination) {
        case FloorCarDestination::NoArg:  theCmd[3] = FLOOR_NO_ARG; break;
        case FloorCarDestination::inLot:  theCmd[3] = FLOOR_CAR_IN_LOT; break;
        case FloorCarDestination::inElev: theCmd[3] = FLOOR_CAR_IN_ELEV; break;
        case FloorCarDestination::toCart: theCmd[3] = FLOOR_CAR_TO_CART; break;
    }
    return theCmd;
}

uint8_t *AutoParkConfig::CarCmd(AutoParkConfig::CarCommand aCommand) {
    theCmd[0] = TARGET_CAR;
    switch (aCommand) {
        case CarCommand::forward:   theCmd[1] = CAR_FORWARD; break;
        case CarCommand::backward:  theCmd[1] = CAR_BACKWARD; break;
        case CarCommand::stop:      theCmd[1] = CAR_STOP; break;
    }
    return theCmd;
}
