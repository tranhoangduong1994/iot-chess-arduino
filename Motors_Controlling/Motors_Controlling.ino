#include <Arduino.h>
#include <assert.h>

#define MAGNET 8

#define DIR_X A1
#define STEP_X A0
#define ENABLE_X 38 // optional (just delete ENABLE from everywhere if not used)
#define STOP_X 18

#define DIR_Y A7
#define STEP_Y A6
#define ENABLE_Y A2 // optional (just delete ENABLE from everywhere if not used)
#define STOP_Y 19

int currentX, currentY;
boolean isXEnabled, isYEnabled;

const int X0_OFFSET = 27;
const int Y0_OFFSET = 0;
const int QUICK_DELAY = 25;
const int NORMAL_DELAY = 50;

void onXInterrupted();
void onYInterrupted(); 


/////////////////////////////////////////////////////////////////////////////////////////
#pragma custom data types

enum MagnetState {
    UNCHANGED,
    OFF,
    ON
};

struct PhysicsPosition {
    int x;
    int y;
    PhysicsPosition(int x, int y) {
       this->x = x;
       this->y = y; 
    }
    PhysicsPosition();
};

struct LogicPosition {
    int file;
    char rank;
    LogicPosition(int file, int rank) {
        this->file = file;
        this->rank = rank;
    }
};

PhysicsPosition getPhysicsPositionByLogicPosition(LogicPosition logicPosition) {
    if (logicPosition.file < 'a' || logicPosition.file > 'h') {
        assert(false);
    }       

    int fileNo = (logicPosition.file - 'a') + 1;
    int rankNo = logicPosition.rank;

    return PhysicsPosition((fileNo - 1) * 40 + X0_OFFSET, (rankNo - 1) * 40 + Y0_OFFSET);
}

void moveTo(PhysicsPosition target, MagnetState beginningState, MagnetState endingState, bool positionAdjustment);
void moveToOrigin();

void movePiece(LogicPosition from, LogicPosition to);
void moveKnight(LogicPosition from, LogicPosition to);
void capturePiece(LogicPosition from, LogicPosition to, bool isKnight = false);

//void testMove(LogicPosition lPos) {
//    PhysicsPosition pPos = getPhysicsPositionByLogicPosition(lPos);
//    moveTo(pPos.x, pPos.y);
//}
/////////////////////////////////////////////////////////////////////////////////////////

#pragma Adruino system delegators

void setup() {
  Serial.begin(9600);

  // Configure stopper pin to read HIGH unless grounded
  pinMode(STOP_X, INPUT_PULLUP);//keo chan ngat X len + nguon
  pinMode(STOP_Y, INPUT_PULLUP);

  pinMode(MAGNET, OUTPUT);

  pinMode(STEP_X, OUTPUT);
  pinMode(ENABLE_X, OUTPUT);
  pinMode(DIR_X, OUTPUT);

  pinMode(STEP_Y, OUTPUT);
  pinMode(ENABLE_Y, OUTPUT);
  pinMode(DIR_Y, OUTPUT);

  attachInterrupt(5, onXInterrupted, FALLING);//ngat khi xay ra chuyen tu cao xuong thap
  attachInterrupt(4, onYInterrupted, FALLING);//ngat khi xay ra chuyen tu cao xuong thap
  
  delay(200);//delay for config

  moveToOrigin();
}

void loop() {
    moveKnight(LogicPosition('g', 1), LogicPosition('f', 3));
    moveKnight(LogicPosition('f', 3), LogicPosition('g', 1));
}
////////////////////////////////////////////////////////////////////////////////

#pragma custom system delegators

void onXInterrupted() {
  Serial.println("onXInterrupted");

  digitalWrite(ENABLE_X, HIGH);
  isXEnabled = true;

  delay(400);

  if (digitalRead(STOP_X) == HIGH) {
    digitalWrite(ENABLE_X, LOW); 
    isXEnabled = false;
    Serial.println("ko phai ve goc toa do X");
  } else {
    Serial.println("ve goc toa do X");
  }
}

void onYInterrupted() {
  Serial.println("onYInterrupted");

  digitalWrite(ENABLE_Y, HIGH); 
  isYEnabled = true;

  delay(400);

  if (digitalRead(STOP_Y) == HIGH) {
    digitalWrite(ENABLE_Y, LOW);
    isYEnabled = false;

    Serial.println("ko phai ve goc toa do Y");
  } else {
    Serial.println(" ve goc toa do Y");
  }
}

////////////////////////////////////////////////////////////////////////////////
void moveToOrigin(){
  Serial.println("moving to origin");

  //kiem tra cong tac co bi ngat ko neu bi ngat tuc la dang o vi tri goc roi
  if (digitalRead(STOP_X) == HIGH) {
    digitalWrite(ENABLE_X, LOW); 
    isXEnabled = false;
  } else {
    digitalWrite(ENABLE_X, HIGH); 
    isXEnabled = true;
  }

  if (digitalRead(STOP_Y) == HIGH) { 
    digitalWrite(ENABLE_Y, LOW);
    isYEnabled = false;
  } else { 
    digitalWrite(ENABLE_Y, HIGH);
    isYEnabled = true;
  }

  digitalWrite(DIR_X, HIGH);
  digitalWrite(DIR_Y, HIGH);

  while (!isXEnabled || !isYEnabled) {
    digitalWrite(STEP_X, HIGH);
    digitalWrite(STEP_Y, HIGH);

    delayMicroseconds(25);

    digitalWrite(STEP_X, LOW);
    digitalWrite(STEP_Y, LOW);

    delayMicroseconds(25);

    if (isXEnabled && isYEnabled) {
      break;
    }
  }

  currentX = 0; 
  currentY = 0;

  Serial.println("Exit moveToOrigin");
}

void moveTo(PhysicsPosition target, MagnetState beginningState, MagnetState endingState, bool positionAdjustment) {
    int targetX = target.x;
    int targetY = target.y;
    Serial.print("moving to");
    Serial.print(" x = "); 
    Serial.print(targetX);
    
    Serial.print(", y = "); 
    Serial.println(targetY);
    
    if (beginningState == MagnetState::ON) {
        Serial.println("Enabled maget");
        digitalWrite(MAGNET, HIGH);
    } else if (beginningState == MagnetState::OFF) {
        Serial.println("Disabled maget");
        digitalWrite(MAGNET, LOW);
    }

  if (positionAdjustment) {
      if (targetY == currentY) {//horizontal
        if (targetX > currentX) {//moving right
            targetX += 5;
        } else {//moving left
            targetX -= 5;
        }
      } else if (targetX == currentX) {//vertical
        if (targetY > currentY) {//moving up
            targetY += 5;
        } else {//moving down
            targetY -= 5;
        }
      } else {//diagonal
        if (targetX > currentX) {//moving right
            targetX += 5;
        } else {//moving left
            targetX -= 5;
        }
        if (targetY > currentY) {//moving up
            targetY += 5;
        } else {//moving down
            targetY -= 5;
        }
      }
  }

  long distanceX = targetX - currentX;
  long distanceY = targetY - currentY;

  long a1 = distanceX * distanceX;
  long a2 = distanceY * distanceY;
  long sum = a1 + a2;
  
  int delayDuration = NORMAL_DELAY;
  if (sum >= 120 * 120) {
    Serial.print("YES, sum = ");
    Serial.println(sum);
    delayDuration = QUICK_DELAY;
  } else {
    Serial.print("NO, a1 = ");
    Serial.print(a1);
    Serial.print(", a2 = ");
    Serial.println(a2);
  }

  if (distanceX > 0) {
    digitalWrite(DIR_X,LOW);// di chuyen ra xa goc toa do
  } else {
    digitalWrite(DIR_X,HIGH); // di chuyen ve goc toa do
  }

  if (distanceY > 0) {
    digitalWrite(DIR_Y,LOW);// di chuyen ra xa goc toa do
  } else {
    digitalWrite(DIR_Y,HIGH); // di chuyen ve goc toa do    
  }

  while (distanceX || distanceY) {
    if (!distanceX) {
      digitalWrite(ENABLE_X, HIGH);
      isXEnabled = true;
    } else {
      digitalWrite(ENABLE_X, LOW);
      isXEnabled = false;
    }

    if (!distanceY) {
      digitalWrite(ENABLE_Y, HIGH);
      isYEnabled = true;
    } else {
      digitalWrite(ENABLE_Y, LOW);
      isYEnabled = false;
    }

    // chay 1 vong-1mm
    for (int i = 0; i < 200; i++) {
      digitalWrite(STEP_X, HIGH); // Output high
      digitalWrite(STEP_Y, HIGH); 
      delayMicroseconds(delayDuration); //
      
      digitalWrite(STEP_X, LOW); // Output low
      digitalWrite(STEP_Y, LOW); 
      delayMicroseconds(delayDuration); //
    }

    if (distanceX < 0) {
      distanceX++;
      currentX--;
    } else if (distanceX > 0) {
      distanceX--;
      currentX++;
    }

    if (distanceY < 0) {
      distanceY++;
      currentY--;
    } else if (distanceY > 0) {
      distanceY--;
      currentY++;
    }
  }

    if (endingState == MagnetState::ON) {
        Serial.println("Enabled maget");
        digitalWrite(MAGNET, HIGH);
    } else if (endingState == MagnetState::OFF) {
        Serial.println("Disabled maget");
        digitalWrite(MAGNET, LOW);
    }
}

void movePiece(LogicPosition from, LogicPosition to) {
    PhysicsPosition pFrom = getPhysicsPositionByLogicPosition(from);
    PhysicsPosition pTo = getPhysicsPositionByLogicPosition(to);
    
    moveTo(pFrom, MagnetState::OFF, MagnetState::ON, false);
    moveTo(pTo, MagnetState::UNCHANGED, MagnetState::OFF, true);
}

void moveKnight(LogicPosition from, LogicPosition to) {
    int deltaFile = to.file - from.file;
    int deltaRank = to.rank - from.rank;
    int moveLeft;
    int moveDown;
    
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

    PhysicsPosition pFrom = getPhysicsPositionByLogicPosition(from);
    PhysicsPosition pTo = getPhysicsPositionByLogicPosition(to);

    PhysicsPosition pMiddle1(pFrom.x + (20 + 5) * pow(-1, moveLeft), pFrom.y + (20 + 5) * pow(-1, moveDown));
    PhysicsPosition pMiddle2(pTo.x - (20 - 5) * pow(-1, moveLeft), pTo.y - (20 - 5) * pow(-1, moveDown));
    PhysicsPosition pToAdjusted(pTo.x + 5 * pow(-1, moveLeft), pTo.y + 5 * pow(-1, moveDown));

    moveTo(pFrom, MagnetState::OFF, MagnetState::ON, false);
    moveTo(pMiddle1, MagnetState::UNCHANGED, MagnetState::UNCHANGED, false);
    moveTo(pMiddle2, MagnetState::UNCHANGED, MagnetState::UNCHANGED, false);
    moveTo(pToAdjusted, MagnetState::UNCHANGED, MagnetState::OFF, false);
    
}

void capturePiece(LogicPosition from, LogicPosition to, bool isKnight) {
    PhysicsPosition pFrom = getPhysicsPositionByLogicPosition(from);
    PhysicsPosition pTo = getPhysicsPositionByLogicPosition(to);
    
    PhysicsPosition sideSquare = getPhysicsPositionByLogicPosition(LogicPosition('h', to.rank));
    sideSquare.x += 40;
    
    moveTo(pTo, MagnetState::OFF, MagnetState::ON, false);
    moveTo(sideSquare, MagnetState::UNCHANGED, MagnetState::OFF, false);
    
    if (!isKnight) {
        movePiece(from, to);
        return;
    }

    moveKnight(from, to);
}

