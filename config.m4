dnl config.m4 for extension indexed_array

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([indexed_array],
dnl   [for indexed_array support],
dnl   [AS_HELP_STRING([--with-indexed_array],
dnl     [Include indexed_array support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([indexed_array],
  [whether to enable indexed_array support],
  [AS_HELP_STRING([--enable-indexed_array],
    [Enable indexed_array support])],
  [no])

if test "$PHP_INDEXED_ARRAY" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, INDEXED_ARRAY_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-indexed_array -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/indexed_array.h"  # you most likely want to change this
  dnl if test -r $PHP_INDEXED_ARRAY/$SEARCH_FOR; then # path given as parameter
  dnl   INDEXED_ARRAY_DIR=$PHP_INDEXED_ARRAY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for indexed_array files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       INDEXED_ARRAY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$INDEXED_ARRAY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the indexed_array distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-indexed_array -> add include path
  dnl PHP_ADD_INCLUDE($INDEXED_ARRAY_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-indexed_array -> check for lib and symbol presence
  dnl LIBNAME=INDEXED_ARRAY # you may want to change this
  dnl LIBSYMBOL=INDEXED_ARRAY # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_INDEXED_ARRAY_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your indexed_array library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $INDEXED_ARRAY_DIR/$PHP_LIBDIR, INDEXED_ARRAY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_INDEXED_ARRAY_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your indexed_array library.])
  dnl ],[
  dnl   -L$INDEXED_ARRAY_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(INDEXED_ARRAY_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_INDEXED_ARRAY, 1, [ Have indexed_array support ])
  PHP_ADD_LIBRARY(stdc++, 1, INDEXED_ARRAY_SHARED_LIBADD)
  PHP_NEW_EXTENSION(indexed_array, indexed_array.cpp, $ext_shared)
fi
