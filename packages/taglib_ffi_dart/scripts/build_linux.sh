#!/bin/bash

set -e

if [ -z "$1" ];then
  echo "error: need to specify the directory to save built *.so".
  echo ""
  echo "Usage: $0 <directory/to/save/built/*.so>"
  exit 1
fi

SCRIPT_DIR="$(dirname $0 | xargs realpath)"
SRC_ROOT="${SCRIPT_DIR}/../src"
SAVE_ROOT="$(realpath $1)"
BUILD_ROOT="${SCRIPT_DIR}/../src/build"

cmake -S "${SRC_ROOT}" -B "${BUILD_ROOT}"
cmake --build "${BUILD_ROOT}" -j $(nproc)

if [ ! -d "${SAVE_ROOT}" ];then
	mkdir "${SAVE_ROOT}"
fi

cp "${BUILD_ROOT}/libtaglib_ffi.so" "${SAVE_ROOT}/"
cp "${BUILD_ROOT}/taglib/taglib/libtag.so" "${SAVE_ROOT}/"
cp "${BUILD_ROOT}/taglib/taglib/libtag.so.2" "${SAVE_ROOT}/"
cp "${BUILD_ROOT}/taglib/taglib/libtag.so.2.0.1" "${SAVE_ROOT}/"

echo "Success, libs saved in directory: ${SAVE_ROOT}"

