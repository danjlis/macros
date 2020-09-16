AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include \
  -I$(OPT_SPHENIX)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

pkginclude_HEADERS = \
  TreeMaker.h \
  TreeMakerLinkDef.h

lib_LTLIBRARIES = \
  libTreeMaker.la

libTreeMaker_la_SOURCES = \
  TreeMaker.C \
  TreeMaker_Dict.C

libTreeMaker_la_LIBADD = \
  -lboost_filesystem \
  -lFROG \
  -lffaobjects \
  -lphool \
  -lfun4all \
  -lSubsysReco \
  -lcalo_util \
  -lcalo_io \
  -lphg4hit \
  -lg4dst	

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals_TreeMaker

testexternals_TreeMaker_SOURCES = testexternals.cc
testexternals_TreeMaker_LDADD   = libTreeMaker.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

%_Dict.C: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)
