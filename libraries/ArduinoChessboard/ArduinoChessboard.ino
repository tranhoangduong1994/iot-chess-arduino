#include <MotorsController.h>
#include <SwitchesController.h>
#include <MessageController.h>

MessageController* messageController;

bool system_ready = false;

void setup () {
    Serial.begin(115200);
    
/*    pinMode(10, OUTPUT);

    digitalWrite(10, LOW);
    delay(150);
    digitalWrite(10, HIGH);
    delay(50);
    digitalWrite(10, LOW);
    delay(150);
    digitalWrite(10, HIGH);*/

    SwitchesController::getInstance();
    MotorsController::getInstance();
    messageController = MessageController::getInstance();
}

void loop() {
    messageController->checkMessage();
}

