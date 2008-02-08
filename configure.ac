#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
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

PKG_CHECK_MODULES([tsData], [tsData])
METNO_CHECK_LIB([tsData], [main], , , , [$tsData_LIBS], 
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
