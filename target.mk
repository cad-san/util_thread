#---- Outputs ----#
COMPONENT_NAME = util_thread

#--- Inputs ----#
CPPUTEST_HOME = cpputest
CPP_PLATFORM = gcc
PROJECT_HOME_DIR = .
SCRIPT_DIR = $(PROJECT_HOME_DIR)/scripts

SRC_DIRS = \
	$(PROJECT_HOME_DIR)/src\

TEST_SRC_DIRS = \
	$(PROJECT_HOME_DIR)/tests\
	$(PROJECT_HOME_DIR)/tests/thread\
	$(PROJECT_HOME_DIR)/tests/message\

INC_DIRS = \
	$(PROJECT_HOME_DIR)/include\
	$(PROJECT_HOME_DIR)/mocks\

INCLUDE_DIRS = \
	$(CPPUTEST_HOME)/include\
	$(PROJECT_HOME_DIR)/tests\
	${INC_DIRS}

MOCKS_SRC_DIRS = \
	$(PROJECT_HOME_DIR)/mocks\

#--- Librarys ---#
LIBRARY_DIRS = \

LIBRARY_FILES = \
	-lpthread\
	-lrt\

#--- Configs ---#

CPPWARNINGFLAGS = \
	-Wall -W -Werror -pedantic-errors\
	-Wcast-qual -Wcast-align -Wwrite-strings\
	-Wconversion -Wfloat-equal -Wpointer-arith\

CPPOPTIONFLAGS = \
	-std=c++0x -Wno-c++98-compat
