//----------------------------------------------------------------------------
// Copyright (c) 2005, Oscar Kramer, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description:
// 
// Special "sensor model" that provides a conventional map projection but
// with additional capability for adjusting the map x,y coordinates with
// offset, scale, and rotation transform.  Funtions to control notifications,
// information and error output.
//
//----------------------------------------------------------------------------
// $Id: ossimAdjMapModel.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAdjMapModel_HEADER
#define ossimAdjMapModel_HEADER

#include <ossim/matrix/newmat.h>
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimRefPtr.h>

using namespace NEWMAT;

class ossimMapProjection;

//----------------------------------------------------------------------------
//  CLASS DESCRIPTION: ossimAdjMapModel
//! Special "sensor model" that provides a conventional map projection but with
//  additional
//! capability for adjusting the map x,y coordinates with offset, scale, and
//  rotation transform.
//----------------------------------------------------------------------------
class OSSIMDLLEXPORT ossimAdjMapModel : public ossimSensorModel
{
public:
   //! Constructs to uninitialized state (needed by factory).
   ossimAdjMapModel();

   //! Constructs with map projection and image rectangle size.
   ossimAdjMapModel(ossimMapProjection* map_proj, const ossimIpt& image_size);

   //! Copy constructor
   ossimAdjMapModel(const ossimAdjMapModel& copy_this);

   //! Constructs given geom KWL.
   ossimAdjMapModel(const ossimKeywordlist& kwl, const char* prefix);

   //! Constructs given geom filename.
   ossimAdjMapModel(const ossimFilename& geom_filename);

   //! Initializes base class data members after map model established.
   //  Returns TRUE if all's well.
   bool initializeFromMap();

   //! Returns pointer to a new instance, copy of this.
   virtual ossimObject* dup() const;
   
   //! Extends base-class implementation. Dumps contents of object to ostream.
   virtual std::ostream& print(std::ostream& out) const;
   
   //! Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
   //! KWL files. Returns true if successful.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   
   //! Writes a template of geom keywords processed by loadState and saveState
   //  to output stream.
   static void writeGeomTemplate(ostream& os);
   
   //! Overrides base class pure virtual.
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   
   //! Overrides base class virtual.
   virtual void lineSampleToWorld(const ossimDpt& image_point,
                                  ossimGpt&       worldPoint) const;
   
   //! Rigorous inverse transform implented, overrides base-class' iterative
   //  solution.
   virtual void worldToLineSample(const ossimGpt& world_point,
                                  ossimDpt&       image_point) const;
      
   //! Following a change to the adjustable parameter set, this virtual is
   //  called to permit 
   //! instances to compute derived quantities after parameter change.
   virtual void updateModel();

   //! Initializes adjustable parameters to their default values.
   //! Overrides ossimAdjustableParameterInterface virtual method.
   virtual void initAdjustableParameters();

   //! Fetches the GSD from the internal map projection.
   virtual ossimDpt 	getMetersPerPixel () const;

   //! Enumeration for adjustable parameter positions in theAdjParams vector
   enum AdjParamIndex
   {
      OFFSET_X = 0,         // meters
      OFFSET_Y,             // meters
      ROTATION,             // degrees CCW
      SCALE_X,              // ratio
      SCALE_Y,              // ratio
      NUM_ADJ_PARAMS // not an index
   };

   /*!
    * ossimOptimizableProjection
    */
   inline virtual bool useForward()const {return true;} //!ground to image faster (you don't need DEM)

private:
   //! Destructor only deletes the map projection.
   virtual ~ossimAdjMapModel();
   
   //! This is the vector of UNNORMALIZED, BIASED parameters as used in the
   //! model. The collection of normalized, 0-biased, parameters used in
   //! registration are found in the
   //! ossimAdjustableParameterInterface and related classes.
   //!  The quantities here are computed given the base class quantities.
   ColumnVector                    theAdjParams;

   ossimRefPtr<ossimMapProjection> theMapProjection;
   double                          theCosTheta;
   double                          theSinTheta;

TYPE_DATA
};

#endif /* End of "#ifndef ossimAdjMapModel_HEADER" */
