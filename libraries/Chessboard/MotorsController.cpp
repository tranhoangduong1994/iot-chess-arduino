#include <MotorsController.h>

#include <Arduino.h> 

#include <Bitboard.h>
#include <SwitchesController.h>
#include <MessageController.h>

#define MAGNET 12

#define DIR_X A1
#define STEP_X A0
#define ENABLE_X 38 // optional (just delete ENABLE from everywhere if not used)
#define STOP_X 18

#define DIR_Y A7
#define STEP_Y A6
#define ENABLE_Y A2 // optional (just delete ENABLE from everywhere if not used)
#define STOP_Y 19

const int QUICK_DELAY = 50;
const int NORMAL_DELAY = 50;
const int SLOW_DELAY = 100;

const int X0_OFFSET = 28;
const int Y0_OFFSET = 7;

const float SQUARE_SIZE = 39.625;
const int MOVING_DOWN_ADJUSTMENT_DISTANCE = 7;
const int MOVING_UP_ADJUSTMENT_DISTANCE = 3;
const int MOVING_LEFT_ADJUSTMENT_DISTANCE = 5;
const int MOVING_RIGHT_ADJUSTMENT_DISTANCE = 5;
const int MOVING_DIAGONALLY_DOWN_ADJUSTMENT_DISTANCE = 7;
const int MOVING_DIAGONALLY_RIGHT_ADJUSTMENT_DISTANCE = 4;
//const int DIAGONAL_MOVING_ADJUSTMENT_DISTANCE = 4;

MotorsController* MotorsController::instance = NULL;
bool MotorsController::isXMotorDisabled = false;
bool MotorsController::isYMotorDisabled = false;

MotorsController* MotorsController::getInstance() {
    if (!instance) {
        instance = new MotorsController();
        instance->init();
    }
    return instance;
}

void MotorsController::onXInterruptTriggered() {
    digitalWrite(ENABLE_X, HIGH);
    isXMotorDisabled = true;

    delay(400);

    if (digitalRead(STOP_X) == HIGH) {
        digitalWrite(ENABLE_X, LOW);
        isXMotorDisabled = false;
    }
}

void MotorsController::onYInterruptTriggered() {
    digitalWrite(ENABLE_Y, HIGH);
    isYMotorDisabled = true;

    delay(400);

    if (digitalRead(STOP_Y) == HIGH) {
        digitalWrite(ENABLE_Y, LOW);
        isYMotorDisabled = false;
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
    
    digitalWrite(ENABLE_X, HIGH);
    digitalWrite(ENABLE_Y, HIGH);

    MessageController::getInstance()->setMotorsControllerMessageDelegate(this);
}

void MotorsController::moveTo(Point target, MagnetState beginState, MagnetState endState, bool needPositionAdjusting) {
    if (beginState == ON) {
        digitalWrite(MAGNET, HIGH);
        delay(1000);
    } else if (beginState == OFF) {
        digitalWrite(MAGNET, LOW);
        delay(1000);
    }

    if (needPositionAdjusting) {
        if (target.y == currentPoint.y) {//horizontal
            if (target.x > currentPoint.x) {//moving right
                target.x += MOVING_RIGHT_ADJUSTMENT_DISTANCE;
            } else {//moving left
                target.x -= MOVING_LEFT_ADJUSTMENT_DISTANCE;
                if (target.x < 0) {
					target.x = 0;
				}
            }
        } else if (target.x == currentPoint.x) {//vertical
            if (target.y > currentPoint.y) {//moving up
                target.y += MOVING_UP_ADJUSTMENT_DISTANCE;
            } else {//moving down
                target.y -= MOVING_DOWN_ADJUSTMENT_DISTANCE;
                if (target.y < 0) {
					target.y = 0;
				}
            }
        } else {//diagonal
            if (target.x > currentPoint.x) {//moving right
                target.x += MOVING_DIAGONALLY_RIGHT_ADJUSTMENT_DISTANCE;
            } else {//moving left
                target.x -= MOVING_LEFT_ADJUSTMENT_DISTANCE;
                if (target.x < 0) {
					target.x = 0;
				}
            }

            if (target.y > currentPoint.y) {//moving up
                target.y += MOVING_UP_ADJUSTMENT_DISTANCE;
            } else {
                target.y -= MOVING_DIAGONALLY_DOWN_ADJUSTMENT_DISTANCE;
                if (target.y < 0) {
					target.y = 0;
				}                
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
    if (squareSum <= (0.5 * SQUARE_SIZE) * (0.5 * SQUARE_SIZE)) {
		delayDuration = SLOW_DELAY;
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
            //isXMotorDisabled = true;
        } else {
            digitalWrite(ENABLE_X, LOW);
            //isXMotorDisabled = false;
        }

        if (!distanceY) {
            digitalWrite(ENABLE_Y, HIGH);
            //isYMotorDisabled = true;
        } else {
            digitalWrite(ENABLE_Y, LOW);
            //isYMotorDisabled = false;
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
        
        if (distanceX == 0) {
			digitalWrite(ENABLE_X, HIGH);
		}
		
		if (distanceY == 0) {
			digitalWrite(ENABLE_Y, HIGH);
		}
    }

    if (endState == ON) {
        digitalWrite(MAGNET, HIGH);
        delay(1000);
    } else if (endState == OFF) {
        digitalWrite(MAGNET, LOW);
        delay(1000);
    }
}

Point MotorsController::getPointByPosition(Position pos) {
    if (pos.file < 'a' || pos.file > 'h' || pos.rank < 1 || pos.rank > 8) {
        assert(false);
    }

    int fileNo = (pos.file - 'a') + 1;
    int rankNo = pos.rank;

    return Point((fileNo - 1) * SQUARE_SIZE + X0_OFFSET, (rankNo - 1) * SQUARE_SIZE + Y0_OFFSET);
}

void MotorsController::moveToOrigin() {
    if (digitalRead(STOP_X) == HIGH) {
        digitalWrite(ENABLE_X, LOW);
        isXMotorDisabled = false;
    } else {
        digitalWrite(ENABLE_X, HIGH);
        isXMotorDisabled = true;
    }

    if (digitalRead(STOP_Y) == HIGH) {
        digitalWrite(ENABLE_Y, LOW);
        isYMotorDisabled = false;
    } else {
        digitalWrite(ENABLE_Y, HIGH);
        isYMotorDisabled = true;
    } 

    digitalWrite(DIR_X, HIGH);
    digitalWrite(DIR_Y, HIGH);

    while(!isXMotorDisabled || !isYMotorDisabled) {
        digitalWrite(STEP_X, HIGH);
        digitalWrite(STEP_Y, HIGH);

        delayMicroseconds(QUICK_DELAY);

        digitalWrite(STEP_X, LOW);
        digitalWrite(STEP_Y, LOW);

        delayMicroseconds(QUICK_DELAY);

        if (isXMotorDisabled && isYMotorDisabled) {
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
        moveTo(fromPoint, OFF, ON);
        moveTo(toPoint, UNCHANGED, OFF, true);
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
    //Point toPointAdjusted(toPoint.x + (MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveLeft), toPoint.y + (MOVING_ADJUSTMENT_DISTANCE) * pow(-1, moveDown));

    moveTo(fromPoint, OFF, ON);
    moveTo(middlePoint1, UNCHANGED, UNCHANGED);
    moveTo(middlePoint2, UNCHANGED, UNCHANGED);
    moveTo(toPoint, UNCHANGED, OFF, true);
}

void MotorsController::removePiece(Position pos) {
    Point point = getPointByPosition(pos);

    Point sideSquare = getPointByPosition(Position('h', to.rank));
    sideSquare.x += SQUARE_SIZE;
    
    Point middlePoint;
    middlePoint.x = point.x + SQUARE_SIZE / 2;
    
    if (to.rank > 4) {
        sideSquare.y -= SQUARE_SIZE / 2;
        middlePoint.y = point.y - SQUARE_SIZE / 2; 
    } else {
        sideSquare.y += SQUARE_SIZE / 2;
        middlePoint.y = point.y + SQUARE_SIZE / 2; 
    }
    
    //performing capture
    moveTo(point, OFF, ON);
    moveTo(middlePoint, UNCHANGED, UNCHANGED);
    moveTo(sideSquare, UNCHANGED, OFF);
}

void MotorsController::capturePiece(Position from, Position to, bool isKnight) {
    removePiece(to);
    movePiece(from, to, isKnight);
}

void MotorsController::onMoveRequest(Position from, Position to) {
    if (from.file < 'a' || from.file > 'h' || from.rank < 1 || from.rank > 8 ||
        to.file < 'a' || to.file > 'h' || from.rank < 1 || from.rank > 8) {
        MessageController::getInstance()->reply(MOVE_FAILED);
    }

    bool isKnight = false;

    if (abs(from.file - to.file) == 2 && abs(from.rank - to.rank) == 1 ||
        abs(from.file - to.file) == 1 && abs(from.rank - to.rank) == 2) {
        isKnight = true;
    }

    Bitboard currentBitboard = SwitchesController::getInstance()->getCurrentState();
    int squareIndex = (to.rank - 1) * 8 + (to.file - 97);
    if (currentBitboard.getBitByIndex(squareIndex)) {
        capturePiece(from, to, isKnight);
    } else {
        movePiece(from, to, isKnight);
    }

    SwitchesController::getInstance()->scan();
    Bitboard newBitboard = SwitchesController::getInstance()->getCurrentState();
    MessageController::getInstance()->reply(MOVE_DONE, from.toString() + to.toString() + newBitboard.toString());
}

void MotorsController::onEnPassantRequest(Position from, Position to) {
    Position capturedPiecePos(to.file, from.rank);

    removePiece(capturedPiecePos);
    movePiece(from, to);

    SwitchesController::getInstance()->scan();
    Bitboard newBitboard = SwitchesController::getInstance()->getCurrentState();
    MessageController::getInstance()->reply(MOVE_DONE, from.toString() + to.toString() + newBitboard.toString());
}

void MotorsController::onCastlingRequest(CastlingType type) {
	Position kingFromPos, kingToPos, rookFromPos, rookToPos;
	Point fromPoint, toPoint, middlePoint1, middlePoint2;
	switch(type) {
		case WHITE_QUEEN_SIDE:
			kingFromPos = Position('e', 1);
			kingToPos = Position('c', 1);
			rookFromPos = Position('a', 1);
			rookToPos = Position('d', 1);
			fromPoint = getPointByPosition(rookFromPos);
			toPoint = getPointByPosition(rookToPos);
			middlePoint1 = Point(fromPoint.x + (SQUARE_SIZE / 2), fromPoint.y + (SQUARE_SIZE / 2));
			middlePoint2 = Point(toPoint.x - (SQUARE_SIZE / 2), toPoint.y + (SQUARE_SIZE / 2));
			break;
			
		case WHITE_KING_SIDE:
			kingFromPos = Position('e', 1);
			kingToPos = Position('g', 1);
			rookFromPos = Position('h', 1);
			rookToPos = Position('f', 1);
			fromPoint = getPointByPosition(rookFromPos);
			toPoint = getPointByPosition(rookToPos);
			middlePoint1 = Point(fromPoint.x - (SQUARE_SIZE / 2), fromPoint.y + (SQUARE_SIZE / 2));
			middlePoint2 = Point(toPoint.x + (SQUARE_SIZE / 2), toPoint.y + (SQUARE_SIZE / 2));
			break;
			
		case BLACK_QUEEN_SIDE:
			kingFromPos = Position('e', 8);
			kingToPos = Position('c', 8);
			rookFromPos = Position('a', 8);
			rookToPos = Position('d', 8);
			fromPoint = getPointByPosition(rookFromPos);
			toPoint = getPointByPosition(rookToPos);
			middlePoint1 = Point(fromPoint.x + (SQUARE_SIZE / 2), fromPoint.y - (SQUARE_SIZE / 2));
			middlePoint2 = Point(toPoint.x - (SQUARE_SIZE / 2), toPoint.y - (SQUARE_SIZE / 2));
			break;
			
		case BLACK_KING_SIDE:
			kingFromPos = Position('e', 8);
			kingToPos = Position('g', 8);
			rookFromPos = Position('h', 8);
			rookToPos = Position('f', 8);
			fromPoint = getPointByPosition(rookFromPos);
			toPoint = getPointByPosition(rookToPos);
			middlePoint1 = Point(fromPoint.x - (SQUARE_SIZE / 2), fromPoint.y - (SQUARE_SIZE / 2));
			middlePoint2 = Point(toPoint.x + (SQUARE_SIZE / 2), toPoint.y - (SQUARE_SIZE / 2));			
			break;
			
		default:
			return;
	}
	
	movePiece(kingFromPos, kingToPos);
	moveTo(fromPoint, OFF, ON);
    moveTo(middlePoint1, UNCHANGED, UNCHANGED);
    moveTo(middlePoint2, UNCHANGED, UNCHANGED);
    moveTo(toPoint, UNCHANGED, OFF);
    
    SwitchesController::getInstance()->scan();
    Bitboard newBitboard = SwitchesController::getInstance()->getCurrentState();
    MessageController::getInstance()->reply(MOVE_DONE, kingFromPos.toString() + kingToPos.toString() + newBitboard.toString());
}

void MotorsController::onResetRequest() {
    moveToOrigin();
    SwitchesController::getInstance()->scan();
    String bitboard = SwitchesController::getInstance()->getCurrentState().toString();
    MessageController::getInstance()->reply(RESET_DONE, bitboard);
}

void MotorsController::onMagnetOnRequest() {
	Serial.println("onMagnetOnRequest|");
	digitalWrite(MAGNET, HIGH);
}

void MotorsController::onMagnetOffRequest() {
	Serial.println("onMagnetOffRequest|");
	digitalWrite(MAGNET, LOW);
}
