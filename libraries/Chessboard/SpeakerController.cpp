#include "SpeakerController.h"

#include "MessageController.h"

SpeakerController* SpeakerController::instance = NULL;

SpeakerController* SpeakerController::getInstance() {
	if (!instance) {
		instance = new SpeakerController();
		pinMode(10, OUTPUT);
		digitalWrite(10, HIGH);
		MessageController::getInstance()->setSpeakerControllerMessageDelegate(instance);
	}
	/*digitalWrite(10, LOW);
    delay(150);
    digitalWrite(10, HIGH);
    delay(50);
    digitalWrite(10, LOW);
    delay(150);
    digitalWrite(10, HIGH);*/
	return instance;
}

void SpeakerController::onSoundRequest(SoundType type) {
	switch(type) {
		case SOUND_SYSTEM_READY:
		case SOUND_GAME_READY:
		case SOUND_VALID_MOVE:
			digitalWrite(10, LOW);
    		delay(150);
    		digitalWrite(10, HIGH);
    		delay(50);
    		digitalWrite(10, LOW);
    		delay(150);
    		digitalWrite(10, HIGH);
			break;

		case SOUND_INVALID_MOVE:
		case SOUND_OFFPOSITION:
		case SOUND_MULTIPLEMOVE:
			digitalWrite(10, LOW);
    		delay(500);
    		digitalWrite(10, HIGH);
    		break;

    	default:
    		break;
	}
}