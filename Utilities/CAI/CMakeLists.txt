# This is the root CAI CMakeLists file.
# $Id$
#

IF(WIN32)
   SUBDIRS(cai_dll)
ELSE(WIN32)
   ADD_DEFINITIONS(-D_NOHDF16)
   SUBDIRS(src inc)
ENDIF(WIN32)
