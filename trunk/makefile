#configuration file
CONFIG_FILE = ./config/config
MATHLINK_FILE = ./config/MathLink
PARAMETERS_FILE = ./config/params
VNODE_FILE = NULL

# Import configuration file
-include $(CONFIG_FILE)
-include $(VNODE_FILE)
-include $(PARAMETERS_FILE)
-include $(MATHLINK_FILE)

# Override C++ compiler defined in VNODE
CXX = $(COMPILER)

# This project will uses the VNODE-LP library
CXXFLAGS += -I$(ML_DIR)/include \
			-I$(INSTALL_DIR)/include \
			-I$(INSTALL_DIR)/FADBAD++ \
			-I$(HEADDIR)
LDFLAGS  += -L$(ML_DIR)/lib \
			-L$(INSTALL_DIR)/lib 
LIBS := -lvnode $(LIBS) $(MLLIBS) -lgdi32

 
ifeq ($(DEBUG),0)
	LIBS += -mwindows
endif

all : $(CONFIG_FILE)
	make $(BIN)
	@echo $(BIN) plug-in builded with success

$(BIN) : $(OBJ) $(OBJDIR)/f.o
	@echo -e "\033[00mLinking : $@\033[31m"
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o :	$(SRCDIR)/%.c
	@echo -e "\033[00mCompiling : $@\033[31m"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo -e "\033[00mCompiling : $@\033[31m"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

$(SRCDIR)/%tm.c : $(SRCDIR)/%.tm
	@echo -e "\033[00mCompiling : $@\033[31m"
	@$(MPREP) $^ -o $@

.PHONY: clean eraseTemp mrproper


mrproper: clean eraseTemp
	@-$(RM) -f $(BIN).exe
	@-$(RM) -f $(BIN)
	@-$(RM) -f ./config/config
	@echo Configuration file removed

clean:
	@-$(RM) $(OBJ)
	@echo Cleaned

eraseTemp:
	@-$(RM) ./include/*.h~
	@-$(RM) ./src/*.cc~
	@-$(RM) ./src/*.tm~
	@echo Backup editor file erased