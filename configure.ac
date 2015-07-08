AC_INIT(tarantool-base58, version-0.1)
AC_PREFIX_DEFAULT([/usr])

AC_PROG_CC
if test "$ac_cv_prog_cc_c89" == "no" ; then
	AC_MSG_ERROR([c compiler not found])
fi
AC_CHECK_HEADERS([tarantool/lua.h])
if test "$ac_cv_header_tarantool_lua_h" == "no" ; then
	AC_MSG_ERROR([lua.h not found])
fi
AC_CHECK_HEADERS([tarantool/lauxlib.h])
if test "$ac_cv_header_tarantool_luaxlib_h" == "no" ; then
	AC_MSG_ERROR([luaxlib.h not found])
fi
srcdir="src"
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
