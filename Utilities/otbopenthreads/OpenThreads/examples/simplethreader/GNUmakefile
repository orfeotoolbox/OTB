TOPDIR = ../../.
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	ThreadObserver.cpp \
	ThreadReporter.cpp \
	SimpleThreader.cpp \
	$(NULL)

INC      += -I$(TOPDIR)/include
LIBS     += -lOpenThreads

EXEC = SimpleThreader

include $(TOPDIR)/Make/makerules