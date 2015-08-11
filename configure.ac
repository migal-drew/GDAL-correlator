#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([GDAL-correlator], [1.0.0], [https://github.com/migal-drew/GDAL-correlator/issues])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT
AC_CONFIG_SRCDIR([src/sources/GDALFeaturePoint.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
])

AC_OUTPUT
