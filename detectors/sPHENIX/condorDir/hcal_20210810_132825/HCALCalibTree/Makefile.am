AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include \
  -I$(OPT_SPHENIX)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

include_HEADERS = \
  HCALCalibTree.h \
  HCALCalibTreeLinkDef.h

lib_LTLIBRARIES = \
  libHCALCalibTree.la 

libHCALCalibTree_la_SOURCES = \
  HCALCalibTree.C \
  HCALCalibTree_Dict.C 

libHCALCalibTree_la_LIBADD = \
  -lphool \
  -lg4dst \
  -lfun4all \
  -lSubsysReco \
  -lcalo_io

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals_HCALCalibTree 

testexternals_HCALCalibTree_SOURCES = testexternals.cc

testexternals_HCALCalibTree_LDADD   = \
  libHCALCalibTree.la 

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

%_Dict.C: %.h %LinkDef.h
	rootcint -f $@ -c $(CINTFLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)
