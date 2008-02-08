#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([pets2], [4.0.0], [diana@met.no])
AM_INIT_AUTOMAKE
METNO_SET_VERSION_INFO

AC_CONFIG_SRCDIR([src/ptDiagram.cc])
AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
#AC_PROG_SED

AC_LANG_PUSH(C++)

# Checks for libraries.
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL

#METNO_HAVE_OPENGL
METNO_HAVE_GL

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$prefix/lib/pkgconfig

# Check for the puMet library.
PKG_CHECK_MODULES([puMet], [pumet])
METNO_CHECK_LIB([puMet], [main], , , , [$puMet_LIBS], 
                [the puMet package])
METNO_CHECK_HEADERS([puMet/symbolMaker.h], , , ,
                    [$puMet_CFLAGS], [the puMet package])

# Check for the puCtools library.
PKG_CHECK_MODULES([puCtools], [puctools])
METNO_CHECK_LIB([puctools], [main], , , , [$puCtools_LIBS], 
                [the puCtools package])
METNO_CHECK_HEADERS([puCtools/puMath.h], , , ,
                    [$puCtools_CFLAGS], [the puCtools package])

# Check for the puTools library.
PKG_CHECK_MODULES([puTools], [putools])
METNO_CHECK_LIB([putools], [main], , , , [$puTools_LIBS], 
                [the puTools package])
METNO_CHECK_HEADERS([puTools/miString.h], , , ,
                    [$puTools_CFLAGS], [the puTools package])

# Check for the puDatatypes library.
PKG_CHECK_MODULES([puDatatypes], [pudatatypes])
METNO_CHECK_LIB([pudatatypes], [main], , , , [$puDatatypes_LIBS], 
                [the puDatatypes package])
METNO_CHECK_HEADERS([puDatatypes/miPosition.h], , , ,
                    [$puDatatypes_CFLAGS], [the puDatatypes package])

# Check for the glp library.
PKG_CHECK_MODULES([glp], [glp])
METNO_CHECK_LIB([glp], [main], , , , [$glp_LIBS], 
                [the glp package])
METNO_CHECK_HEADERS([glp/GLP.h glp/glpfile.h], , , ,
                    [$glp_CFLAGS], [the glp package])

# Check for the glText library.
PKG_CHECK_MODULES([glText], [gltext])
METNO_CHECK_LIB([glText], [main], , , , [$glText_LIBS], 
                [the glText package])
METNO_CHECK_HEADERS([glText/glText.h], , , ,
                    [$glText_CFLAGS], [the glText package])

# Check for the parameter library.
PKG_CHECK_MODULES([parameter], [parameter])
METNO_CHECK_LIB([parameter], [main], , , , [$parameter_LIBS], 
                [the parameter package])
METNO_CHECK_HEADERS([parameter/parameter.h], , , ,
                    [$parameter_CFLAGS], [the parameter package])

# Check for the tsData library
PKG_CHECK_MODULES([tsData], [tsdata])
METNO_CHECK_LIB([tsdata], [main], , , , [$tsData_LIBS], 
                [the tsData package])
METNO_CHECK_HEADERS([tsData/ptDiagramData.h tsData/ptWeatherParameter.h], , , ,
                    [$tsData_CFLAGS], [the tsData package])

# Checks for header files.
AC_PATH_X
#AC_HEADER_STDC
#AC_CHECK_HEADERS([float.h limits.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
#AC_STRUCT_TM

# Checks for library functions.
AC_LANG_POP(C++)

#making our own .pc file, checking that it is ok and forfilled
AC_CONFIG_FILES(pets2.pc)
AC_OUTPUT

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:.
PKG_CHECK_MODULES([own_dependencies], [${PACKAGE_TARNAME}])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile])
AC_OUTPUT

#AC_CHECK_FUNCS([rint sqrt strdup])
