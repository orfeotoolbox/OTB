AM_CPPFLAGS = -I$(top_srcdir)/third_party/zlib-1.2.3/contrib \
	      -I$(top_srcdir)/third_party/uriparser-0.7.1/include

lib_LTLIBRARIES = libminizip.la liburiparser.la

libminizip_la_SOURCES = \
	zlib-1.2.3/contrib/minizip/unzip.c \
	zlib-1.2.3/contrib/minizip/ioapi.c \
	zlib-1.2.3/contrib/minizip/zip.c
libminizip_la_LDFLAGS = -lz

liburiparser_la_SOURCES = \
	uriparser-0.7.1/lib/UriCommon.c \
	uriparser-0.7.1/lib/UriCommon.h \
	uriparser-0.7.1/lib/UriCompare.c \
	uriparser-0.7.1/lib/UriEscape.c \
	uriparser-0.7.1/lib/UriFile.c \
	uriparser-0.7.1/lib/UriIp4.c \
	uriparser-0.7.1/lib/UriIp4Base.c \
	uriparser-0.7.1/lib/UriIp4Base.h \
	uriparser-0.7.1/lib/UriNormalize.c \
	uriparser-0.7.1/lib/UriNormalizeBase.c \
	uriparser-0.7.1/lib/UriNormalizeBase.h \
	uriparser-0.7.1/lib/UriParse.c \
	uriparser-0.7.1/lib/UriParseBase.c \
	uriparser-0.7.1/lib/UriParseBase.h \
	uriparser-0.7.1/lib/UriQuery.c \
	uriparser-0.7.1/lib/UriRecompose.c \
	uriparser-0.7.1/lib/UriResolve.c \
	uriparser-0.7.1/lib/UriShorten.c
uriparser = uriparser-0.7.1
uriparserinc = $(includedir)/kml/third_party/$(boost)/include
liburiparserincludedir = $(uriparserinc)
liburiparserinclude_HEADERS = \
	$(uriparser)/include/uriparser/Uri.h \
	$(uriparser)/include/uriparser/UriBase.h \
	$(uriparser)/include/uriparser/UriDefsAnsi.h \
	$(uriparser)/include/uriparser/UriDefsConfig.h \
	$(uriparser)/include/uriparser/UriDefsUnicode.h \
	$(uriparser)/include/uriparser/UriIp4.h
liburiparser_la_LDFLAGS = -version-info 1:5:0

boost = boost_1_34_1/boost
boostinc = $(includedir)/kml/third_party/$(boost)
libboostincludedir = $(boostinc)
libboostinclude_HEADERS = \
	${boost}/assert.hpp \
	${boost}/checked_delete.hpp \
	${boost}/config.hpp \
	${boost}/intrusive_ptr.hpp \
	${boost}/scoped_ptr.hpp

libboostdetailincludedir = $(boostinc)/detail
libboostdetailinclude_HEADERS = \
	$(boost)/detail/workaround.hpp

libboostconfigincludedir = $(boostinc)/config
libboostconfiginclude_HEADERS = \
	$(boost)/config/posix_features.hpp \
	$(boost)/config/select_compiler_config.hpp \
	$(boost)/config/select_stdlib_config.hpp \
	$(boost)/config/select_platform_config.hpp \
	$(boost)/config/suffix.hpp \
	$(boost)/config/user.hpp

libboostconfigcompilerincludedir = $(boostinc)/config/compiler
libboostconfigcompilerinclude_HEADERS = \
	$(boost)/config/compiler/gcc.hpp
	
libboostconfigno_tr1includedir = $(boostinc)/config/no_tr1
libboostconfigno_tr1include_HEADERS = \
	$(boost)/config/no_tr1/utility.hpp
	
libboostconfigstdlibincludedir = $(boostinc)/config/stdlib
libboostconfigstdlibinclude_HEADERS = \
	$(boost)/config/stdlib/libstdcpp3.hpp

libboostconfigplatformincludedir = $(boostinc)/config/platform
libboostconfigplatforminclude_HEADERS = \
	$(boost)/config/platform/linux.hpp \
	$(boost)/config/platform/macos.hpp

EXTRA_DIST = \
	$(boost)/README \
	$(boost)/config/abi_prefix.hpp \
	$(boost)/config/abi_suffix.hpp \
	$(boost)/config/auto_link.hpp \
	$(boost)/config/abi/borland_prefix.hpp \
	$(boost)/config/abi/borland_suffix.hpp \
	$(boost)/config/abi/msvc_prefix.hpp \
	$(boost)/config/abi/msvc_suffix.hpp \
	$(boost)/config/compiler/borland.hpp \
	$(boost)/config/compiler/comeau.hpp \
	$(boost)/config/compiler/common_edg.hpp \
	$(boost)/config/compiler/compaq_cxx.hpp \
	$(boost)/config/compiler/digitalmars.hpp \
	$(boost)/config/compiler/gcc_xml.hpp \
	$(boost)/config/compiler/greenhills.hpp \
	$(boost)/config/compiler/hp_acc.hpp \
	$(boost)/config/compiler/intel.hpp \
	$(boost)/config/compiler/kai.hpp \
	$(boost)/config/compiler/metrowerks.hpp \
	$(boost)/config/compiler/mpw.hpp \
	$(boost)/config/compiler/sgi_mipspro.hpp \
	$(boost)/config/compiler/sunpro_cc.hpp \
	$(boost)/config/compiler/vacpp.hpp \
	$(boost)/config/compiler/visualc.hpp \
	$(boost)/config/no_tr1/complex.hpp \
	$(boost)/config/no_tr1/functional.hpp \
	$(boost)/config/no_tr1/memory.hpp \
	$(boost)/config/platform/aix.hpp \
	$(boost)/config/platform/amigaos.hpp \
	$(boost)/config/platform/beos.hpp \
	$(boost)/config/platform/bsd.hpp \
	$(boost)/config/platform/cygwin.hpp \
	$(boost)/config/platform/hpux.hpp \
	$(boost)/config/platform/irix.hpp \
	$(boost)/config/platform/qnxnto.hpp \
	$(boost)/config/platform/solaris.hpp \
	$(boost)/config/platform/win32.hpp \
	$(boost)/config/requires_threads.hpp \
	$(boost)/config/stdlib/dinkumware.hpp \
	$(boost)/config/stdlib/libcomo.hpp \
	$(boost)/config/stdlib/modena.hpp \
	$(boost)/config/stdlib/msl.hpp \
	$(boost)/config/stdlib/roguewave.hpp \
	$(boost)/config/stdlib/sgi.hpp \
	$(boost)/config/stdlib/stlport.hpp \
	$(boost)/config/stdlib/vacpp.hpp \
	zlib-1.2.3/zconf.h \
	zlib-1.2.3/zlib.h \
	zlib-1.2.3/contrib/minizip/crypt.h \
	zlib-1.2.3/contrib/minizip/ioapi.h \
	zlib-1.2.3/contrib/minizip/iowin32.c \
	zlib-1.2.3/contrib/minizip/iowin32.h \
	zlib-1.2.3/contrib/minizip/miniunz.c \
	zlib-1.2.3/contrib/minizip/minizip.c \
	zlib-1.2.3/contrib/minizip/mztools.c \
	zlib-1.2.3/contrib/minizip/mztools.h \
	zlib-1.2.3/contrib/minizip/unzip.h \
	zlib-1.2.3/contrib/minizip/zip.h \
	zlib-1.2.3.win32/bin/minizip.dll \
	zlib-1.2.3.win32/bin/zlib1.dll \
	zlib-1.2.3.win32/lib/minizip.lib \
	zlib-1.2.3.win32/lib/zdll.lib \
	zlib-1.2.3.win32/lib/zlib.lib

