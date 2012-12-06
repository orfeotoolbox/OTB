#ifndef ossimPpjFrameSensorFile_HEADER
#define ossimPpjFrameSensorFile_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <iostream>

class OSSIM_DLL ossimPpjFrameSensorFile : public ossimObject
{
public:
   enum PointType
   {
      UNKNOWN_POINT_TYPE = 0,
      BASIC_POINT_TYPE   = 1
   };
   class OSSIM_DLL PointMap
   {
   public:
      PointType      m_type;
      ossimDpt3d     m_point;
      ossimDpt       m_pixelPoint;
   };
   typedef std::vector<PointMap> PointMapList;
   typedef std::vector<ossim_float64> DoubleArrayType;
   ossimPpjFrameSensorFile();

   virtual bool readFile(const ossimFilename& file);
   virtual bool readStream(std::istream& is); 

   const ossimString&     getBaseName()const;
   ossim_int64            getImageNumber()const;
   const ossimDpt&        getPrincipalPoint()const;
   const ossimGpt&        getPlatformPosition()const;
   const NEWMAT::Matrix&  getExtrinsic()const;
   const NEWMAT::Matrix&  getIntrinsic()const;
   const ossimDpt&        getImageSize()const;
   const DoubleArrayType& getRadialDistortion()const;
   const DoubleArrayType& getTangentialDistortion()const;

   double getAverageProjectedHeight()const;
protected:
   void reset();
   ossimPpjFrameSensorFile(const ossimPpjFrameSensorFile& src);
   ossimPpjFrameSensorFile& operator =(const ossimPpjFrameSensorFile& src);

   ossimGpt                      m_platformPosition;
   ossimFilename                 m_filename;
   ossimString                   m_fileBaseName;
   ossim_int64                   m_imageNumber;
   ossimRefPtr<ossimXmlDocument> m_ppjXml;
   ossimDpt                      m_imageSize;
   ossimDpt                      m_principalPoint;
   NEWMAT::Matrix                m_extrinsicMatrix;
   NEWMAT::Matrix                m_intrinsicMatrix;
   DoubleArrayType               m_radialDistortion;
   DoubleArrayType               m_tangentialDistortion;

   PointMapList                  m_pointMapList;
   double                        m_averageProjectedHeight;
};

#endif
