
Anti AudioDeviceDuck

# Introduction

This project aims to invalid `AudioDeviceDuck` by hooking it with an empty function.

# Build & Install

## Dependency

+ CMake 3.10+

+ Xcode / Xcode CLI Tools

## Build

```bash
# clone and change into the repo directory

git clone --recurse-submodules https://github.com/icpz/WeChat-AntiDuck

mkdir -p build && cd build

cmake .. && make
```

The above steps will produce `build/injection`.

## Install

```bash
# assume already in `build' directory after build step

make inject
```

## Uninstall

```bash
# assume already in `build' directory after build step

make restore
```

## Inject Other Applications

The script injects WeChat.app by default, but you can also inject other apps.

```bash
# MUST cd into build/injection now
cd build/injection

sudo make APP_EXEC=QQ inject

# sudo make APP_EXEC=QQ restore
```


If the executable of application is not match with the application name, you could invoke

```bash
sudo make APP_EXEC=obs APP_NAME=OBS.app inject
```

# Thanks

+ [chromium](https://chromium.googlesource.com/chromium/src/media/+/master/audio/mac/audio_low_latency_input_mac.cc#38)

+ [kubo/funchook](https://github.com/kubo/funchook)

+ [Sunnyyoung/WeChatTweak-macOS](https://github.com/Sunnyyoung/WeChatTweak-macOS)

