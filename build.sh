#!/bin/bash
cd ./build
cmake -DTONE_FREQUENCY=19000 .. && cd ..
cmake --build build && ./build/QtAudioWake
