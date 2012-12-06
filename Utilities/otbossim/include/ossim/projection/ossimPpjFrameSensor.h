//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  PPJ Frame Model
//
//*******************************************************************
//  $Id$
#ifndef ossimPpjFrameSensor_HEADER
#define ossimPpjFrameSensor_HEADER 1
#include <ossim/projection/ossimSensorModel.h>

class OSSIM_DLL ossimPpjFrameSensor : public ossimSensorModel
{
public:
   ossimPpjFrameSensor();

   ossimPpjFrameSensor(const ossimPpjFrameSensor& src);

   virtual ossimObject* dup()const;
   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;
   
   virtual void lineSampleToWorld(const ossimDpt& image_point,
                                 ossimGpt&       worldPoint) const;
   
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                const double&   heightEllipsoid,
                                ossimGpt&       worldPoint) const;

   virtual void worldToLineSample(const ossimGpt& world_point,
                                  ossimDpt&       image_point) const;

   virtual void updateModel();
   
   void setFocalLength(double focX, double focY);

   void setCameraPosition(const ossimGpt& value);
   
   void setPrincipalPoint(const ossimDpt& value);

   void setecef2CamMatrix(const NEWMAT::Matrix& value);
   void setAveragePrjectedHeight(double averageProjectedHeight);

   inline virtual bool useForward()const {return true;}
   
   virtual void initAdjustableParameters();
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   
protected:
   NEWMAT::Matrix m_ecef2Cam;
   NEWMAT::Matrix m_ecef2CamInverse;
   ossimDpt       m_principalPoint;   // pixels
   double         m_focalLengthX;     // pixels
   double         m_focalLengthY;     // pixels
   double         m_focalLength;      // pixels
   ossimEcefPoint m_ecefCameraPosition;
   ossimGpt       m_cameraPositionEllipsoid;

   // Not used yet
   double         m_radialK1;
   double         m_radialK2;
   double         m_radialP1;
   double         m_radialP2;
   
   ossimGpt       m_adjustedCameraPosition;
   double         m_adjustedFocalLength;
   double         m_averageProjectedHeight;
   TYPE_DATA
};

#endif /* #ifndef ossimPpjFrameSensor_HEADER */
