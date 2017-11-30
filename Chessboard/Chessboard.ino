#include <MotorsController.h>
#include <DisplayController.h>
#include <SwitchesController.h>
#include <KeyboardScanner.h>
#include <MessageController.h>

MotorsController* motorsController;
DisplayController* displayController;
SwitchesController* switchesController;
KeyboardScanner* keyboardScanner;
MessageController* messageController;

bool system_ready = false;

void setup () {
    Serial.begin(9600);

    displayController = DisplayController::getInstance();
    displayController->print(1, "IOT chessboard");
    displayController->print(2, "By WCB Team");
    
    keyboardScanner = KeyboardScanner::getInstance();

    switchesController = SwitchesController::getInstance();

//    motorsController = MotorsController::getInstance();

    messageController = MessageController::getInstance();
}

void loop() {
    if (Serial) {
        if (!system_ready) {
            system_ready = true;
            delay(100);
            MessageController::getInstance()->send(SYSTEM_READY);
        }
    }
    
    switchesController->scan();
    keyboardScanner->scan();
    messageController->checkMessage();
}

