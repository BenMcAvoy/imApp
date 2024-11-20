#!/usr/bin/env bash

export VCPKG_CMAKE_SYSTEM_NAME=Windows
export VCPKG_TARGET_TRIPLET=x64-windows

cmake -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_SYSTEM_NAME=Windows \
	-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
	-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
	-DCMAKE_CXX_FLAGS="-static-libgcc -static-libstdc++ -static" \
	-DCMAKE_EXE_LINKER_FLAGS="-static" \
	-DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
	-B build \
	-G Ninja

cmake --build build --config Release
