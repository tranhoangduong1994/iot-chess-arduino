#ifndef __SpeakerController__
#define __SpeakerController__

#include <assert.h>

#include <MessageController.h>

#include <DataTypes.h>

class SpeakerController : public SpeakerControllerMessageProtocol {
public:
    void onSoundRequest(SoundType soundType) override;

    static SpeakerController* getInstance();

private:
    static SpeakerController* instance;
};

#endif
