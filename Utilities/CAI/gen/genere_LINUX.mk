#
#Version : $Header: genere.mk,v 1.7 00/11/29 11:02:37 cmf Exp $
#
MAKE=make
CPP=cpp -traditional
RM=/bin/rm

all : CAI_NOHDF

CAI_HDF :
	$(CPP) -P -D_HDF16 $(Le_makefile) > Cai.mk
	$(MAKE) -f Cai.mk CC='$(COMPILO)' $(BSLFLAG)
	$(RM) -f Cai.mk

CAI_NOHDF :
	@$(CPP) -P -D_NOHDF16 $(Le_makefile) > Cai.mk
	@$(MAKE) -f Cai.mk CC='$(COMPILO)' $(BSLFLAG)
	@$(RM) -f Cai.mk

