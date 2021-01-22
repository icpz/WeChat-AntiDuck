#include <stdio.h>
#include <CoreAudio/AudioHardware.h>

#include <fishhook.h>

#define _STR(x) #x
#define STR(x) _STR(x)

#define _LOGAT __FILE__ ":" STR(__LINE__) "] "
#define LOG(fmt, ...) fprintf(stdout, _LOGAT fmt "\n", ##__VA_ARGS__)

OSStatus Patched_AudioDeviceDuck(AudioDeviceID inDevice,
                                 Float32 inDucedLevel,
                                 const AudioTimeStamp *inStartTime,
                                 Float32 inRampDuration) {
    LOG("HOW DARE YOU!!!");
    return 0;
}

__attribute__((constructor))
static void ctor(void) {
    int ret;
    void *origin_binding;

    struct rebinding rebindingds[] = {
        { "AudioDeviceDuck", (void *)&Patched_AudioDeviceDuck, &origin_binding},
    };

    ret = rebind_symbols(rebindingds, 1);

    if (ret) {
        LOG("rebind failed: %d", ret);
        return;
    }

    LOG("rebind %s with %p, orig=%p", rebindingds[0].name, rebindingds[0].replacement, rebindingds[0].replaced);

    LOG("AudioDeviceDuck() patched!");
}

