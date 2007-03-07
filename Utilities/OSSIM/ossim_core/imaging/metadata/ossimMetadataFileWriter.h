//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimMetadataFileWriter.h,v 1.12 2005/08/08 22:00:24 dburken Exp $

#ifndef ossimMetadataFileWriter_H
#define ossimMetadataFileWriter_H

#include <base/common/ossimConstants.h>
#include <base/common/ossimConnectableObject.h>
#include <base/common/ossimProcessInterface.h>
#include <base/common/events/ossimConnectableObjectListener.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimIrect.h>
#include <base/common/events/ossimObjectEvents.h>
#include <base/common/events/ossimProcessProgressEvent.h>

class ossimImageSourceInterface;

/**
 * ossimMetadataFileWriter
 *
 * Typical usage something like this:
 *
 * ossimObject* obj = ossimImageMetaDataWriterRegistry::instance()->
 *     createObject(ossimString("ossimReadmeFileWriter"));
 * if (!obj)
 * {
 *    return;
 * }
 * ossimMetadataFileWriter* mw = PTR_CAST(ossimMetadataFileWriter, obj);
 * if (!mw)
 * {
 *    return;
 * }
 * 
 * mw->setFilename(xmlFile);
 * mw->loadState(kwl);
 * mw->connectMyInputTo(ih.get());
 * mw->execute();
 * delete mw;
 */
class OSSIMDLLEXPORT ossimMetadataFileWriter :
   public ossimConnectableObject,
   public ossimProcessInterface,
   public ossimConnectableObjectListener
{   
public:
   ossimMetadataFileWriter();
   
   virtual ~ossimMetadataFileWriter();

   virtual ossimObject*       getObject();
   virtual const ossimObject* getObject() const;

   virtual void initialize();
   
   virtual bool execute();
   
   virtual void setPercentComplete(double percentComplete);
   
   virtual void setFilename(const ossimFilename& file);
   
   const ossimFilename& getFilename()const;

   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix = 0);
   
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object) const;
   
   virtual void disconnectInputEvent(ossimConnectionEvent& event);
   virtual void connectInputEvent(ossimConnectionEvent& event);
   virtual void propertyEvent(ossimPropertyEvent& event);

   /**
    * Ossim uses a concept of "pixel is point" internally.
    * 
    * This means that if you say a tie point is 30.0N -81.0W, the center of
    * the pixel at the tie point is 30.0N -81.0W.
    *
    */ 
   virtual void setPixelType(ossimPixelType pixelType);
   virtual ossimPixelType getPixelType() const;

   /**
    * Sets the area of interest to write the meta data for.
    * 
    * @param areaOfInterest Sets theAreaOfInterest.
    *
    * @note By default the writers will use
    * "theInputConnection->getBoundingRect()" if theAreaOfInterest has nans.
    */
   virtual void setAreaOfInterest(const ossimIrect& areaOfInterest);

   /**
    * @returns theAreaOfInterest.
    */
   virtual ossimIrect getAreaOfInterest() const;

   /**
    * void getMetadatatypeList(std::vector<ossimString>& metadatatypeList)const
    *
    * pure virtual
    * 
    * Appends the writers metadata types to the "metadatatypeList".
    * 
    * This is the actual image type name.  So for
    * example, ossimTiffWorldFileWriter has tiff_world_file type.
    *
    * @param metadatatypeList stl::vector<ossimString> list to append to.
    *
    * @note All writers should append to the list, not, clear it and then add
    * their types.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList)const=0;

   /**
    * bool hasMetadataType(const ossimString& metadataType) const
    *
    * @param imageType string representing image type.
    *
    * @return true if "metadataType" is supported by writer.
    */
   virtual bool hasMetadataType(const ossimString& metadataType)const=0;
   
protected:
   /**
    * Write out the file.
    * @return true on success, false on error.
    */
   virtual bool writeFile() = 0;

   ossimImageSourceInterface* theInputConnection;
   ossimFilename              theFilename;
   ossimPixelType             thePixelType;
   ossimIrect                 theAreaOfInterest;
   
TYPE_DATA
};

#endif /* End of #ifndef ossimMetadataFileWriter_H */
