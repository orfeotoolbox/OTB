//*****************************************************************************
// FILE: ossimCoarseGridModel.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION:
//   Contains declaration of class ossimCoarseGridModel. This is an
//   implementation of an interpolation sensor model.
//
//*****************************************************************************
//  $Id: ossimCoarseGridModel.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimCoarseGridModel_HEADER
#define ossimCoarseGridModel_HEADER

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDblGrid.h>
#include <ossim/base/ossimFilename.h>

/******************************************************************************
 *
 * CLASS:  ossimCoarseGridModel
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimCoarseGridModel : public ossimSensorModel
{
public:
   /**
    * CONSTRUCTORS:
    */
   ossimCoarseGridModel();
   ossimCoarseGridModel(const ossimCoarseGridModel& copy_this);

   /**
    * CONSTRUCTOR (filename)
    * Accepts name of geometry file. This can be either MET ECG geom file, or
    * OSSIM keywordlist geometry file.
    */
   ossimCoarseGridModel(const ossimFilename& geom_file);

   /**
    * CONSTRUCTOR (keywordlist)
    * Accepts OSSIM keywordlist geometry file.
    */
   ossimCoarseGridModel(const ossimKeywordlist& geom_kwl);
   
   ~ossimCoarseGridModel();

   /**
    * This method will build a grid from any projector.
    * The first argument must be the image space bounds for
    * the projection.  The second argument is the projector
    * that will be used to approximate a bilinear grid over.
    *
    * The accuracy of the grid can be controlled by the static method
    * setInterpolationError.
    */
  virtual void buildGrid(const ossimDrect& imageBounds,
			 ossimProjection* proj,
                         double heightDelta=500.0,
                         bool enableHeightFlag=false,
                         bool makeAdjustableFlag=true);

   /**
    *  This is used when building a grid from a projector.
    *  You can set the interpolation error.  The default is
    *  subpixel accuracy (within .1 of a pixel).
    */
   static void setInterpolationError(double error=.1);
   static void setMinGridSpacing(ossim_int32 minSpacing = 100);
   /**
    * METHOD: print()
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   /**
    * METHODS:  saveState, loadState
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * STATIC METHOD: writeGeomTemplate(ostream)
    * Writes a template of geometry keywords processed by loadState and
    * saveState to output stream.
    */
   static void writeGeomTemplate(ostream& os);

   /**
    * METHOD: dup()
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const
      { return new ossimCoarseGridModel(*this); }
   
   /**
    * METHOD: loadGeomFile()
    * Since there are tweo geometry file formats supported (native KWL and MET
    * ECG), this method determines which parser method to invoke.
    */
   bool loadGeomFile(const ossimFilename& geom_file);
   
   /**
    * METHOD: saveCoarseGrid(), loadCoarseGrid()
    * Saves/loads the coarse grid to/from the specified file. Returns true if
    * successful.
    */
   bool saveCoarseGrid(const ossimFilename& cgFileName) const;
   bool loadCoarseGrid(const ossimFilename& cgFileName);

   /**
    * METHOD: loadMetEcgGeom()
    * Service to initialize this object from a MET ECG geom and grid files. 
    */
   bool loadMetEcgGeom(const ossimFilename& ecgGeomFile,
                       const ossimFilename& ecgGridFile);

   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   
   virtual void lineSampleToWorld(const ossimDpt& image_point,
                                  ossimGpt&       gpt) const;
   /**
    * METHOD: lineSampleHeightToWorld(image_point, height, &ground_point)
    * This is the virtual that performs the actual work of projecting
    * the image point to the earth at some specified elevation.
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       world_pt) const;


   virtual void initAdjustableParameters();

   /*!
    * ossimOptimizableProjection
    */
   inline virtual bool useForward()const {return false;} //!image to ground faster

   
private:
   /**
    * METHOD: reallocateGrid()
    * Deletes existing allocated memory and reallocates
    * new space. This may happen if a new grid is loaded over an existing one.
    */
   void reallocateGrid(const ossimIpt& size);
   
   /**
    * Data Members:
    */
   mutable ossimFilename theGridFilename;
   ossimDblGrid  theLatGrid;         // degrees
   ossimDblGrid  theLonGrid;         // degrees
   ossimDblGrid  theDlatDhGrid;      // degrees/meter
   ossimDblGrid  theDlonDhGrid;      // degrees/meter
   ossimDblGrid* theDlatDparamGrid;  // degrees/(units-of-param)
   ossimDblGrid* theDlonDparamGrid;  // degrees/(units-of-param)

   static double       theInterpolationError;
   static ossim_int32  theMinGridSpacing;
   ossimAdjustmentInfo theInitialAdjustment;
   bool                theHeightEnabledFlag;
   
   TYPE_DATA
};

#endif
