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
//  $Id: ossimImageWriter.cpp,v 1.14 2006/01/03 13:24:18 dburken Exp $

#include <imaging/ossimImageWriter.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimFilename.h>
#include <base/common/ossimKeywordNames.h>
#include <base/context/ossimErrorContext.h>
#include <imaging/formats/tiff/ossimTiffOverviewBuilder.h>
#include <imaging/factory/ossimImageHandlerRegistry.h>
#include <imaging/formats/ossimImageHandler.h>

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
   
