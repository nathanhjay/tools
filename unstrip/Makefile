#
# A GNU Makefile
#
# This Makefile builds the unstrip tool
#

# Make sure to set the DYNINST_ROOT environment variable to the directory where
# Dyninst is installed. The directory should contain the include and
# $(PLATFORM)/lib directories

# Or uncomment the following line and change the path accordingly
# DYNINST_ROOT = /change/me

ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif


DYNINST_INCLUDE = $(DYNINST_ROOT)/include
DYNINST_LIB =  $(DYNINST_ROOT)/lib

# These should point to where libelf and libdwarf are installed
LOCAL_INC_DIR = /p/paradyn/packages/libdwarf/include
LOCAL_LIBS_DIR = /p/paradyn/packages/libdwarf/lib

CXX = g++
CXXFLAGS = --std=c++11 -g -Wall -D_GLIBCXX_USE_CXX11_ABI=0

all: unstrip 

unstrip: unstrip.o
	$(CXX) $(CXXFLAGS) -L$(DYNINST_LIB) \
		-L$(LOCAL_LIBS_DIR) \
		-ldwarf -lelf \
		-lparseAPI -lsymtabAPI -lsymLite -ldynElf -ldynDwarf -linstructionAPI \
		-lcommon \
		-o unstrip unstrip.o \
		callback.o database.o fingerprint.o semanticDescriptor.o types.o util.o

unstrip-static: unstrip.o
	$(CXX) $(CXXFLAGS) -L$(DYNINST_LIB) \
		-L$(LOCAL_LIBS_DIR) \
		-o unstrip-static unstrip.o \
		callback.o database.o fingerprint.o semanticDescriptor.o types.o util.o \
		-Wl,-Bstatic \
		-lparseAPI_static -lsymtabAPI_static -ldynElf_static -ldynDwarf_static -linstructionAPI_static \
		-ldwarf \
		-lcommon_static \
		-liberty \
		-Wl,-Bdynamic \
		-lelf

unstrip.o: unstrip.C util.o types.o semanticDescriptor.o database.o fingerprint.o callback.o
	$(CXX) $(CXXFLAGS) -I$(LOCAL_INC_DIR) -I$(DYNINST_INCLUDE) -c unstrip.C

fingerprint.o: fingerprint.C database.o
	$(CXX) $(CXXFLAGS) -I$(LOCAL_INC_DIR) -I$(DYNINST_INCLUDE) -c fingerprint.C

database.o: database.C semanticDescriptor.o
	$(CXX) $(CXXFLAGS) -I$(LOCAL_INC_DIR) -I$(DYNINST_INCLUDE) -c database.C 

semanticDescriptor.o: semanticDescriptor.C types.o util.o 
	$(CXX) $(CXXFLAGS) -I$(LOCAL_INC_DIR) -I$(DYNINST_INCLUDE) -c semanticDescriptor.C

types.o: types.C 
	$(CXX) $(CXXFLAGS) -I$(LOCAL_INC_DIR) -I$(DYNINST_INCLUDE) -c types.C

callback.o: callback.C util.o
	$(CXX) $(CXXFLAGS) -I$(LOCAL_INC_DIR) -I$(DYNINST_INCLUDE) -c callback.C

util.o: util.C types.o
	$(CXX) $(CXXFLAGS) -I$(LOCAL_INC_DIR) -I$(DYNINST_INCLUDE) -c util.C

clean: 
	rm -f unstrip unstrip-static *.o
