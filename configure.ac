dnl *************************************************************
dnl *
dnl * This is a part of ebotula.
dnl * It is distributed under the GNU General Public License
dnl * See the file COPYING for details.
dnl *
dnl * (c)2003 Steffen Laube <realebula@gmx.de>
dnl *************************************************************
dnl run first:
dnl
dnl aclocal && autoconf && automake -a
dnl
dnl to build the configure script
dnl *************************************************************

AC_PREREQ([2.71])
AC_INIT([ebotula],[0.1.18],[Laube.Steffe@gmx.de])
AM_INIT_AUTOMAKE

AC_USE_SYSTEM_EXTENSIONS

AC_CONFIG_SRCDIR([src/ircbot.c])
AC_CONFIG_HEADERS([src/include/config.h])


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB
AM_PROG_CC_C_O

CFLAGS=`echo $CFLAGS | sed 's/-O4//'`
CFLAGS=`echo $CFLAGS | sed 's/-g//'`

dnl ===========================
dnl checking library
dnl ===========================
AC_ARG_WITH(gdbmlib,
  [  --with-gdbmlib=<path>   path to the gdbm library installation],
  [GDBMLIB_PREFIX=$with_gdbmlib
  AC_SUBST(GDBMLIB_PREFIX)
  GDBM_LIB="-L${GDBMLIB_PREFIX} -lgdbm"],
  [
	GDBM_LIB="-lgdbm"
	AC_CHECK_LIB(gdbm,gdbm_open)
  ]

)
AC_SUBST(GDBM_LIB)

# cunit test
AC_CHECK_LIB(cunit,CU_initialize_registry,
	[CUNIT_LIB="-lcunit"],
	AC_MSG_ERROR("cunit library not found.")
)
AC_SUBST(CUNIT_LIB)

AC_CHECK_LIB(crypt,crypt,
  [CRYPT_LIB="-lcrypt"],
  AC_MSG_ERROR("crypt library not found.")
)
AC_SUBST(CRYPT_LIB)

AC_CHECK_LIB(pthread,pthread_create,
  [PTHREAD_LIB="-lpthread"],
  AC_MSG_ERROR("pthread library not fount.")
)
AC_SUBST(PTHREAD_LIB)


# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_DIRENT

# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

#AC_CHECK_HEADERS([argz.h arpa/inet.h fcntl.h inttypes.h langinfo.h libintl.h limits.h locale.h malloc.h netdb.h netinet/in.h stddef.h stdint.h stdio_ext.h stdlib.h string.h strings.h sys/param.h sys/socket.h termios.h unistd.h syslog.h])
AC_INCLUDES_DEFAULT

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_TYPE_UID_T
AC_C_VOLATILE

# Checks for library functions.
AM_GNU_GETTEXT([external], [need-ngettext])
AM_GNU_GETTEXT_VERSION([0.20])
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CACHE_CHECK([for the type of signal handlers], ac_cv_type_retsigtype, [ ac_cv_type_retsigtype=yes])
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify bzero clock_gettime getcwd gethostbyname gethostname mempcpy memset mkdir munmap nl_langinfo setlocale socket stpcpy strcasecmp strchr strcspn strdup strpbrk strrchr strstr strtoul syslog uname])



dnl ===========================
dnl Path section
dnl ===========================
test "x$prefix" = xNONE && prefix=$ac_default_prefix
test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'

_sysconfdir=`eval echo $sysconfdir`
_sysconfdir="$_sysconfdir/ebotula"

_databasedir=`eval echo $localstatedir`
_databasedir="$_databasedir/ebotula"

_localebasedir=`eval echo $datadir`
_localebasedir="$_localebasedir/locale"

AC_DEFINE_UNQUOTED(CONFDIR,"$_sysconfdir", Define the default path to the configuration file)
AC_DEFINE_UNQUOTED(DATABASEDIR,"$_databasedir", Define the default path to the database files)
AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR,"$_localebasedir", Define the default path to the local files)

dnl ===========================
dnl optional Features
dnl ===========================
debug=no
AC_ARG_ENABLE(debug,[  --enable-debug          Enable debugging],, enable_debug=no)

if test "$enable_debug" = no; then
    AC_DEFINE(NDEBUG,,Undefine if you want create a debugging version)
else
    DFLAGS=-g
    AC_CHECK_LIB(rt,clock_gettime,
      [RT_LIB="-lrt"],
      AC_MSG_ERROR("rt library not fount.")
    )
    AC_SUBST(RT_LIB)
fi
AC_SUBST(DFLAGS)

profile=no
AC_ARG_ENABLE(profile,[  --enable-profile        Enable profiling for debugging],, enable_profile=no)

if test "$enable_profile" = yes; then
    PFLAGS=-pg
fi
AC_SUBST(PFLAGS)

dnl ===========================
dnl end 
dnl ===========================
AC_CONFIG_FILES([Makefile  
		 intl/Makefile 
		 po/Makefile.in
                 ebotula.spec
		 doc/ebotula.1
		 doc/ebotula_db.1
		 doc/ebotula.conf.5
                 doc/Makefile
                 etc/Makefile
                 src/Makefile
                 testsuit/Makefile
		 tools/Makefile])
AC_OUTPUT
