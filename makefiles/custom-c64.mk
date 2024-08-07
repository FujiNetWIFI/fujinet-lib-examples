# COMPILE FLAGS

################################################################
# DISK creation

SUFFIX = .prg
DISK_TASKS += .create-d64

DISK_FILE = $(DIST_DIR)/$(PROGRAM).d64

.create-d64:
	@echo "creating $(DISK_FILE) from program $(PROGRAM_TGT)"
	@if [ -f $(DISK_FILE) ] ; then \
	  rm $(DISK_FILE) ; \
	fi ;
	@cc1541 -q -n "fle: $(PROGRAM)" -f "$(PROGRAM)" -P -w $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) -H "created by fenrock" $(DISK_FILE)

################################################################
# TESTING / EMULATOR

c64_EMUCMD    := $(VICE_HOME)/x64sc
c128_EMUCMD   := $(VICE_HOME)/x128
c16_EMUCMD    := $(VICE_HOME)/xvic
pet_EMUCMD    := $(VICE_HOME)/xpet
plus4_EMUCMD  := $(VICE_HOME)/xplus4
vic20_EMUCMD  := $(VICE_HOME)/xvic
#x16_EMUCMD    := $(VICE_HOME)/xplus4 -ramsize 16
cbm510_EMUCMD := $(VICE_HOME)/xcbm5x0
cbm610_EMUCMD := $(VICE_HOME)/xcbm2