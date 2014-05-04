#!/bin/bash
#set -e
UNCRUSTIFY_OPTIONS=" -c .uncrustify.cfg -l CPP -q"

find $1 -name "*.h" -o -name "*.c" -o -name "*.cpp" |
while read filename; do
	uncrustify ${UNCRUSTIFY_OPTIONS} ${filename}

	# 差分チェック
	result=`diff ${filename} ${filename}.uncrustify`
	if [ -z "$result" ];then
#		echo ${filename}
		rm ${filename}.uncrustify
	else
		echo "M ${filename}"
	fi
done
