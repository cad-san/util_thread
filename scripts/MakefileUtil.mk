#This makefile for Util Command

#Set this to @ to keep the makefile quiet
SILENCE = @

#--- Outputs & Inputs ----#
include target.mk

uncrustify:
	${SILENCE} ${SCRIPT_DIR}/uncrustify.sh $(SRC_DIRS)
	${SILENCE} ${SCRIPT_DIR}/uncrustify.sh $(INC_DIRS)
	${SILENCE} ${SCRIPT_DIR}/uncrustify.sh $(TEST_SRC_DIRS)
	${SILENCE} ${SCRIPT_DIR}/uncrustify.sh $(MOCKS_SRC_DIRS)

cppcheck:
	${SILENCE} ${SCRIPT_DIR}/cppcheck.sh $(INC_DIRS) $(PROJECT_HOME_DIR)/src
