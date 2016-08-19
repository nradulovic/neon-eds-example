#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Neon test], [1.0], [nenad.b.radulovic@gmail.com])
AC_CONFIG_SRCDIR([])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])
AM_PATH_PYTHON
LT_INIT

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_LN_S

# Checks for libraries.
# PKG_CHECK_MODULES(XML, libxml-2.0)

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([neon])
AC_OUTPUT


