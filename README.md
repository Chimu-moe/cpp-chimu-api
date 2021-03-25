# cpp-chimu-api
C++ Chimu API wrapper with easy usage.

## Install dependencies: UBUNTU
```
sudo apt install build-essential meson libcurl4-openssl-dev
```

## How to build
```
meson build
ninja -C build
```

## Examples
Very basic example:
```cpp
    Chimu::APIAccess access;

    if (access.isConnected()) {
        try {
            Chimu::Beatmap map = access.getBeatmap(75);
            // TODO: do something with this map
        } catch (Chimu::APIException ex) {
            // TODO: handle
        }
    }
```

# LICENSE
My code is licensed under UNLICENSE, do whatever you like.
Please note that some third-party dependencies might have different licenses!
