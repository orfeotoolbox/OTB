//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Doug Shibla (dshibla@imagelinks.com)
//
// DESCRIPTION:
//
//*****************************************************************************
//  $Id: ossimNitfMapModel.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimNitfMapModel_HEADER
#define ossimNitfMapModel_HEADER

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <iostream>

class ossimString;
class ossimMapProjection;

class OSSIMDLLEXPORT ossimNitfMapModel : public ossimSensorModel
{
public:
   /*!
    * CONSTRUCTORS:
    */
   ossimNitfMapModel();
   ossimNitfMapModel(const ossimFilename& init_file);
   ossimNitfMapModel(const ossimKeywordlist& geom_kwl);
   
   virtual ~ossimNitfMapModel();

   /*!
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const { return 0; } // TBR
   
   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   /*!
    * Writes a template of geom keywords processed by loadState and saveState
    * to output stream.
    */
   static void writeGeomTemplate(ostream& os);
   
   //***
   // Overrides base class pure virtual.
   //***
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   /*!
    * ossimOptimizableProjection
    */
   inline virtual bool useForward()const {return false;} //!image to ground faster

protected:
   
TYPE_DATA
};

#endif
