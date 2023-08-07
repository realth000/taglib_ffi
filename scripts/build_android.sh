#!/bin/bash

set -ex

if [ "x${ANDROID_HOME}" == "x" ];then
	echo "error: env ANDROID_HOME not set"
	exit 1
fi

SCRIPT_DIR="$(dirname $0 | xargs realpath)"
SRC_ROOT="${SCRIPT_DIR}/../src"
BUILD_ROOT="${SCRIPT_DIR}/../build/taglib_ffi_android"
#SAVE_ROOT="$(realpath $1)"
SAVE_ROOT="${SCRIPT_DIR}/../android/libs"
ABIS=("arm64-v8a" "armeabi-v7a" "x86_64" "x86")
NDK_ROOT=""

if [ ! -d "${ANDROID_HOME}/ndk/" ];then
	echo "error: Android NDK not installed in ANDROID_HOME(${ANDROID_HOME}). Install Android ndk and try this later"
	exit 1
fi


NDK_ROOT="$(ls -d ${ANDROID_HOME}/ndk/* | sort | tail -n 1)"

if [ "x${NDK_ROOT}" == "x" ];then
	echo "error: Can not find a installed Android NDK in ANDROID_HOME\(${ANDROID_HOME}\). Install Android ndk and try this later"
	exit 1
fi

if [ -d ${SAVE_ROOT} ];then
	rm -rf ${SAVE_ROOT}/*
fi

if [ ! -d "${BUILD_ROOT}" ];then
	mkdir -p "${BUILD_ROOT}"
fi

if [ ! -d "${SAVE_ROOT}" ];then
	mkdir "${SAVE_ROOT}"
fi

for ABI in ${ABIS[@]}
do
	if [ -d ${BUILD_ROOT} ];then
		rm -rf ${BUILD_ROOT}
	fi
	mkdir ${BUILD_ROOT}
	pushd ${BUILD_ROOT}
	cmake ${SRC_ROOT} \
	-DCMAKE_TOOLCHAIN_FILE="${NDK_ROOT}"/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI=${ABI} \
	-DANDROID_PLATFORM=android-33 \
	-DBUILD_SHARED_LIBS=on \
	-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -fstack-protector-all -D_FORTIFY_SOURCE=2 -O2 -UNDEBUG" \
	-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -fstack-protector-all -D_FORTIFY_SOURCE=2 -O2 -UNDEBUG"

	cmake --build . -j $(nproc) --target taglib_ffi
	mkdir ${SAVE_ROOT}/${ABI}
	cp -rf $(find -name "*.so")  ${SAVE_ROOT}/${ABI}/
	popd
	rm -rf ${BUILD_ROOT}
done

find ${SAVE_ROOT} -name "*.so" | xargs "${NDK_ROOT}"/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip

echo "Success, libs saved in directory: ${SAVE_ROOT}"

