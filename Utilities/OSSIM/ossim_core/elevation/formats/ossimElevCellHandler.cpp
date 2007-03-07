//*****************************************************************************
// FILE: ossimElevCellHandler.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// DESCRIPTION:
//   Contains implementation of class ossimElevCellHandler. This is the base
//   class for all DEM file readers including DTED. Each file shall have its
//   corresponding ossimElevCellHandler. When more than one file is accessed,
//   an ossimElevCombiner is used which owns multiple instances of this class.
//
// SOFTWARE HISTORY:
//>
//   19Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************

#include <elevation/formats/ossimElevCellHandler.h>
#include <base/data_types/ossimKeyword.h>
#include <base/data_types/ossimGpt.h>

RTTI_DEF1(ossimElevCellHandler, "ossimElevCellHandler" , ossimElevSource)

//***
// Define Trace flags for use within this file:
//***
#include "base/common/ossimTrace.h"
static ossimTrace traceExec  ("ossimElevCellHandler:exec");
static ossimTrace traceDebug ("ossimElevCellHandler:debug");

static const ossimKeyword DEM_FILENAME_KW ("dem_filename",
                                           "Name of DEM file to load.");
static const ossimIpt ZERO_SIZE_IPT (0, 0);

ossimElevCellHandler::ossimElevCellHandler ()
{
}

ossimElevCellHandler::ossimElevCellHandler(const char* filename)
   :
      theFilename (filename)
{}

ossimElevCellHandler::~ossimElevCellHandler()
{}

ossimFilename ossimElevCellHandler::getFilename() const
{
   return theFilename;
}

double ossimElevCellHandler::getMeanSpacingMeters() const
{
   return theMeanSpacing;
}

double ossimElevCellHandler::getAccuracyLE90(const ossimGpt&) const
{
   return theAbsLE90;
}

double ossimElevCellHandler::getAccuracyCE90(const ossimGpt&) const
{
   return theAbsCE90;
}

bool ossimElevCellHandler::canConnectMyInputTo(
   ossim_int32 inputIndex,
   const ossimConnectableObject* object) const
{         
   return false;
}

// Hidden from use.
ossimElevCellHandler::ossimElevCellHandler (const ossimElevCellHandler&)
{
}

const ossimElevCellHandler& ossimElevCellHandler::operator=(
   const ossimElevCellHandler& rhs)
{
   return rhs;
}
