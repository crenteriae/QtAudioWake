#!/bin/bash
cd ./build
cmake -DTONE_FREQUENCY=22000 .. && cd ..
cmake --build build && ./build/QtAudioWake
