//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimEsriShapeFileCutter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimEsriShapeFileCutter_HEADER
#define ossimEsriShapeFileCutter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimViewInterface.h>
#include <ossim/imaging/ossimMaskFilter.h>
#include <ossim/imaging/ossimEsriShapeFileFilter.h>

/**
 * This class wraps the ossimEsriShapeFileFilter and ossimMaskFilter to implement
 * a composite cutter.
 *
 * the ossimEsriShapeFileFilter is saved with a shape_file_filter. prefix and the
 * ossimMaskFilter is saved with a mask. prefix.  Please see ossimEsriShapeFileFilter
 * and ossimMaskFilter classes for their keywords. theborder size has an optional units
 * associated with it. 
 * <pre>
 *
 * The border size can be us-feet, feet, meters, or degrees and is only applied
 * to polygons.
 *
 * Example keyword list
 *
 * shape_file_filter.brush_color:  255 255 255
 * shape_file_filter.feature_name:  
 * shape_file_filter.filename:  
 * shape_file_filter.fill_flag:  0
 * shape_file_filter.max_quadtree_levels:  10
 * shape_file_filter.pen_color:  255 255 255
 * shape_file_filter.point_width_height:  1 1 
 * shape_file_filter.thickness:  1
 * shape_file_filter.border_size: 50 meters
 * shape_file_filter.type:  ossimEsriShapeFileFilter
 * mask.mask_type: select
 * mask.type: ossimMaskFilter
 
 * </pre>
 *
 */
class ossimEsriShapeFileCutter : public ossimImageSourceFilter,
                                 public ossimViewInterface
{
public:
   /**
    * allocates the mask and esri loader
    */
   ossimEsriShapeFileCutter();
   
   /**
    * destroys the data members
    */
   virtual ~ossimEsriShapeFileCutter();
   
   /**
    * Sent to the input
    */
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;

   /**
    * The bounding rect call is passed to the mask filter
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   /**
    * The get tile call is passed to the mask filter
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   /**
    * is passed to the filter and the shape file loader
    */
   virtual void initialize();

   /**
    * passed to the input source.
    */
   virtual void getDecimationFactors(vector<ossimDpt>& decimations)const;

   /**
    * passed to the input source.
    */
   virtual ossim_uint32 getNumberOfDecimationLevels()const;

   /**
    * passed to the input source.
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;
   
   /**
    * passed to the input source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * passed to the input source.
    */
   virtual void getValidImageVertices(
      vector<ossimIpt>& validVertices,
      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
      ossim_uint32 resLevel=0)const;

   /**
    * passed to the input source.
    */
   virtual ossim_uint32 getTileWidth() const;

   /**
    * passed to the input source.
    */
   virtual ossim_uint32 getTileHeight() const;

   /**
    * passed to the input source.
    */
   virtual double getNullPixelValue(ossim_uint32 band)const;
   
   /**
    * passed to the input source.
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   
   /**
    * passed to the input source.
    */
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   /**
    * passed to the shape file to load the shape file that will be used
    * by the mask filter as input.
    */
   void loadFile(const ossimFilename& filename);

   /**
    * this is here to support the view interface of the esri shape file.
    * it passes view calls to the esri filter
    */
   virtual bool setView(ossimObject* baseObject,
                        bool ownsTheView = false);
   
   /**
    * this is here to support the view interface of the esri shape file.
    * Will return the view setting for the esri shape file.
    */
   virtual ossimObject* getView();

   /**
    * this is here to support the view interface of the esri shape file.
    * Will return the view setting for the esri shape file.
    */
   virtual const ossimObject* getView()const;

   /**
    * Return the current esri shape file being used
    */
   ossimFilename getFilename()const;

   /**
    * Sets the mask type.  Passes the call to the Mask filter.
    */
   void setMaskType(ossimMaskFilter::ossimFileSelectionMaskType type);

   /**
    * returns the mask type from the maks filter.
    */
   ossimMaskFilter::ossimFileSelectionMaskType getMaskType()const;

   /**
    * Passes load state call to the esri shape file loader with prefix
    * shape_file_filter.  Then passes the load state cll to the mask
    * filter with prefix = mask.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Passes save state call to the esri shape file loader with prefix
    * shape_file_filter.  Then passes the load state cll to the mask
    * filter with prefix = mask.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   ossimEsriShapeFileFilter* theEsriShapeFile;
   ossimMaskFilter*          theMaskFilter;
TYPE_DATA
};

#endif
