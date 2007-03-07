//*******************************************************************
// Copyright (C) 2003 Stroage Area Networks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimMetadataFileWriter.cpp,v 1.8 2005/06/19 16:07:29 dburken Exp $

#include <imaging/metadata/ossimMetadataFileWriter.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimViewController.h>
#include <base/common/ossimKeywordNames.h>
#include <base/context/ossimErrorContext.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimFilename.h>
#include <imaging/factory/ossimImageHandlerRegistry.h>
#include <imaging/formats/ossimImageHandler.h>


RTTI_DEF3(ossimMetadataFileWriter,
          "ossimMetadataFileWriter",
          ossimConnectableObject,
          ossimProcessInterface,
          ossimConnectableObjectListener);

static ossimTrace traceDebug("ossimMetadataFileWriter:degug");

ossimMetadataFileWriter::ossimMetadataFileWriter()
   :
      ossimProcessInterface(),
      ossimConnectableObjectListener(),
      theInputConnection(NULL),
      theFilename(),
      thePixelType(OSSIM_PIXEL_IS_POINT),
      theAreaOfInterest()
{
   theAreaOfInterest.makeNan();
}

ossimMetadataFileWriter::~ossimMetadataFileWriter()
{
}

void ossimMetadataFileWriter::initialize()
{
   theInputConnection = PTR_CAST(ossimImageSourceInterface,
                                 getInput());
}

bool ossimMetadataFileWriter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   bool result = ossimConnectableObject::loadState(kwl, prefix);

   // Get the filename...
   const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if(lookup)
   {
      ossimString filename = lookup;

      setFilename(filename);
   }

   // Note: Defaulted to point in constructor.
   lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_TYPE_KW);
   if (lookup)
   {
      ossimString s = lookup;
      s.downcase();
      if (s.contains("area"))
      {
         thePixelType = OSSIM_PIXEL_IS_AREA;
      }
      else if (s.contains("point"))
      {
         thePixelType = OSSIM_PIXEL_IS_POINT;
      }
   }
   
   if(theInputObjectList.size() != 1)
   {
      theInputObjectList.resize(1);
      theInputListIsFixedFlag = true;
   }
   
   theOutputListIsFixedFlag = false;
      
   return result;
}

void ossimMetadataFileWriter::disconnectInputEvent(ossimConnectionEvent& event)
{
   initialize();
}

void ossimMetadataFileWriter::connectInputEvent(ossimConnectionEvent& event)
{
   theInputConnection = PTR_CAST(ossimImageSourceInterface, getInput());
   initialize();
}

void ossimMetadataFileWriter::propertyEvent(ossimPropertyEvent& event)
{
   initialize();
}

void ossimMetadataFileWriter::setPixelType(ossimPixelType pixelType)
{
   thePixelType = pixelType;
}

ossimPixelType ossimMetadataFileWriter::getPixelType()const
{
   return thePixelType;
}

ossimObject* ossimMetadataFileWriter::getObject()
{
   return this;
}

const ossimObject* ossimMetadataFileWriter::getObject() const
{
   return this;
}

bool ossimMetadataFileWriter::execute()
{
   bool result = false;
   
   if (!theInputConnection)
   {
      initialize();
      if (!theInputConnection)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimMetadataFileWriter::execute DEBUG:\n"
               << "theInputConnection is null!"
               << endl;
         }
         return result;
      }
   }
   
   if(theFilename == ossimFilename::NIL)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimMetadataFileWriter::execute DEBUG:\n"
            << "theFilename is not set!"
            << endl;
      }
      return result;
   }
   
   //---
   // If theAreaOfInterest hasNans then it was probably not set so use
   // the bounding rectangle of the input source.
   //---
   if (theAreaOfInterest.hasNans())
   {
      theAreaOfInterest = theInputConnection->getBoundingRect();
      if (theAreaOfInterest.hasNans())
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimMetadataFileWriter::execute DEBUG:\n"
               << "theAreaOfInterest is not set!"
               << endl;
         }
         return result;
      }
   }

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);

   setPercentComplete(0.0);

   result = writeFile();

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimMetadataFileWriter::execute DEBUG:\n"
         << (result?"successful":"unsuccessful") << " write of file:  "
         << theFilename
         << endl;
   }
   
   return result;
}

void ossimMetadataFileWriter::setPercentComplete(double percentComplete)
{
   ossimProcessInterface::setPercentComplete(percentComplete);
   ossimProcessProgressEvent event(this,
                                   percentComplete);
   fireEvent(event);      
}

void ossimMetadataFileWriter::setFilename(const ossimFilename& file)
{
   theFilename = file;
}

const ossimFilename& ossimMetadataFileWriter::getFilename() const
{
   return theFilename;
}

bool ossimMetadataFileWriter::canConnectMyInputTo(ossim_int32 inputIndex,
                                                  const ossimConnectableObject* object) const
{
   return (object &&
           ((PTR_CAST(ossimImageSourceInterface, object) &&
             inputIndex == 0)||
            (PTR_CAST(ossimViewController, object) &&
             inputIndex == 1)));
}

void ossimMetadataFileWriter::setAreaOfInterest(
   const ossimIrect& areaOfInterest)
{
   theAreaOfInterest = areaOfInterest;
}

ossimIrect ossimMetadataFileWriter::getAreaOfInterest() const
{
   return theAreaOfInterest;
}
