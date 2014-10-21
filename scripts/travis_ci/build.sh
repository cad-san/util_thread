#!/bin/bash
# build script for Travis CI

if [ "$BUILD_TARGET" = "test" ]; then
    if [ "$CC" = "clang" ]; then
        make test || exit 1
    else
        make gcov || exit 1
    fi
fi

if [ "$BUILD_TARGET" = "release" ]; then
    make release || exit 1
fi
