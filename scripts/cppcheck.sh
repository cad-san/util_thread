#!/bin/bash
CPPCHECK_OPTIONS="--enable=style,performance,portability,information"

while [ -n "$2" ]
do
        INCDIRS+="-I $1 "
        shift
done
SRCDIR=${1}

cppcheck ${CPPCHECK_OPTIONS} ${INCDIRS} ${SRCDIR}
