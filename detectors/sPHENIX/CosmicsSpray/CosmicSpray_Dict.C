// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CosmicSpray_Dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "CosmicSpray.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *CosmicSpray_Dictionary();
   static void CosmicSpray_TClassManip(TClass*);
   static void delete_CosmicSpray(void *p);
   static void deleteArray_CosmicSpray(void *p);
   static void destruct_CosmicSpray(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CosmicSpray*)
   {
      ::CosmicSpray *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CosmicSpray));
      static ::ROOT::TGenericClassInfo 
         instance("CosmicSpray", "CosmicSpray.h", 19,
                  typeid(::CosmicSpray), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CosmicSpray_Dictionary, isa_proxy, 1,
                  sizeof(::CosmicSpray) );
      instance.SetDelete(&delete_CosmicSpray);
      instance.SetDeleteArray(&deleteArray_CosmicSpray);
      instance.SetDestructor(&destruct_CosmicSpray);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CosmicSpray*)
   {
      return GenerateInitInstanceLocal((::CosmicSpray*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CosmicSpray*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CosmicSpray_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CosmicSpray*)0x0)->GetClass();
      CosmicSpray_TClassManip(theClass);
   return theClass;
   }

   static void CosmicSpray_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CosmicSpray(void *p) {
      delete ((::CosmicSpray*)p);
   }
   static void deleteArray_CosmicSpray(void *p) {
      delete [] ((::CosmicSpray*)p);
   }
   static void destruct_CosmicSpray(void *p) {
      typedef ::CosmicSpray current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CosmicSpray

namespace {
  void TriggerDictionaryInitialization_CosmicSpray_Dict_Impl() {
    static const char* headers[] = {
"CosmicSpray.h",
0
    };
    static const char* includePaths[] = {
"/sphenix/user/dlis/Projects/install/include",
"/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/release/release_new/new.4/include",
"/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/root-6.22.02/include",
"/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/include",
"/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/root-6.22.02/include/",
"/gpfs/mnt/gpfs02/sphenix/user/dlis/Projects/HCALSims/macros/detectors/sPHENIX/CosmicsSpray/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "CosmicSpray_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CosmicSpray.h")))  CosmicSpray;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CosmicSpray_Dict dictionary payload"

#ifndef PACKAGE_NAME
  #define PACKAGE_NAME ""
#endif
#ifndef PACKAGE_TARNAME
  #define PACKAGE_TARNAME ""
#endif
#ifndef PACKAGE_VERSION
  #define PACKAGE_VERSION ""
#endif
#ifndef PACKAGE_STRING
  #define PACKAGE_STRING ""
#endif
#ifndef PACKAGE_BUGREPORT
  #define PACKAGE_BUGREPORT ""
#endif
#ifndef PACKAGE_URL
  #define PACKAGE_URL ""
#endif
#ifndef PACKAGE
  #define PACKAGE "g4picoDst"
#endif
#ifndef VERSION
  #define VERSION "1.00"
#endif
#ifndef STDC_HEADERS
  #define STDC_HEADERS 1
#endif
#ifndef HAVE_SYS_TYPES_H
  #define HAVE_SYS_TYPES_H 1
#endif
#ifndef HAVE_SYS_STAT_H
  #define HAVE_SYS_STAT_H 1
#endif
#ifndef HAVE_STDLIB_H
  #define HAVE_STDLIB_H 1
#endif
#ifndef HAVE_STRING_H
  #define HAVE_STRING_H 1
#endif
#ifndef HAVE_MEMORY_H
  #define HAVE_MEMORY_H 1
#endif
#ifndef HAVE_STRINGS_H
  #define HAVE_STRINGS_H 1
#endif
#ifndef HAVE_INTTYPES_H
  #define HAVE_INTTYPES_H 1
#endif
#ifndef HAVE_STDINT_H
  #define HAVE_STDINT_H 1
#endif
#ifndef HAVE_UNISTD_H
  #define HAVE_UNISTD_H 1
#endif
#ifndef HAVE_DLFCN_H
  #define HAVE_DLFCN_H 1
#endif
#ifndef LT_OBJDIR
  #define LT_OBJDIR ".libs/"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CosmicSpray.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CosmicSpray", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CosmicSpray_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CosmicSpray_Dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CosmicSpray_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CosmicSpray_Dict() {
  TriggerDictionaryInitialization_CosmicSpray_Dict_Impl();
}
