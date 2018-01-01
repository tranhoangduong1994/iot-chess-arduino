#include <MotorsController.h>
#include <SwitchesController.h>
#include <MessageController.h>
#include <SpeakerController.h>

MessageController* messageController;

bool system_ready = false;

void setup () {
    Serial.begin(115200);

    SwitchesController::getInstance();
    MotorsController::getInstance();
    SpeakerController::getInstance();

    messageController = MessageController::getInstance();
}

void loop() {
    messageController->checkMessage();
}

