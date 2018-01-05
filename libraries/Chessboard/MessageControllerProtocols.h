#ifndef __MessageControllerProtocols__
#define __MessageControllerProtocols__

#include <DataTypes.h>
#include <Arduino.h>

class MotorsControllerMessageProtocol {
public:
	virtual void onMoveRequest(Position from, Position to) = 0;
	virtual void onEnPassantRequest(Position from, Position to) = 0;
	virtual void onCastlingRequest(CastlingType type) = 0;
	virtual void onResetRequest() = 0;
	virtual void onMagnetOnRequest() = 0;
	virtual void onMagnetOffRequest() = 0;
};

class SwitchesControllerMessageProtocol {
public:
	virtual void onScanRequest() = 0;
	virtual void onResetRequest() = 0;
};

class SpeakerControllerMessageProtocol {
public:
	virtual void onSoundRequest(SoundType soundType) = 0;
};

#endif
