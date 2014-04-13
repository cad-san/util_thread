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

INC_DIRS = \
	$(PROJECT_HOME_DIR)/include\
	$(PROJECT_HOME_DIR)/mocks\

INCLUDE_DIRS = \
	$(CPPUTEST_HOME)/include\
	$(PROJECT_HOME_DIR)/tests\
	${INC_DIRS}

MOCKS_SRC_DIRS = \
	$(PROJECT_HOME_DIR)/mocks\
