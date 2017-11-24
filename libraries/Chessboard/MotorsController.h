#ifndef __MotorsController__
#define __MotorsController__

#include <assert.h>

#include <DataTypes.h>

class MotorsController : public MotorsControllerMessageProtocol {
public:
    void moveToOrigin();
    void movePiece(Position from, Position to, bool isKnight = false);
    void capturePiece(Position from, Position to, bool isKnight = false);

    void onMoveRequest(Position from, Position to) override;
    void onResetRequest() override;

    static MotorsController* getInstance();

private:
    Point currentPoint;
    static bool isXMotorEnabled;   
    static bool isYMotorEnabled;

    static void onXInterruptTriggered();
    static void onYInterruptTriggered();

    void init();
    void moveTo(Point target, MagnetState beginState, MagnetState endState, bool needPositionAdjusting);

    Point getPointByPosition(Position pos);

    MotorsController() {}
    static MotorsController* instance;
};

#endif
