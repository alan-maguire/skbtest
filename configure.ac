# Process this file with autoconf to produce a configure script.

# Prelude:
AC_PREREQ([2.59])
AC_INIT([skbtest], [0.1], [myemail@example.com])

# unique source file --- primitive safety check
    AC_CONFIG_SRCDIR([kernel/skbtest.c])

# place to put some extra build scripts installed
AC_CONFIG_AUX_DIR([ac])

# Look for/generate m4 files under top/m4
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Silent rules by default - use make V=1 for verbose
AM_SILENT_RULES([yes])

# Check for dependencies:
AM_LIB_KTF

# List directories containing ktf kernel module source:
# (You can have more than one of these if you have modules in more than one directory:)
AM_KTF_DIR([kernel])
AC_CONFIG_FILES([Makefile
		 kernel/Makefile
])
AC_OUTPUT
