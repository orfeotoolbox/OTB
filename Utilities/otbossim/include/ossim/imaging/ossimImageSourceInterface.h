//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSourceInterface.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageSourceInterface_HEADER
#define ossimImageSourceInterface_HEADER

#include <vector>

#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>

class ossimImageData;
class ossimObject;

class OSSIMDLLEXPORT ossimImageSourceInterface
{
public:
   virtual ~ossimImageSourceInterface();
   
   virtual ossimObject* getObject()=0;
   virtual const ossimObject* getObject()const=0;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIpt& origin,
                                               ossim_uint32 resLevel=0) = 0;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0) = 0;
   
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const=0;
   virtual void getDecimationFactors(std::vector<ossimDpt>& decimations)const=0;
   virtual ossim_uint32 getNumberOfDecimationLevels()const=0;

   /*!
    * Returns the number of bands available from the input.
    */
   virtual ossim_uint32 getNumberOfInputBands() const = 0;

   /*!
    * Returns the number of bands in output.
    */
   virtual ossim_uint32 getNumberOfOutputBands() const = 0;

   /*!
    * Initializes bandList to the zero based order of output bands.
    */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const=0;
   
   virtual ossimScalarType getOutputScalarType() const = 0;

   virtual ossim_uint32 getTileWidth()  const = 0;
   virtual ossim_uint32 getTileHeight() const = 0;

   virtual double getNullPixelValue(ossim_uint32 band=0)const=0;
   virtual double getMinPixelValue(ossim_uint32 band=0)const=0;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const=0;

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const=0;
   
   virtual void getValidImageVertices(std::vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const=0;
   
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0)=0;

   /**
    * Sets the image geometry keyword list.
    *
    * @note Callers should note that this will override any existing
    *       geometry.
    *
    * @note This does not write the geometry to disk, to do so call
    *       one of the saveImageGeometry methods. 
    */
   virtual void setImageGeometry(const ossimKeywordlist& kwl)=0;

   /**
    * Saves the image geometry to a file that will be made based on the image
    * file name.
    */
   virtual void saveImageGeometry()const=0;

   /**
    * Saves the image geomety to geom_file.
    */
   virtual void saveImageGeometry(const ossimFilename& geometry_file)const=0;

   virtual void initialize()=0;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0)=0;
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const=0;

TYPE_DATA
};

#endif
