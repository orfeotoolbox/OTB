//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter.
//*******************************************************************
//  $Id: ossimImageWriter.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimImageWriter.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>

RTTI_DEF1(ossimImageWriter, "ossimImageWriter", ossimOutputSource);

ossimImageWriter::ossimImageWriter(ossimObject* owner)
   : ossimOutputSource(owner,
                       1,
                       0,
                       true,
                       true)
{
   theAreaOfInterest.makeNan();
}

ossimImageWriter::ossimImageWriter(ossimObject* owner,
                    ossim_uint32 numberOfInputs,
                    ossim_uint32 numberOfOutputs,
                    bool inputListIsFixed,
                    bool outputListIsFixed)
   :ossimOutputSource(owner,
                      numberOfInputs,
                      numberOfOutputs,
                      inputListIsFixed,
                      outputListIsFixed)
{
   theAreaOfInterest.makeNan();
}

ossimImageWriter::~ossimImageWriter()
{
}

bool ossimImageWriter::setViewingRect(const ossimIrect &aRect)
{
   setAreaOfInterest(aRect);
   
   return true;
}

ossimIrect ossimImageWriter::getAreaOfInterest() const
{
   return theAreaOfInterest;
}
 
void ossimImageWriter::setAreaOfInterest(const ossimIrect& inputAreaOfInterest)
{
   theAreaOfInterest = inputAreaOfInterest;
}

bool ossimImageWriter::saveState(ossimKeywordlist& kwl,
                                 const char* prefix)const
{
   return ossimOutputSource::saveState(kwl, prefix);
}

bool ossimImageWriter::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   return ossimOutputSource::loadState(kwl, prefix);
}
   
