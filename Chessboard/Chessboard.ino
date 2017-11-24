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

void setup () {
    Serial.begin(9600);

    displayController = DisplayController::getInstance();
    displayController->print(1, "IOT chessboard");
    displayController->print(2, "By WCB Team");
    
    keyboardScanner = KeyboardScanner::getInstance();

    switchesController = SwitchesController::getInstance();

    motorsController = MotorsController::getInstance();

    messageController = MessageController::getInstance();
}

void loop() {
    keyboardScanner->scan();
    switchesController->scan();
    messageController->checkMessage();
}

