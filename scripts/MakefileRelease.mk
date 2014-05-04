#This makefile makes all the main book code with CppUTest test harness

#Set this to @ to keep the makefile quiet
SILENCE = @

#--- Outputs & Inputs ----#
include target.mk

OBJS_DIR = $(PROJECT_HOME_DIR)/objs
LIB_DIR = $(PROJECT_HOME_DIR)/lib

#---- Helper Functions ----#
get_src_from_dir  = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
get_dirs_from_dirspec  = $(wildcard $1)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
__src_to = $(subst .c,$1, $(subst .cpp,$1,$2))
src_to = $(addprefix $(OBJS_DIR)/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1)

#---- Setting ----#
TARGET = \
	$(LIB_DIR)/lib$(COMPONENT_NAME).a

INCLUDES_DIRS_EXPANDED = $(call get_dirs_from_dirspec, $(INC_DIRS))
INCLUDES += $(foreach dir, $(INCLUDES_DIRS_EXPANDED), -I$(dir))

SRCS += $(call get_src_from_dir_list, $(SRC_DIRS)) $(SRC_FILES)
OBJS = $(call src_to_o,$(SRCS))

CPPFLAGS += $(INCLUDES) $(CPPWARNINGFLAGS)
LDFLAGS  = $(LIBRARY_FILES)

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo Building archive $@
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(AR) $(ARFLAGS) $@ $^

$(OBJS_DIR)/%.o: %.cpp
	@echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(COMPILE.cpp) $(DEP_FLAGS) $(OUTPUT_OPTION) $<

$(OBJS_DIR)/%.o: %.c
	@echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(COMPILE.c) $(DEP_FLAGS)  $(OUTPUT_OPTION) $<

clean:
	@echo Making clean
	$(SILENCE)$(RM) $(OBJS) $(TARGET)
	$(SILENCE)rm -rf gcov $(OBJS_DIR)
