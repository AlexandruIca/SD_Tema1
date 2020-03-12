#!/usr/bin/env bash

cmake-format -i ./CMakeLists.txt \
                ./src/CMakeLists.txt \
                ./cmake/CompilerWarnings.cmake \
                ./cmake/StandardProjectSettings.cmake
