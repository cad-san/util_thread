# this is set up to by default to make the top level and test with CppUTest
all: test

clean:
	@if [ -e cpputest/Makefile_using_MakefileWorker ]; \
	then make cleanTest; \
	else make cleanRelease; \
	fi

test: CppUTest CppUTestExt
	make -f scripts/cpputest.mk

gcov: CppUTest CppUTestExt clean
	make -f scripts/cpputest.mk CPPUTEST_USE_GCOV=Y gcov

cleanTest:
	make -i -f scripts/cpputest.mk clean

release: clean
	make -f scripts/release.mk

cleanRelease:
	make -i -f scripts/release.mk clean

format:
	make -i -f scripts/utility.mk uncrustify

check:
	make -i -f scripts/utility.mk cppcheck

CppUTest: cpputest/lib/libCppUTest.a

CppUTestExt: cpputest/lib/libCppUTestExt.a

cpputest/lib/libCppUTest.a:
	pwd
	make -i -C cpputest -f Makefile_using_MakefileWorker

cpputest/lib/libCppUTestExt.a:
	make -i -C cpputest -f Makefile_using_MakefileWorker extensions
