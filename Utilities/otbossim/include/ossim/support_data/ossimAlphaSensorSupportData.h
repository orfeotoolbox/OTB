#ifndef ossimAlphaSensorSupportData_HEADER
#define ossimAlphaSensorSupportData_HEADER 1
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimPolynom.h>

class ossimEnviHeader;

class OSSIM_DLL ossimAlphaSensorSupportData : public ossimObject
{
public:
   enum PolynomialDegrees
   {
      MAX_DEGREE     = 10,
      MAX_TERMS      = MAX_DEGREE + 1,
      SCAN_ANGLE_DEG = 2,
      ROLL_DEG       = 7,
      PITCH_DEG      = 5,
      HEADING_DEG    = 4,
      LON_DEG        = 2,
      LAT_DEG        = 2,
      ALT_DEG        = 2
   };

   ossimAlphaSensorSupportData();

   virtual bool readSupportFiles(const ossimFilename& hdrFile);
   virtual bool readSupportFiles(const ossimEnviHeader& hdr);
   virtual bool readHdrFile(const ossimFilename& hdrFile);
   virtual bool readHdrFile(const ossimEnviHeader& hdr);
   virtual bool readInsGpsFile(const ossimFilename& txtFile);

   inline bool isHSI()const {return m_isHSI;}

   const ossimDpt&      getImageSize()const;
   const ossim_float64& getRollBias()const;
   const ossim_float64& getPitchBias()const;
   const ossim_float64& getHeadingBias()const;
   const ossim_float64& getFov()const;
   const ossim_float64& getSlitRot()const;

   const std::vector< ossim_float64 >& getRollPoly()const;
   const std::vector< ossim_float64 >& getPitchPoly()const;
   const std::vector< ossim_float64 >& getHeadingPoly()const;
   const std::vector< ossim_float64 >& getLonPoly()const;
   const std::vector< ossim_float64 >& getLatPoly()const;
   const std::vector< ossim_float64 >& getAltPoly()const;
   const std::vector< ossim_float64 >& getScanPoly()const;

protected:
   void reset();
   ossimAlphaSensorSupportData(const ossimAlphaSensorSupportData& src);
   ossimAlphaSensorSupportData& operator =(const ossimAlphaSensorSupportData& src);

   bool m_isHSI;

   // hdr data
   ossimString   m_sensorType;
   ossimDpt      m_imageSize;
   ossim_float64 m_rollBias;
   ossim_float64 m_pitchBias;
   ossim_float64 m_headingBias;
   ossim_float64 m_fov;
   ossim_float64 m_slitRot;

   // insgps data
   std::vector< ossimPolynom<ossim_float64,1>::VAR_TUPLE > m_imgLine;
   std::vector< ossim_float64 > m_roll;
   std::vector< ossim_float64 > m_pitch;
   std::vector< ossim_float64 > m_heading;
   std::vector< ossim_float64 > m_lon;
   std::vector< ossim_float64 > m_lat;
   std::vector< ossim_float64 > m_alt;
   std::vector< ossim_float64 > m_scanAng;

   // insgps polynomials
   std::vector< ossim_float64 > m_rollPolyCoef;
   std::vector< ossim_float64 > m_pitchPolyCoef;
   std::vector< ossim_float64 > m_headingPolyCoef;
   std::vector< ossim_float64 > m_lonPolyCoef;
   std::vector< ossim_float64 > m_latPolyCoef;
   std::vector< ossim_float64 > m_altPolyCoef;
   std::vector< ossim_float64 > m_scanAngPolyCoef;
};

#endif
