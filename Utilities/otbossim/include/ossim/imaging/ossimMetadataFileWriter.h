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
//  $Id: ossimMetadataFileWriter.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimMetadataFileWriter_H
#define ossimMetadataFileWriter_H

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimProcessProgressEvent.h>

class ossimImageSource;

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
   

   virtual ossimObject*       getObject();
   virtual const ossimObject* getObject() const;

   virtual void initialize();
   
   virtual bool execute();
   
   virtual void setPercentComplete(double percentComplete);
   
   virtual void setFilename(const ossimFilename& file);
   
   const ossimFilename& getFilename()const;

   /**
    * Load state method:
    *
    * This method call base class ossimConnectableObject::loadState then
    * looks for its keywords.
    *
    * @param kwl Keyword list to initialize from.
    *
    * @param prefix Usually something like: "object1."
    *
    * @return This method will alway return true as it is intended to be
    * used in conjuction with the set methods.
    *
    * Keywords picked up by loadState:
    *
    * filename: foo.tfw
    *
    * (pixel_type should be area or point)
    * 
    * pixel_type: area
    */
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
   
   /*!
    * property interface
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
protected:
   virtual ~ossimMetadataFileWriter();
   /**
    * Write out the file.
    * @return true on success, false on error.
    */
   virtual bool writeFile() = 0;

   ossimImageSource* theInputConnection;
   ossimFilename              theFilename;
   ossimPixelType             thePixelType;
   ossimIrect                 theAreaOfInterest;
   
TYPE_DATA
};

#endif /* End of #ifndef ossimMetadataFileWriter_H */
