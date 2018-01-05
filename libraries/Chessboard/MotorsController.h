#ifndef __MotorsController__
#define __MotorsController__

#include <assert.h>

#include <MessageController.h>

#include <DataTypes.h>

class MotorsController : public MotorsControllerMessageProtocol {
public:
    void moveToOrigin();
    void movePiece(Position from, Position to, bool isKnight = false);
    void removePiece(Position pos);
    void capturePiece(Position from, Position to, bool isKnight = false);

    void onMoveRequest(Position from, Position to) override;
    void onEnPassantRequest(Position from, Position to) override;
    void onCastlingRequest(CastlingType type) override;
    void onResetRequest() override;
    void onMagnetOnRequest() override;
    void onMagnetOffRequest() override;

    static MotorsController* getInstance();

private:
    Point currentPoint;
    static bool isXMotorDisabled;   
    static bool isYMotorDisabled;

    static void onXInterruptTriggered();
    static void onYInterruptTriggered();

    void init();
    void moveTo(Point target, MagnetState beginState, MagnetState endState, bool needPositionAdjusting = false);

    Point getPointByPosition(Position pos);

    MotorsController() {}
    static MotorsController* instance;
};

#endif
