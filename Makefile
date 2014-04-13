# this is set up to by default to make the top level and test with CppUTest
all: test

clean:
	@if [ -e cpputest/Makefile_using_MakefileWorker ]; \
	then make cleanTest; \
	else make cleanRelease; \
	fi

test: CppUTest CppUTestExt
	make -f scripts/MakefileCppUTest.mk

gcov: CppUTest CppUTestExt clean
	make -f scripts/MakefileCppUTest.mk CPPUTEST_USE_GCOV=Y gcov

cleanTest:
	make -i -f scripts/MakefileCppUTest.mk clean

release: clean
	make -f scripts/MakefileRelease.mk

cleanRelease:
	make -i -f scripts/MakefileRelease.mk clean

format:
	make -i -f scripts/MakefileUtil.mk uncrustify

check:
	make -i -f scripts/MakefileUtil.mk cppcheck

CppUTest: cpputest/lib/libCppUTest.a

CppUTestExt: cpputest/lib/libCppUTestExt.a

cpputest/lib/libCppUTest.a:
	pwd
	make -i -C cpputest -f Makefile_using_MakefileWorker

cpputest/lib/libCppUTestExt.a:
	make -i -C cpputest -f Makefile_using_MakefileWorker extensions
