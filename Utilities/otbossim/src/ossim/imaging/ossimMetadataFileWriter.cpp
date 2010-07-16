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
//  $Id: ossimMetadataFileWriter.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <ossim/imaging/ossimMetadataFileWriter.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimViewController.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimFilenameProperty.h>

RTTI_DEF3(ossimMetadataFileWriter,
          "ossimMetadataFileWriter",
          ossimConnectableObject,
          ossimProcessInterface,
          ossimConnectableObjectListener);

static ossimTrace traceDebug("ossimMetadataFileWriter:degug");

ossimMetadataFileWriter::ossimMetadataFileWriter()
   :ossimConnectableObject(0),
      ossimProcessInterface(),
      ossimConnectableObjectListener(),
      theInputConnection(NULL),
      theFilename(),
      thePixelType(OSSIM_PIXEL_IS_POINT),
      theAreaOfInterest()
{
   addListener((ossimConnectableObjectListener*)this);
   theAreaOfInterest.makeNan();
}

ossimMetadataFileWriter::~ossimMetadataFileWriter()
{
   removeListener((ossimConnectableObjectListener*)this);
}

void ossimMetadataFileWriter::initialize()
{
   theInputConnection = dynamic_cast<ossimImageSource*> (getInput(0));
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

void ossimMetadataFileWriter::disconnectInputEvent(ossimConnectionEvent& /* event */)
{
   initialize();
}

void ossimMetadataFileWriter::connectInputEvent(ossimConnectionEvent& /* event */)
{
   initialize();
}

void ossimMetadataFileWriter::propertyEvent(ossimPropertyEvent& /* event */)
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
           ((PTR_CAST(ossimImageSource, object) &&
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

void ossimMetadataFileWriter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid())return;
   
   ossimString name = property->getName();

   if(name == ossimKeywordNames::FILENAME_KW)
   {
      setFilename(property->valueToString());
   }
   else
   {
      ossimConnectableObject::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimMetadataFileWriter::getProperty(const ossimString& name)const
{
   if(name == ossimKeywordNames::FILENAME_KW)
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, theFilename);
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_OUTPUT);
      
      return filenameProp;
      
   }

   return ossimConnectableObject::getProperty(name);
}

void ossimMetadataFileWriter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimConnectableObject::getPropertyNames(propertyNames);
   propertyNames.push_back(ossimKeywordNames::FILENAME_KW);
}
