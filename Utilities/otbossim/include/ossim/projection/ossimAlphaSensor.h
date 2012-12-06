//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  Alpha Sensor Base Class
//
//*******************************************************************
//  $Id$
#ifndef ossimAlphaSensor_HEADER
#define ossimAlphaSensor_HEADER 1
#include <ossim/projection/ossimSensorModel.h>

class OSSIM_DLL ossimAlphaSensor : public ossimSensorModel
{
public:
   ossimAlphaSensor();

   ossimAlphaSensor(const ossimAlphaSensor& src);

   virtual ossimObject* dup()const;
   
      
   virtual void lineSampToWorld(const ossimDpt& image_point,
                                ossimGpt&       worldPoint) const;

   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                const double&   heightEllipsoid,
                                ossimGpt&       worldPoint) const;

   virtual void updateModel();

   inline virtual bool useForward()const {return true;}
   
   virtual void initAdjustableParameters();
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   void setFov(const double fov);
   void setRollBias(const double rollBias);
   void setPitchBias(const double pitchBias);
   void setHeadingBias(const double headingBias);
   void setSlitRot(const double slitRot);

   void setRollPoly(const std::vector< ossim_float64 > rollPoly);
   void setPitchPoly(const std::vector< ossim_float64 > pitchPoly);
   void setHeadingPoly(const std::vector< ossim_float64 > headingPoly);
   void setLonPoly(const std::vector< ossim_float64 > lonPoly);
   void setLatPoly(const std::vector< ossim_float64 > latPoly);
   void setAltPoly(const std::vector< ossim_float64 > altPoly);
   void setScanPoly(const std::vector< ossim_float64 > scanPoly);

   void getPositionOrientation(const ossim_float64& line,
                                     ossimEcefPoint& pos,
                                     NEWMAT::Matrix& cam2EcfRot)const;

   ossimEcefPoint getCameraPosition(const ossim_float64& line)const;

   NEWMAT::Matrix getPlatform2LocalRot(const ossim_float64& line)const;

   ossim_float64  evalPoly(const std::vector<ossim_float64>& polyCoef,
                           const ossim_float64& line)const;
   
protected:
   NEWMAT::Matrix formHPRmat(const ossim_float64& roll,
                             const ossim_float64& pitch,
                             const ossim_float64& heading)const;

   NEWMAT::Matrix formLLAmat(const ossim_float64& lat,
                             const ossim_float64& lon,
                             const ossim_float64& az)const;

   ossim_float64 m_rollBias;
   ossim_float64 m_pitchBias;
   ossim_float64 m_headingBias;
   ossim_float64 m_fov;
   ossim_float64 m_slitRot;

   ossim_float64 m_focalLength;

   std::vector< ossim_float64 > m_rollPoly;
   std::vector< ossim_float64 > m_pitchPoly;
   std::vector< ossim_float64 > m_headingPoly;
   std::vector< ossim_float64 > m_lonPoly;
   std::vector< ossim_float64 > m_latPoly;
   std::vector< ossim_float64 > m_altPoly;
   std::vector< ossim_float64 > m_scanPoly;

   // Bias rotation matrix
   NEWMAT::Matrix m_cam2Platform;
  
   ossim_float64 m_adjustedFocalLength;
   TYPE_DATA
};

#endif // #ifndef ossimAlphaSensor_HEADER
