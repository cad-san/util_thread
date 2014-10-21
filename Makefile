# this is set up to by default to make the top level and test with CppUTest
all: test

clean:
	@if [ -e cpputest/Makefile_using_MakefileWorker ]; \
	then make clean_test; \
	else make clean_release; \
	fi

clean_all: clean CppUTestClean

### for test code
test: CppUTest CppUTestExt
	make -f scripts/cpputest.mk

gcov: CppUTest CppUTestExt clean
	make -f scripts/cpputest.mk CPPUTEST_USE_GCOV=Y gcov

clean_test:
	make -i -f scripts/cpputest.mk clean

### for release build
release: clean
	make -f scripts/release.mk

clean_release:
	make -i -f scripts/release.mk clean

### utility
format:
	make -i -f scripts/utility.mk uncrustify

check:
	make -i -f scripts/utility.mk cppcheck

### for cpputest
CppUTest: cpputest/lib/libCppUTest.a

CppUTestExt: cpputest/lib/libCppUTestExt.a

CppUTestClean:
	make -i -C cpputest -f Makefile_using_MakefileWorker clean
	make -i -C cpputest -f Makefile_using_MakefileWorker cleanExtensions

cpputest/lib/libCppUTest.a:
	make -i -C cpputest -f Makefile_using_MakefileWorker

cpputest/lib/libCppUTestExt.a:
	make -i -C cpputest -f Makefile_using_MakefileWorker extensions
