include $(top_srcdir)/makerules

    
noinst_libdir = $(sqrat_builddir)
noinst_bindir = $(sqrat_builddir)
ORIGPATH=$(sqrat_srcdir)

BUILT_SOURCES = scripts
CLEANFILES = scripts
scripts: Makefile
	-ln -s $(ORIGPATH)/sqrattest/scripts $(sqrat_builddir)
    
nobase_include_HEADERS = $(ORIGPATH)/include/sqmodule.h\
    $(ORIGPATH)/include/sqrat.h $(ORIGPATH)/include/sqratimport.h\
    $(ORIGPATH)/include/sqrat/sqratAllocator.h\
    $(ORIGPATH)/include/sqrat/sqratArray.h\
    $(ORIGPATH)/include/sqrat/sqratClass.h\
    $(ORIGPATH)/include/sqrat/sqratClassType.h\
    $(ORIGPATH)/include/sqrat/sqratConst.h\
    $(ORIGPATH)/include/sqrat/sqratFunction.h\
    $(ORIGPATH)/include/sqrat/sqratGlobalMethods.h\
    $(ORIGPATH)/include/sqrat/sqratMemberMethods.h\
    $(ORIGPATH)/include/sqrat/sqratObject.h\
    $(ORIGPATH)/include/sqrat/sqratOverloadMethods.h\
    $(ORIGPATH)/include/sqrat/sqratScript.h\
    $(ORIGPATH)/include/sqrat/sqratTable.h\
    $(ORIGPATH)/include/sqrat/sqratTypes.h\
    $(ORIGPATH)/include/sqrat/sqratUtil.h\
    $(ORIGPATH)/include/sqrat/sqratVM.h
    

TESTS = import_test \
    class_binding class_properties const_bindings function_overload\
    script_loading squirrel_functions table_binding function_params
    
noinst_PROGRAMS = sq_interp import_test \
    class_binding class_properties const_bindings function_overload\
    script_loading squirrel_functions table_binding function_params

sq_interp_SOURCES = $(sqrat_srcdir)/sq/sq.c
sq_interp_LDADD = -L$(sqrat_builddir) -lsqratimport $(LDADD) -ldl

noinst_LIBRARIES = libgtest.a libsqratimport.a libsqrattestmain.a
libgtest_a_SOURCES = $(ORIGPATH)/gtest-1.3.0/src/gtest-all.cc
libgtest_a_CXXFLAGS = -I$(ORIGPATH)/gtest-1.3.0/ -I$(ORIGPATH)/gtest-1.3.0/include/

libsqratimport_a_SOURCES = $(ORIGPATH)/sqimport/sqratimport.cpp
libsqrattestmain_a_SOURCES = $(ORIGPATH)/sqrattest/Main.cpp $(ORIGPATH)/sqrattest/Vector.cpp
libsqrattestmain_a_CXXFLAGS = -I$(ORIGPATH)/gtest-1.3.0/include/ -I$(ORIGPATH)/sqrattest $(AM_CXXFLAGS)

import_test_SOURCES = $(sqrat_srcdir)/sqrattest/ImportTest.cpp 
import_test_CXXFLAGS = -I$(ORIGPATH)/gtest-1.3.0/ -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
import_test_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest -lsqratimport $(LDADD) -ldl


class_binding_SOURCES = $(sqrat_srcdir)/sqrattest/ClassBinding.cpp 
class_binding_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
class_binding_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 

class_properties_SOURCES = $(sqrat_srcdir)/sqrattest/ClassProperties.cpp 
class_properties_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
class_properties_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 

const_bindings_SOURCES = $(sqrat_srcdir)/sqrattest/ConstBindings.cpp 
const_bindings_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
const_bindings_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 

function_overload_SOURCES = $(sqrat_srcdir)/sqrattest/FunctionOverload.cpp 
function_overload_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
function_overload_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 

script_loading_SOURCES = $(sqrat_srcdir)/sqrattest/ScriptLoading.cpp 
script_loading_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
script_loading_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 

squirrel_functions_SOURCES = $(sqrat_srcdir)/sqrattest/SquirrelFunctions.cpp 
squirrel_functions_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
squirrel_functions_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 

table_binding_SOURCES = $(sqrat_srcdir)/sqrattest/TableBinding.cpp 
table_binding_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
table_binding_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 

function_params_SOURCES = $(sqrat_srcdir)/sqrattest/FunctionParams.cpp 
function_params_CXXFLAGS = -I$(ORIGPATH)/sqrattest -I$(ORIGPATH)/gtest-1.3.0/include/ $(AM_CXXFLAGS)
function_params_LDADD = -L$(sqrat_builddir) -lsqrattestmain -lgtest $(LDADD) 



