#!/bin/bash
set -e

# Career Navigator — Build & Launch Script
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

# Locate Qt CMake and tools
CMAKE_BIN="/Users/gunjan/Qt/Tools/CMake/CMake.app/Contents/bin/cmake"
NINJA_BIN="/Users/gunjan/Qt/Tools/Ninja/ninja"
QT_PREFIX="/Users/gunjan/Qt/6.11.1/macos"

if [ ! -f "$CMAKE_BIN" ]; then
    CMAKE_BIN="cmake"
fi

echo "=========================================================="
echo "   🚀 CAREER NAVIGATOR — C++ OOPs PRACTICAL (Qt6 GUI)    "
echo "=========================================================="

mkdir -p build
echo "Configuring project..."
if [ -f "$NINJA_BIN" ]; then
    "$CMAKE_BIN" -B build -G Ninja -DCMAKE_PREFIX_PATH="$QT_PREFIX" -DCMAKE_MAKE_PROGRAM="$NINJA_BIN"
else
    "$CMAKE_BIN" -B build -DCMAKE_PREFIX_PATH="$QT_PREFIX"
fi

echo "Compiling application..."
"$CMAKE_BIN" --build build

echo "Launching Career Navigator GUI..."
exec ./build/CareerNavigator
