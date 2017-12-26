#ifndef __DataTypes__
#define __DataTypes__

#include <Arduino.h>

enum CastlingType {
	WHITE_QUEEN_SIDE,
	WHITE_KING_SIDE,
	BLACK_QUEEN_SIDE,
	BLACK_KING_SIDE
};

enum MagnetState {
    UNCHANGED,
    OFF,
    ON
};

struct Point {
    int x;
    int y;
    Point() {}
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

struct Position {
    int file;
    int rank;
    Position() {}
    Position(int file, int rank) {
          this->file = file;
          this->rank = rank;
    }
    String toString() {
        return String((char)file) + String(rank);
    }
};

#endif
