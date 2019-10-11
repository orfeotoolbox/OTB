/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef ossimTileMapModel_HEADER
#define ossimTileMapModel_HEADER

#include <ossimPluginConstants.h>
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimMatrix3x3.h>

class ossimMapProjection;

namespace ossimplugins
{
//******************************************************************************
//*
//* CLASS:  ossimTileMapModel
//*
//*******************************************************************************
class OSSIM_PLUGINS_DLL ossimTileMapModel : public ossimSensorModel
{
public:
  /*!
   * CONSTRUCTORS:
   */

  using ossimOptimizableProjection::operator=;

  ossimTileMapModel();
  ossimTileMapModel(const ossimKeywordlist& geom_kwl);
  ossimTileMapModel(const ossimTileMapModel& rhs);

  virtual ~ossimTileMapModel(){};

  enum ProjectionType
  {
    UNKNOWN_PROJECTION = 0,
    UTM_MAP,
    UTM_ORBIT,
    SOM_MAP,
    SOM_ORBIT
  };



  /*!
   * Returns pointer to a new instance, copy of this.
   */
  virtual ossimObject* dup() const { return 0; } // TBR

  /*!
   * Extends base-class implementation. Dumps contents of object to ostream.
   */
  virtual std::ostream& print(std::ostream& out) const;


  bool open(const ossimFilename& file);

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
  static void writeGeomTemplate(std::ostream& os);

  //***
  // Overrides base class pure virtual.
  //***
  virtual void worldToLineSample(const ossimGpt& ground_point,
                                 ossimDpt&       img_pt) const;
  virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                       const double&   heightEllipsoid,
                                       ossimGpt&       worldPoint) const;
  virtual void lineSampleToWorld(const ossimDpt& image_point,
                                 ossimGpt&       worldPoint) const;

  /*!
   * Set/Get the depth of the quadtree decomposition
   *
   */
  void setDepth(unsigned int depth){
    qDepth = depth;
  }
  unsigned int getDepth()
  {
    return qDepth;
  }


  /*!
   * ossimOptimizableProjection
   */
  inline virtual bool useForward()const {return true;} //!image to ground faster


protected:



  //***
  // Image constant parameters:
  //***
  double           theIllumAzimuth;
  double           theIllumElevation;
  double           theOrbitAltitude;
  double           theOrbitInclination;
  int              theMapZone;
  ossimDpt         theMapOffset;
  int              theWrsPathNumber;
  int              theWrsRowNumber;
  double           theMeridianalAngle;
  double           thePositionError;

  ProjectionType       theProjectionType;
  ossimRefPtr<ossimMapProjection>  theMapProjection;

  double           theMapAzimAngle;
  double           theMapAzimCos;
  double           theMapAzimSin;
  double           theMap2IcRotAngle;
  double           theMap2IcRotCos;
  double           theMap2IcRotSin;
 //
 // //***
 // // Adjustable parameters:
 // //***
  double           theIntrackOffset;
  double           theCrtrackOffset;
  double           theLineGsdCorr;
  double           theSampGsdCorr;
  double           theRollOffset;
  double           theYawOffset;
  double           theYawRate;
  double           theMapRotation;
 //
 // //***
 // // Quantities derived from the adjustable parameters:
 // //***
  NEWMAT::Matrix   theRollRotMat;

  unsigned int qDepth;
  TYPE_DATA


};
} // End: namespace ossimplugins
#endif
