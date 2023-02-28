#!/bin/bash

if [ $# -lt 1 ];then
	echo "Usage: $0 [path to save *.so]"
	exit 1
fi

BUILD_ROOT="$(dirname $0 | xargs realpath)/../build"
SAVE_ROOT="$(realpath $1)"
ABIS=("arm64-v8a" "armeabi-v7a" "x86_64" "x86")

if [ -d ${BUILD_ROOT} ];then
	rm -rf ${BUILD_ROOT}
fi

if [ -d ${SAVE_ROOT} ];then
	rm -rf ${SAVE_ROOT}
fi

mkdir ${SAVE_ROOT}

for ABI in ${ABIS[@]}
do
	mkdir ${BUILD_ROOT}
	pushd ${BUILD_ROOT}
	cmake ../ \
	-DCMAKE_TOOLCHAIN_FILE=/home/th000/Android/Sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI=${ABI} \
	-DANDROID_PLATFORM=android-33 \
	-DBUILD_SHARED_LIBS=on \
	-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -fstack-protector-all -D_FORTIFY_SOURCE=2 -O2 -UNDEBUG" \
	-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -fstack-protector-all -D_FORTIFY_SOURCE=2 -O2 -UNDEBUG"
	
	cmake --build . -j $(nproc) --target MeipuruLibC
	mkdir ${SAVE_ROOT}/${ABI}
	cp -rf ./libMeipuruLibC.so ./taglib/taglib/libtag.so ./libMeipuruLib.so ${SAVE_ROOT}/${ABI}/
	popd
	rm -rf ${BUILD_ROOT}
done

