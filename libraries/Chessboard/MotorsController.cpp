#include <MotorsController.h>

#include <Arduino.h> 

#include <Bitboard.h>
#include <SwitchesController.h>
#include <MessageController.h>

#define MAGNET 8

#define DIR_X A1
#define STEP_X A0
#define ENABLE_X 38 // optional (just delete ENABLE from everywhere if not used)
#define STOP_X 18

#define DIR_Y A7
#define STEP_Y A6
#define ENABLE_Y A2 // optional (just delete ENABLE from everywhere if not used)
#define STOP_Y 19

const int QUICK_DELAY = 25;
const int NORMAL_DELAY = 50;

const int X0_OFFSET = 27;
const int Y0_OFFSET = 0;

const int SQUARE_SIZE = 40;
const int MOVING_ADJUSTMENT_DISTANCE = 5;

MotorsController* MotorsController::instance = NULL;
bool MotorsController::isXMotorEnabled = false;
bool MotorsController::isYMotorEnabled = false;

MotorsController* MotorsController::getInstance() {
    if (!instance) {
        instance = new MotorsController();
        instance->init();
    }
    return instance;
}

void MotorsController::onXInterruptTriggered() {
    digitalWrite(ENABLE_X, HIGH);
    isXMotorEnabled = true;

    delay(400);

    if (digitalRead(STOP_X) == HIGH) {
        digitalWrite(ENABLE_X, LOW);
        isXMotorEnabled = false;
    }
}

void MotorsController::onYInterruptTriggered() {
    digitalWrite(ENABLE_Y, HIGH);
    isYMotorEnabled = true;

    delay(400);

    if (digitalRead(STOP_Y) == HIGH) {
        digitalWrite(ENABLE_Y, LOW);
        isYMotorEnabled = false;
    }
}

void MotorsController::init() {
    pinMode(STOP_X, INPUT_PULLUP);
    pinMode(STOP_Y, INPUT_PULLUP);

    pinMode(MAGNET, OUTPUT);

    pinMode(STEP_X, OUTPUT);
    pinMode(ENABLE_X, OUTPUT);
    pinMode(DIR_X, OUTPUT);

    pinMode(STEP_Y, OUTPUT);
    pinMode(ENABLE_Y, OUTPUT);
    pinMode(DIR_Y, OUTPUT);    

    attachInterrupt(5, MotorsController::onXInterruptTriggered, FALLING);
    attachInterrupt(4, MotorsController::onYInterruptTriggered, FALLING);

    delay(200);

    moveToOrigin();
}

void MotorsController::moveTo(Point target, MagnetState beginState, MagnetState endState, bool needPositionAdjusting) {
    if (beginState == MagnetState::ON) {
        digitalWrite(MAGNET, HIGH);
        delay(1000);
    } else if (beginState == MagnetState::OFF) {
        digitalWrite(MAGNET, LOW);
        delay(1000);
    }

    if (needPositionAdjusting) {
        if (target.y == currentPoint.y) {//horizontal
            if (target.x > currentPoint.x) {//moving right
                target.x += MOVING_ADJUSTMENT_DISTANCE;
            } else {//moving left
                target.x -= MOVING_ADJUSTMENT_DISTANCE;
            }
        } else if (target.x == currentPoint.x) {//vertical
            if (target.y > currentPoint.y) {//moving up
                target.y += MOVING_ADJUSTMENT_DISTANCE;
            } else {//moving down
                target.y -= MOVING_ADJUSTMENT_DISTANCE;
            }
        } else {//diagonal
            if (target.x > currentPoint.x) {//moving right
                target.x += MOVING_ADJUSTMENT_DISTANCE;
            } else {//moving left
                target.x -= MOVING_ADJUSTMENT_DISTANCE;
            }

            if (target.y > currentPoint.y) {//moving up
                target.y += MOVING_ADJUSTMENT_DISTANCE;
            } else {
                target.y -= MOVING_ADJUSTMENT_DISTANCE;
            }
        }
    }

    long distanceX = target.x - currentPoint.x;
    long distanceY = target.y - currentPoint.y;
    long squareSum = distanceX * distanceX + distanceY * distanceY;

    int delayDuration = NORMAL_DELAY;
    if (squareSum >= (3 * SQUARE_SIZE) * (3 * SQUARE_SIZE)) {
        delayDuration = QUICK_DELAY;
    }

    if (distanceX > 0) {
        digitalWrite(DIR_X, LOW);
    } else {
        digitalWrite(DIR_X, HIGH);
    }

    if (distanceY > 0) {
        digitalWrite(DIR_Y, LOW);
    } else {
        digitalWrite(DIR_Y, HIGH);
    }

    while (distanceX || distanceY) {
        if (!distanceX) {
            digitalWrite(ENABLE_X, HIGH);
            isXMotorEnabled = true;
        } else {
            digitalWrite(ENABLE_X, LOW);
            isXMotorEnabled = false;
        }

        if (!distanceY) {
            digitalWrite(ENABLE_Y, HIGH);
            isYMotorEnabled = true;
        } else {
            digitalWrite(ENABLE_Y, LOW);
            isYMotorEnabled = false;
        }

        for (int i = 0; i < 200; i++) {
            digitalWrite(STEP_X, HIGH);
            digitalWrite(STEP_Y, HIGH);
            delayMicroseconds(delayDuration);

            digitalWrite(STEP_X, LOW);
            digitalWrite(STEP_Y, LOW);
            delayMicroseconds(delayDuration);
        }

        if (distanceX < 0) {
            distanceX++;
            currentPoint.x--;
        } else if (distanceX > 0) {
            distanceX--;
            currentPoint.x++;
        }

        if (distanceY < 0) {
            distanceY++;
            currentPoint.y--;
        } else if (distanceY > 0) {
            distanceY--;
            currentPoint.y++;
        }        
    }

    if (endState == MagnetState::ON) {
        digitalWrite(MAGNET, HIGH);
        delay(1000);
    } else if (endState == MagnetState::OFF) {
        digitalWrite(MAGNET, LOW);
        delay(1000);
    }
}

Point MotorsController::getPointByPosition(Position pos) {
    if (pos.file < 'a' || pos.file > 'h') {
        assert(false);
    }

    int fileNo = (pos.file - 'a') + 1;
    int rankNo = pos.rank;

    return Point((fileNo - 1) * SQUARE_SIZE + X0_OFFSET, (rankNo - 1) * SQUARE_SIZE + Y0_OFFSET);
}

void MotorsController::moveToOrigin() {
    if (digitalRead(STOP_X) == HIGH) {
        digitalWrite(ENABLE_X, LOW);
        isXMotorEnabled = false;
    } else {
        digitalWrite(ENABLE_X, HIGH);
        isXMotorEnabled = true;
    }

    if (digitalRead(STOP_Y) == HIGH) {
        digitalWrite(ENABLE_Y, LOW);
        isYMotorEnabled = false;
    } else {
        digitalWrite(ENABLE_Y, HIGH);
        isYMotorEnabled = true;
    } 

    digitalWrite(DIR_X, HIGH);
    digitalWrite(DIR_Y, HIGH);

    while(!isXMotorEnabled || !isYMotorEnabled) {
        digitalWrite(STEP_X, HIGH);
        digitalWrite(STEP_Y, HIGH);

        delayMicroseconds(QUICK_DELAY);

        digitalWrite(STEP_X, LOW);
        digitalWrite(STEP_Y, LOW);

        delayMicroseconds(QUICK_DELAY);

        if (isXMotorEnabled && isYMotorEnabled) {
            break;
        }
    }

    currentPoint.x = 0;
    currentPoint.y = 0;
}

void MotorsController::movePiece(Position from, Position to, bool isKnight) {
    Point fromPoint = getPointByPosition(from);
    Point toPoint = getPointByPosition(to);
    
    if (!isKnight) {
        moveTo(fromPoint, MagnetState::OFF, MagnetState::ON);
        moveTo(toPoint, MagnetState::UNCHANGED, MagnetState::OFF, true);
        return;
    }

    int deltaFile = to.file - from.file;
    int deltaRank = to.rank - from.rank;
    int moveLeft, moveDown;

    if (deltaFile < 0) {
        moveLeft = 1;
    } else if (deltaFile > 0) {
        moveLeft = 0;
    } else {
        assert(false);
    }

    if (deltaRank < 0) {
        moveDown = 1;
    } else if (deltaRank > 0) {
        moveDown = 0;
    } else {
        assert(false);
    }    

    // Point middlePoint1(fromPoint.x + (SQUARE_SIZE / 2 + MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveLeft), fromPoint.y + (SQUARE_SIZE / 2 + MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveDown));
    // Point middlePoint2(toPoint.x - (SQUARE_SIZE / 2 - MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveLeft), toPoint.y - (SQUARE_SIZE / 2 - MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveDown));
    Point middlePoint1(fromPoint.x + (SQUARE_SIZE / 2) * pow(-1, moveLeft), fromPoint.y + (SQUARE_SIZE / 2) * pow(-1, moveDown));
    Point middlePoint2(toPoint.x - (SQUARE_SIZE / 2) * pow(-1, moveLeft), toPoint.y - (SQUARE_SIZE / 2) * pow(-1, moveDown));
    Point toPointAdjusted(toPoint.x + (MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveLeft), toPoint.y + (MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveDown));

    moveTo(fromPoint, MagnetState::OFF, MagnetState::ON);
    moveTo(middlePoint1, MagnetState::UNCHANGED, MagnetState::UNCHANGED);
    moveTo(middlePoint2, MagnetState::UNCHANGED, MagnetState::UNCHANGED);
    moveTo(toPointAdjusted, MagnetState::UNCHANGED, MagnetState::OFF);
}

void MotorsController::capturePiece(Position from, Position to, bool isKnight) {
    Point fromPoint = getPointByPosition(from);
    Point toPoint = getPointByPosition(to);

    Point sideSquare = getPointByPosition(Position('h', to.rank));
    sideSquare.x += SQUARE_SIZE;
    sideSquare.y += SQUARE_SIZE / 2;

    //performing capture
    moveTo(toPoint, MagnetState::OFF, MagnetState::ON);
    moveTo(Point(toPoint.x + SQUARE_SIZE / 2, toPoint.y + SQUARE_SIZE / 2), MagnetState::UNCHANGED, MagnetState::UNCHANGED);
    moveTo(sideSquare, MagnetState::UNCHANGED, MagnetState::OFF);

    movePiece(from, to, isKnight);
}

void MotorsController::onMoveRequest(Position from, Position to) {
    if (from.file < 'a' || from.file > 'h' || from.rank < 1 || from.rank > 8 ||
        to.file < 'a' || to.file > 'h' || from.rank < 1 || from.rank > 8) {
        MessageController::getInstance()->reply(ReplyingType::MOVE_FAILED);
    }

    bool isKnight = false;

    if (abs(from.file - to.file) == 2 && abs(from.rank - to.rank) == 1 ||
        abs(from.file - to.file) == 1 && abs(from.rank - to.rank) == 2) {
        isKnight = true;
    }

    const Bitboard& currentSwitches = SwitchesController::getInstance()->getCurrentState();
    int squareIndex = (to.rank - 1) * 8 + (to.file - 96);
    if (currentSwitches.getBitByIndex(squareIndex)) {
        capturePiece(from, to, isKnight);
    } else {
        movePiece(from, to, isKnight);
    }

    SwitchesController::getInstance()->scan();

    MessageController::getInstance()->reply(ReplyingType::MOVE_DONE, currentSwitches.toString());
}

void MotorsController::onResetRequest() {
    moveToOrigin();
    MessageController::getInstance()->reply(ReplyingType::RESET_DONE);
}