CXX=`root-config --cxx`
CXXFLAGS=`root-config --cflags` -Wall -Werror -O3 -Wextra -Wno-unused-local-typedefs -Wno-deprecated-declarations -std=c++11 -g

ifeq "$(GCCVERSION)" "1"
	CXXFLAGS += -Wno-error=misleading-indentation
endif

define SPHENIXERR
 SPHENIX is not set at all. Please set this environment variable to point to your build - this should be either
export SPHENIX=$(PWD)
or
source setEnv.sh
if you have made appropriate changes.
For more, see README for full setup recommendations
endef

ifndef SPHENIX
$(error "$(SPHENIXERR)")
endif


INCLUDE=-I$(PWD) -I$(SPHENIX)
ROOTLIBS=`root-config --glibs`

MKDIR_LIB=mkdir -p $(SPHENIX)/install/lib
MKDIR_OUTPUT=mkdir -p $(PWD)/output

all: mkdirLib mkdirOutput treeMakerDict.C $(SPHENIX)/install/lib/libTreeMaker.so

mkdirLib:
	$(MKDIR_LIB)

mkdirOutput:
	$(MKDIR_OUTPUT)

treeMakerDict.C: TreeMaker.h Linkdef.h
	@rootcint -f $@ -c $(INCLUDE) -p $^

$(SPHENIX)/install/lib/libTreeMaker.so: treeMakerDict.C TreeMaker.C
	@$(CXX) $(CXXFLAGS) -shared  $(INCLUDE) -o $(SPHENIX)/install/lib/libTreeMaker.so $(ROOTLIBS)


clean:
	rm -f *.so
	rm -f lib/*.so
	rm -rf lib
	rm -f $(SPHENIX)/install/lib/libTreeMaker.so
	rm -f *.o
	rm -f *~
	rm -f *#
	rm -f treeMakerDict.C
	rm -f *.pcm
