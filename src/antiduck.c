#include <stdio.h>
#include <CoreAudio/AudioHardware.h>

#include <funchook.h>

#define _STR(x) #x
#define STR(x) _STR(x)

#define _LOGAT __FILE__ ":" STR(__LINE__) "] "
#define LOG(fmt, ...) fprintf(stdout, _LOGAT fmt "\n", ##__VA_ARGS__)

OSStatus AudioDeviceDuck(AudioDeviceID inDevice,
                         Float32 inDuckedLevel,
                         const AudioTimeStamp* __nullable inStartTime,
                         Float32 inRampDuration) __attribute__((weak_import));

OSStatus Patched_AudioDeviceDuck(AudioDeviceID inDevice,
                                 Float32 inDucedLevel,
                                 const AudioTimeStamp *inStartTime,
                                 Float32 inRampDuration) {
    LOG("HOW DARE YOU!!!");
    return 0;
}

__attribute__((constructor))
static void ctor(void) {
    funchook_t *hook = funchook_create();
    int ret;

    void *add = AudioDeviceDuck;
    ret = funchook_prepare(hook, &add, Patched_AudioDeviceDuck);
    if (ret != 0) {
        LOG("failed to prepare function %d: %s", ret, funchook_error_message(hook));
        goto __error;
    }

    ret = funchook_install(hook, 0);
    if (ret != 0) {
        LOG("failed to install function %d: %s", ret, funchook_error_message(hook));
        goto __error;
    }

    LOG("AudioDeviceDuck() patched!");

    return;

__error:
    funchook_destroy(hook);
}

