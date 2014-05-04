#This makefile makes all the main book code with CppUTest test harness

#Set this to @ to keep the makefile quiet
SILENCE = @

#--- Outputs & Inputs ----#
include target.mk

#---- Setting ----#
CPPUTEST_WARNINGFLAGS = $(CPPWARNINGFLAGS)

CPPUTEST_CXXFLAGS +=
CPPUTEST_LDFLAGS += $(LIBRARY_DIRS)
LD_LIBRARIES = $(LIBRARY_FILES)

CPPUTEST_USE_EXTENSIONS = Y

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
