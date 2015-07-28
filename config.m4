dnl $Id$
dnl config.m4 for extension ozglib

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(ozglib, for ozglib support,
dnl Make sure that the comment is aligned:
dnl [  --with-ozglib             Include ozglib support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(ozglib, whether to enable ozglib support,
dnl Make sure that the comment is aligned:
dnl [  --enable-ozglib           Enable ozglib support])

if test "$PHP_OZGLIB" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-ozglib -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/ozglib.h"  # you most likely want to change this
  dnl if test -r $PHP_OZGLIB/$SEARCH_FOR; then # path given as parameter
  dnl   OZGLIB_DIR=$PHP_OZGLIB
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for ozglib files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       OZGLIB_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$OZGLIB_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the ozglib distribution])
  dnl fi

  dnl # --with-ozglib -> add include path
  dnl PHP_ADD_INCLUDE($OZGLIB_DIR/include)

  dnl # --with-ozglib -> check for lib and symbol presence
  dnl LIBNAME=ozglib # you may want to change this
  dnl LIBSYMBOL=ozglib # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OZGLIB_DIR/$PHP_LIBDIR, OZGLIB_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_OZGLIBLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong ozglib lib version or lib not found])
  dnl ],[
  dnl   -L$OZGLIB_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(OZGLIB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ozglib, ozglib.c, $ext_shared)
fi
