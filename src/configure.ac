#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([SDLCave], [0.1], [yann.benigot@gmail.com])
AC_CONFIG_SRCDIR([src/sdlcave.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(sdlcave, 0.1)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AM_PATH_SDL(1.2.0, :, AC_MSG_ERROR([SDL not found]))
AC_CHECK_LIB(SDL_gfx, stringRGBA, , AC_MSG_ERROR([SDL_gfx not found]))

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
