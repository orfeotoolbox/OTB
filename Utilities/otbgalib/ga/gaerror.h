// $Header$
/* ----------------------------------------------------------------------------
  error.h
  mbwall 7may95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  This defines the error routines for handling errors.
---------------------------------------------------------------------------- */
#ifndef _ga_error_h_
#define _ga_error_h_

#include <ga/gatypes.h>
#include <ga/gaconfig.h>
#include <ga/std_stream.h>

// This object is for telling us where in the source code an error occurs.
class GASourceLocator {
public:
  GASourceLocator(const char* f, long l) : file(f), line(l) {}
  const char* file;
  long line;
};

#define GA_LOC GASourceLocator(__FILE__, __LINE__)



// These are the indices for all of the error messages used in the library.
enum GAErrorIndex {
// general errors
  gaErrReadError=0,
  gaErrWriteError,
  gaErrUnexpectedEOF,
  gaErrBadProbValue,
  gaErrObjectTypeMismatch,
  gaErrOpUndef,
  gaErrRefsRemain,

// errors for the GA objects
  gaErrNeedRS,
  gaErrBadRS,
  gaErrBadCS,
  gaErrBadPRepl,
  gaErrBadNRepl,
  gaErrBadPopIndex,
  gaErrNoIndividuals,
  gaErrBadPopSize,
  gaErrNoSexualMating,
  gaErrNoAsexualMating,

// errors for the genome and crossover objects
  gaErrSameBehavReqd,
  gaErrSameLengthReqd,
  gaErrBadParentLength,
  gaErrBadResizeBehaviour,
  gaErrBadPhenotypeID,
  gaErrBadPhenotypeValue,
  gaErrBadBndsDim,

// scaling scheme error messages
  gaErrBadLinearScalingMult,
  gaErrBadSigmaTruncationMult,
  gaErrNegFitness,
  gaErrPowerNegFitness,
  gaErrBadSharingCutoff,

// miscellaneous error messages from various data objects
  gaErrNoAlleleIndex,
  gaErrBinStrTooLong,
  gaErrDataLost,
  gaErrBadWhereIndicator,
  gaErrBadTypeIndicator,
  gaErrBadTreeLinks,
  gaErrCannotSwapAncestors,
  gaErrCannotInsertIntoSelf,
  gaErrCannotInsertOnNilNode,
  gaErrCannotInsertWithSiblings,
  gaErrCannotInsertBeforeRoot,
  gaErrCannotInsertAfterRoot
};



// Error messages will look like this: (assuming that you use streams)
//
// GASimpleGA::function: 
//   this is the error message
//   /usr/people/jimmyboy/doughhead.C : XXX
//
// GAGenome::function: 
//   primary error message
//   additional error messages go here
//   /usr/people/algore/distribute_your_intelligence.C : XXX
//
// MyOwnFunction: 
//   primary error message
//   additional error messages go here
//   /usr/people/algore/distribute_your_intelligence.C : XXX
//

void GAErr(const GASourceLocator loc,
	   const char *clss, const char *function_name,
	   GAErrorIndex i, const char *msg2=0, const char *msg3=0);
void GAErr(const GASourceLocator loc,
	   const char *clss, const char *function_name,
	   const char *msg1, const char *msg2=0, const char *msg3=0);
void GAErr(const GASourceLocator loc,
	   const char *function_name,
	   GAErrorIndex i, const char *msg2=0, const char *msg3=0);

// Use this function to turn on/off the error reporting.  If you turn off the
// error reporting, the messages will still get stuck into the global error 
// message string, but they will not be sent to the error stream.

void GAReportErrors(GABoolean flag);

// Provide a mechanism for redirecting the error messages.

#ifdef GALIB_USE_STREAMS
void GASetErrorStream(STD_OSTREAM &);
#else
inline void GASetErrorStream(){} // dummy function
#endif

// This error string contains the text of the most recent error message.  If a
// GAlib function returns an error code, this string will contain the text of
// the explanation for the error.
extern char gaErrMsg[];

#endif
