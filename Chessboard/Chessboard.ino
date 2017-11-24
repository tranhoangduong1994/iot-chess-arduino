#include <MotorsController.h>
#include <DisplayController.h>
#include <BoardScanner.h>
#include <KeyboardScanner.h>
#include <MessageController.h>

MotorsController* motorsController;
DisplayController* displayController;
BoardScanner* boardScanner;
KeyboardScanner* keyboardScanner;

bool flag = false;
Bitboard currentBitboard;

const int MOVES_NUMBER = 6;

Position POSITIONS_TO_MOVE[MOVES_NUMBER * 2] = {
    Position('e', 2), Position('e', 4),
    Position('e', 7), Position('e', 5),
    Position('g', 1), Position('f', 3),
    Position('b', 8), Position('c', 6),
    Position('f', 1), Position('c', 4),
    Position('f', 8), Position('c', 5)
};

int iPosition = 0;

void setup () {
    Serial.begin(9600);
    
    displayController = DisplayController::getInstance();
    displayController->print(1, "IOT chessboard");
    displayController->print(2, "By WCB Team");
    
    motorsController = MotorsController::getInstance();
   
    boardScanner = BoardScanner::getInstance();
    currentBitboard = Bitboard();
}

void loop() {
    Bitboard newBitboard = boardScanner->scan();
    Bitboard comparingBitboard = currentBitboard.compare(newBitboard);
    Serial.print("Current bitboard: ");
    Serial.println(currentBitboard.toString());
    Serial.print("New bitboard: ");
    Serial.println(newBitboard.toString());
    Serial.print("Comparing bitboard: ");
    Serial.println(comparingBitboard.toString());
    Serial.println("----------------------------------------------------------");
    
    if (!flag) {
        if (iPosition > MOVES_NUMBER * 2) {
            flag = true;
            return;
        }

        motorsController->movePiece(POSITIONS_TO_MOVE[iPosition], POSITIONS_TO_MOVE[iPosition + 1]);
        iPosition += 2;
        delay(1000);
    }
}

