#include <MotorsController.h>
#include <SwitchesController.h>
#include <MessageController.h>

MessageController* messageController;

bool system_ready = false;

void setup () {
    Serial.begin(115200);

    SwitchesController::getInstance();
    MotorsController::getInstance();
    messageController = MessageController::getInstance();
}

void loop() {
    messageController->checkMessage();
}

