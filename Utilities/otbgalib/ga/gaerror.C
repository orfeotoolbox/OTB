// $Header$
/* ----------------------------------------------------------------------------
  error.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  This file contains all of the error messages for the library.
---------------------------------------------------------------------------- */
#include <string.h>
#include <ga/gaerror.h>
#include <stdio.h>

char gaErrMsg[512];
char _gaerrbuf1[120];
char _gaerrbuf2[120];


#ifdef GALIB_USE_STREAMS
static STD_OSTREAM *__gaErrStream = & STD_CERR;
#endif
static GABoolean __gaErrFlag = gaTrue;
static char *__gaErrStr[] = {
  "error reading from file: ",
  "error writing to file: ",
  "unexpected EOF encountered during read.",
  "bad probability value.  Must be between 0.0 and 1.0, inclusive.",
  "objects are different types.",
  "this method has not been defined.",
  "core deleted with references remaining.",

  "the custom replacement strategy requires a replacement function",
  "unknown replacement strategy",
  "number of children must be greater than 0",
  "replacement percentage must be between 0.0 and 1.0, inclusive",
  "number of indiv for replacement must be less than pop size",
  "index of individual is out-of-bounds",
  "population contains no individuals from which to clone new individuals",
  "there must be at least one individual in each population",
  "no sexual crossover has been defined.  no mating can occur.",
  "no asexual crossover has been defined.  no mating can occur.",

  "children must have same resize behaviour for any given dimension",
  "parents and children must have the same dimensions",
  "parents must be the same length",
  "upper limit must be greater than lower limit.",
  "bad phenotype - ID is out of bounds.",
  "bad phenotype - value is less than min or greater than max.",
  "dimensions of bounds set do not match dimensions of the genome",

  "linear scaling multiplier must be greater than 1.0",
  "sigma truncation multiplier must be greater than 0.0",
  "negative objective function score!\n\
    all raw objective scores must be positive for linear scaling.",
  "negative objective function score!\n\
    all raw objective scores must be positive for power law scaling.",
  "the cutoff for triangular sharing must be greater than 0.0",

  "cannot index an allele in a bounded, non-discretized set of alleles",
  "length of binary string exceeds maximum for this computer/OS type.",
  "specified value cannot be exactly represented with these bits.",
  "bad 'where' indicator",
  "bogus type, data may be corrupt",
  "bad links in tree.  operation aborted.",
  "cannot swap a node with its ancestor",
  "cannot insert this object into itself",
  "node relative to which insertion is made must be non-NULL.",
  "root node must have no siblings.  insertion aborted.",
  "cannot insert before a root node (only below).",
  "cannot insert after a root node (only below)."  
};

void 
GAErr(const GASourceLocator loc, const char *clss, const char *func,
      const char *msg1, const char *msg2, const char *msg3){
  gaErrMsg[0] = '\0';
  strcat(gaErrMsg, clss);
  strcat(gaErrMsg, "::");
  strcat(gaErrMsg, func);
  strcat(gaErrMsg, ":\n  ");
  strcat(gaErrMsg, msg1);
  strcat(gaErrMsg, "\n");
  if(msg2){
    strcat(gaErrMsg, "  ");
    strcat(gaErrMsg, msg2);
    strcat(gaErrMsg, "\n");
  }
  if(msg3){
    strcat(gaErrMsg, "  ");
    strcat(gaErrMsg, msg3);
    strcat(gaErrMsg, "\n");
  }
  sprintf(_gaerrbuf1, "  %s : %ld\n", loc.file, loc.line);
  strcat(gaErrMsg, _gaerrbuf1);
#ifdef GALIB_USE_STREAMS
  if(__gaErrFlag == gaTrue) *__gaErrStream << gaErrMsg;
#endif
}

void 
GAErr(const GASourceLocator loc, const char *clss, const char *func,
      GAErrorIndex i, const char *msg2, const char *msg3){
  gaErrMsg[0] = '\0';
  strcat(gaErrMsg, clss);
  strcat(gaErrMsg, "::");
  strcat(gaErrMsg, func);
  strcat(gaErrMsg, ":\n  ");
  strcat(gaErrMsg, __gaErrStr[i]);
  strcat(gaErrMsg, "\n");
  if(msg2){
    strcat(gaErrMsg, "  ");
    strcat(gaErrMsg, msg2);
    strcat(gaErrMsg, "\n");
  }
  if(msg3){
    strcat(gaErrMsg, "  ");
    strcat(gaErrMsg, msg3);
    strcat(gaErrMsg, "\n");
  }
  sprintf(_gaerrbuf1, "  %s : %ld\n", loc.file, loc.line);
  strcat(gaErrMsg, _gaerrbuf1);
#ifdef GALIB_USE_STREAMS
  if(__gaErrFlag == gaTrue) *__gaErrStream << gaErrMsg;
#endif
}

void 
GAErr(const GASourceLocator loc, const char *func, 
      GAErrorIndex i, const char *msg2, const char *msg3){
  gaErrMsg[0] = '\0';
  strcat(gaErrMsg, func);
  strcat(gaErrMsg, ":\n  ");
  strcat(gaErrMsg, __gaErrStr[i]);
  strcat(gaErrMsg, "\n");
  if(msg2){
    strcat(gaErrMsg, "  ");
    strcat(gaErrMsg, msg2);
    strcat(gaErrMsg, "\n");
  }
  if(msg3){
    strcat(gaErrMsg, "  ");
    strcat(gaErrMsg, msg3);
    strcat(gaErrMsg, "\n");
  }
  sprintf(_gaerrbuf1, "  %s : %ld\n", loc.file, loc.line);
  strcat(gaErrMsg, _gaerrbuf1);
#ifdef GALIB_USE_STREAMS
  if(__gaErrFlag == gaTrue) *__gaErrStream << gaErrMsg;
#endif
}


void
GAReportErrors(GABoolean flag){
  __gaErrFlag = flag;
}

#ifdef GALIB_USE_STREAMS
void
GASetErrorStream(STD_OSTREAM & s){
  __gaErrStream = &s;
}
#endif
