# ======================================================================
# This is the Matrix configuration input file
# Process this file with autoconf to produce a configure script.
# ======================================================================


# ----------------------------------------------------------------------
# Identify the package and initialize the autotools:
# ----------------------------------------------------------------------

AC_PREREQ(2.59)
AC_INIT(CLHEP Matrix, 2.1.0.1, http://savannah.cern.ch/projects/clhep/, Matrix)
AC_CONFIG_AUX_DIR(autotools)
AC_CANONICAL_TARGET
AC_CONFIG_SRCDIR([src/DiagMatrix.cc])
AM_INIT_AUTOMAKE(1.9 foreign)
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB

# ----------------------------------------------------------------------
# --enable-shared and --enable-static
# both shared and static libraries are built by default
# ----------------------------------------------------------------------

AC_ARG_ENABLE(shared,
   AC_HELP_STRING([--disable-shared],[do not build shared libraries]),
        [case "${enableval}" in
          yes) build_shared="yes";;
          no)  build_shared="no";;
          *)   build_shared="yes";;
        esac],
        [build_shared="yes"])
AC_ARG_ENABLE(static,
   AC_HELP_STRING([--disable-static],[do not build static libraries]),
        [case "${enableval}" in
          yes) build_static="yes";;
          no)  build_static="no";;
          *)   build_static="yes";;
        esac],
        [build_static="yes"])
AC_ARG_ENABLE(visual,
   AC_HELP_STRING([--enable-visual],[on by default when using Visual C++]),
        [case "${enableval}" in
          yes) build_visual="yes";;
          no)  build_visual="no";;
          *)   build_visual="no";;
        esac],
        [build_visual="no"])

AC_ARG_ENABLE(gmake,
   AC_HELP_STRING([--enable-gmake],[use gmake (default is make)]),
        [case "${enableval}" in
          yes) use_gmake="yes";;
          no)  use_gmake="no";;
          *)   use_gmake="no";;
        esac],
        [use_gmake="no"])

# ----------------------------------------------------------------------
# Identify the files that 'configure' is to produce:
# ----------------------------------------------------------------------

# Header containing #defines:
AM_CONFIG_HEADER([Matrix/defs.h])

# Makefiles:
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([Matrix/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([test/Makefile])
AC_CONFIG_FILES([Matrix-deps])
AC_CONFIG_FILES([Matrix/copy-header.pl], [chmod +x Matrix/copy-header.pl])

# Configuration inquiry program/script:
AC_CONFIG_FILES([Matrix-config], [chmod +x Matrix-config])

# Test driver program/script:
AC_CONFIG_FILES([test/testMatrix.sh], [chmod +x test/testMatrix.sh])
AC_CONFIG_FILES([test/testInversion.sh], [chmod +x test/testInversion.sh])
AC_CONFIG_FILES([test/testBug6176.sh], [chmod +x test/testBug6176.sh])

# ----------------------------------------------------------------------
# Capture post-install information:
# ----------------------------------------------------------------------

Matrix_CPPFLAGS="-I$includedir"
Matrix_LDFLAGS="-L$libdir"
Matrix_LIBS="-lCLHEP-$PACKAGE-$VERSION -lCLHEP-Random-$VERSION -lCLHEP-Vector-$VERSION"

AC_SUBST(Matrix_CPPFLAGS)
AC_SUBST(Matrix_LIBS)
AC_SUBST(Matrix_LDFLAGS)

# ----------------------------------------------------------------------
# Supply boilerplate for Matrix/defs.h source header:
# ----------------------------------------------------------------------

AH_TOP([#ifndef MATRIX_DEFS_H
#define MATRIX_DEFS_H])

## the undefs are converted by configure
AH_VERBATIM([PACKAGE],[/* Name of package */
#ifndef PACKAGE
#undef PACKAGE
#endif])
AH_VERBATIM([PACKAGE_BUGREPORT],[/* Define to the address where bug reports for this package should be sent. */
#ifndef PACKAGE_BUGREPORT
#undef PACKAGE_BUGREPORT
#endif])
AH_VERBATIM([PACKAGE_NAME],[/* Define to the full name of this package. */
#ifndef PACKAGE_NAME
#undef PACKAGE_NAME
#endif])
AH_VERBATIM([PACKAGE_STRING],[/* Define to the full name and version of this package. */
#ifndef PACKAGE_STRING
#undef PACKAGE_STRING
#endif])
AH_VERBATIM([PACKAGE_TARNAME],[/* Define to the one symbol short name of this package. */
#ifndef PACKAGE_TARNAME
#undef PACKAGE_TARNAME
#endif])
AH_VERBATIM([PACKAGE_VERSION],[/* Define to the version of this package. */
#ifndef PACKAGE_VERSION
#undef PACKAGE_VERSION
#endif])
AH_VERBATIM([VERSION],[/* Version number of package */
#ifndef VERSION
#undef VERSION
#endif])

## disable allocator until it is fixed
AH_VERBATIM([DISABLE_ALLOC],[/* disable non-compliant allocator */
#ifndef DISABLE_ALLOC
#define DISABLE_ALLOC
#endif])

AH_BOTTOM([#endif  // MATRIX_DEFS_H])


# ----------------------------------------------------------------------
# Account for any user configuration options:
# ----------------------------------------------------------------------


# ----------------------------------------------------------------------
# Check for needed programs:
# ----------------------------------------------------------------------

# Locate a C++ compiler:
AC_PROG_CXX(cl g++ g++4 c++ icc aCC CC cxx cc++ FCC RCC xlC_r xlC gpp)

# Use it hereinafter:
AC_LANG(C++)

# Ensure we've found a preprocessor:
AC_REQUIRE_CPP

# copy and diff
case "$target" in
*-*-win32*)
  COPY_P="copy -p";DIFF_Q="diff -q -b"
  ;;
*-*-cygwin*)
  COPY_P="cp -p";DIFF_Q="diff -q -b"
  ;;
*-*-solaris*)
  COPY_P="cp -p";DIFF_Q="diff -b"
  use_gmake="yes"
  ;;
*-*-linux*)
  COPY_P="cp -p";DIFF_Q="diff -q -b"
  ;;
*)
  COPY_P="cp -p";DIFF_Q="diff -q -b"
esac

AM_CONDITIONAL(USE_GMAKE, test x$use_gmake = xyes)

AC_SUBST(COPY_P)
AC_SUBST(DIFF_Q)

# worry about compiler flags
case "$CXX" in
g++) 
   case "$target" in
   *-*-linux*) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall -D_GNU_SOURCE"
      MY_SHFLAGS="-fPIC -DPIC"
      MY_SHLINK="-shared -Wl,--no-undefined"
      MY_SHNAME="-Wl,-soname,"
      MY_LD="-Wl,--rpath -Wl,"
      SHEXT=so
      AR="ar"; ARFLAGS="cru"
      LEAKTEST="testBug7328"
      ;;
   *-*-darwin*) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall -D_GNU_SOURCE"
      MY_SHFLAGS=""; MY_SHLINK="-dynamiclib -single_module -undefined dynamic_lookup"; SHEXT=dylib
      MY_SHNAME="-Wl,-install_name,"; MY_LD="-bind_at_load -Wl,--rpath -Wl,"
      AR="ar"; ARFLAGS="cru"
      LEAKTEST=""
      ;;
   *-*-cygwin*)
      AM_CXXFLAGS="-O -ansi -pedantic -Wall"
      MY_SHFLAGS="-DPIC"; SHEXT=dll
      MY_SHLINK="-shared -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--whole-archive"
      MY_SHLINKEXTRA="-Wl,--no-whole-archive"
      MY_SHNAME="-Wl,-out-implib="; MY_LD="-Wl,--rpath -Wl,"
      AR="ar"; ARFLAGS="cru"
      build_static="yes"
      build_shared="no"
      LEAKTEST="testBug7328"
      ;;
   *) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall"
      MY_SHFLAGS="-fPIC -DPIC"
      MY_SHLINK="-shared -Wl,--no-undefined"
      MY_SHNAME="-Wl,-soname,"
      MY_LD="-Wl,--rpath -Wl,"
      SHEXT=so
      AR="ar"; ARFLAGS="cru"
      LEAKTEST="testBug7328"
   esac;;
g++4) 
   case "$target" in
   *-*-linux*) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall -D_GNU_SOURCE"
      MY_SHFLAGS="-fPIC -DPIC"
      MY_SHLINK="-shared -Wl,--no-undefined"
      MY_SHNAME="-Wl,-soname,"
      MY_LD="-Wl,--rpath -Wl,"
      SHEXT=so
      AR="ar"; ARFLAGS="cru"
      LEAKTEST="testBug7328"
      ;;
   *) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall"
      MY_SHFLAGS="-fPIC -DPIC"
      MY_SHLINK="-shared -Wl,--no-undefined"
      MY_SHNAME="-Wl,-soname,"
      MY_LD="-Wl,--rpath -Wl,"
      SHEXT=so
      AR="ar"; ARFLAGS="cru"
      LEAKTEST=""
   esac;;
c++) 
   case "$target" in
   *-*-linux*) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall -D_GNU_SOURCE"
      MY_SHFLAGS="-fPIC -DPIC"
      MY_SHLINK="-shared -Wl,--no-undefined"
      MY_SHNAME="-Wl,-soname,"
      MY_LD="-Wl,--rpath -Wl,"
      SHEXT=so
      AR="ar"; ARFLAGS="cru"
      LEAKTEST="testBug7328"
      ;;
   *-*-darwin*) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall -D_GNU_SOURCE"
      MY_SHFLAGS=""; MY_SHLINK="-dynamiclib -single_module -undefined dynamic_lookup"; SHEXT=dylib
      MY_SHNAME="-Wl,-install_name,"; MY_LD="-bind_at_load -Wl,--rpath -Wl,"
      AR="ar"; ARFLAGS="cru"
      LEAKTEST=""
      ;;
   *-*-cygwin*)
      AM_CXXFLAGS="-O -ansi -pedantic -Wall"
      MY_SHFLAGS="-DPIC"; SHEXT=dll
      MY_SHLINK="-shared -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--whole-archive"
      MY_SHLINKEXTRA="-Wl,--no-whole-archive"
      MY_SHNAME="-Wl,-out-implib="; MY_LD="-Wl,--rpath -Wl,"
      AR="ar"; ARFLAGS="cru"
      build_static="yes"
      build_shared="no"
      LEAKTEST="testBug7328"
      ;;
   *) 
      AM_CXXFLAGS="-O -ansi -pedantic -Wall"
      MY_SHFLAGS="-fPIC -DPIC"
      MY_SHLINK="-shared -Wl,--no-undefined"
      MY_SHNAME="-Wl,-soname,"
      MY_LD="-Wl,--rpath -Wl,"
      SHEXT=so
      AR="ar"; ARFLAGS="cru"
      LEAKTEST="testBug7328"
   esac;;
icc)
   case "$target" in
   *-*-linux*)
      AM_CXXFLAGS="-O -ansi -fp-model precise -no-gcc -w1"
      AM_LDFLAGS="-lstdc++"
      MY_SHFLAGS="-fPIC -DPIC"; MY_SHLINK="-shared"; SHEXT=so
      MY_SHNAME="-Wl,-soname,"; MY_LD="-Wl,--rpath -Wl,"
      AR="xiar"; ARFLAGS="cru"
      LEAKTEST="testBug7328"
      ;;
   *)
      AM_CXXFLAGS="-O -ansi -fp-model precise -no-gcc -w1"
      AM_LDFLAGS="-lstdc++"
      MY_SHFLAGS="-fPIC -DPIC"; MY_SHLINK="-shared"; SHEXT=so
      MY_SHNAME="-Wl,-soname,"; MY_LD="-Wl,--rpath -Wl,"
      AR="xiar"; ARFLAGS="cru"
      LEAKTEST="testBug7328"
   esac;;
cl)  
   AM_CXXFLAGS="-EHsc -nologo -GR -MD"
   AR="ar"; ARFLAGS="cru"
   MY_SHFLAGS=" "; MY_SHLINK="lib /NOLOGO"; SHEXT=lib
   MY_SHNAME="/OUT:"; MY_LD=" "
   build_static="no"
   build_shared="yes"
   build_visual="yes"
   CXXFLAGS=" "
   AC_SUBST(CXXFLAGS)
   LEAKTEST="testBug7328"
   ;;
CC)  
   case "$target" in
   *-*-solaris*) 
      AM_CXXFLAGS="-O"
      MY_SHFLAGS="-KPIC -DPIC"; MY_SHLINK="-G"; SHEXT=so
      MY_SHNAME="-h"; MY_LD="-R"
      AM_LDFLAGS="-lsunmath"
      AR="CC"; ARFLAGS="-xar -o"
      build_shared="no"
      LEAKTEST=""
      ;;
   *-*-hpux*) AM_CXXFLAGS="+O3 +DAportable +a1 -z -pta +Onolimit"
              AR="ar"; ARFLAGS="cru"
              build_shared="no"
              LEAKTEST=""
	      ;;
   *-*-irix*) AM_CXXFLAGS="-O -OPT:Olimit=0 -pta"
              AR="ar"; ARFLAGS="cru"
              LEAKTEST=""
              build_shared="no"
	      ;;
   *)   echo UNEXPECTED CHOICE OF OPERATING SYSTEM FOR $CXX: $target
   esac;;
aCC) 
   AM_CXXFLAGS="-O -Aa +DAportable +Onolimit"
   AR="ar"; ARFLAGS="cru"
   build_shared="no"
   LEAKTEST=""
   ;;
cxx) 
   AM_CXXFLAGS="-O -std strict_ansi -timplicit_local"
   AR="ar"; ARFLAGS="cru"
   build_shared="no"
   LEAKTEST=""
   ;;
xlC) 
   AM_CXXFLAGS="-O3 -qtwolink -+"
   AR="ar"; ARFLAGS="cru"
   build_shared="no"
   LEAKTEST=""
   ;;
*)   echo UNEXPECTED CHOICE OF C++ COMPILER: $CXX
esac

AM_CONDITIONAL(BUILD_SHARED, test x$build_shared = xyes)
AM_CONDITIONAL(BUILD_STATIC, test x$build_static = xyes)
AM_CONDITIONAL(BUILD_VISUAL, test x$build_visual = xyes)

AC_SUBST(AM_CXXFLAGS)
AC_SUBST(AM_LDFLAGS)
AC_SUBST(MY_SHFLAGS)
AC_SUBST(MY_SHLINK)
AC_SUBST(MY_SHNAME)
AC_SUBST(MY_LD)
AC_SUBST(SHEXT)
AC_SUBST(AR)
AC_SUBST(ARFLAGS)
AC_SUBST(LEAKTEST)


# ----------------------------------------------------------------------
# Check for supported link flags
# ----------------------------------------------------------------------

AC_CHECK_LIB([c -Wl,--as-needed],open,[MY_SHEXTRA="-Wl,--as-needed"])
AC_SUBST(MY_SHEXTRA)

# ----------------------------------------------------------------------
# Set system-dependent options:
# ----------------------------------------------------------------------


# ----------------------------------------------------------------------
# Check for needed libraries:
# ----------------------------------------------------------------------


# ----------------------------------------------------------------------
# Check for needed header files:
# ----------------------------------------------------------------------


# ----------------------------------------------------------------------
# Check for needed typedefs, structures, and compiler characteristics:
# ----------------------------------------------------------------------


# ----------------------------------------------------------------------
# Check for needed library functions:
# ----------------------------------------------------------------------


# ----------------------------------------------------------------------
# Finish up:
# ----------------------------------------------------------------------

AC_OUTPUT


# ======================================================================
