#include <ossim/support_data/ossimPpjFrameSensorFile.h>
#include <ossim/base/ossimXmlNode.h>
#include <fstream>
#include <ossim/matrix/newmatio.h>
#include <ossim/base/ossimEcefPoint.h>

ossimPpjFrameSensorFile::ossimPpjFrameSensorFile()
:m_radialDistortion(2),
m_tangentialDistortion(2),
m_averageProjectedHeight(0.0)
{
   reset();
}

const ossimString& ossimPpjFrameSensorFile::getBaseName()const
{
   return m_fileBaseName;
}

ossim_int64 ossimPpjFrameSensorFile::getImageNumber()const
{
   return m_imageNumber;
}

const ossimDpt& ossimPpjFrameSensorFile::getPrincipalPoint()const
{
   return m_principalPoint;
}
const ossimGpt& ossimPpjFrameSensorFile::getPlatformPosition()const
{
   return m_platformPosition;
}

const NEWMAT::Matrix& ossimPpjFrameSensorFile::getExtrinsic()const
{
   return m_extrinsicMatrix;
}

const NEWMAT::Matrix& ossimPpjFrameSensorFile::getIntrinsic()const
{
   return m_intrinsicMatrix;
}

const ossimDpt& ossimPpjFrameSensorFile::getImageSize()const
{
   return m_imageSize;
}

const ossimPpjFrameSensorFile::DoubleArrayType& ossimPpjFrameSensorFile::getRadialDistortion()const
{
   return m_radialDistortion;
}

const ossimPpjFrameSensorFile::DoubleArrayType& ossimPpjFrameSensorFile::getTangentialDistortion()const
{
   return m_tangentialDistortion;
}

double ossimPpjFrameSensorFile::getAverageProjectedHeight()const
{
   return m_averageProjectedHeight;
}

void ossimPpjFrameSensorFile::reset()
{
   m_ppjXml = 0;
   m_principalPoint = ossimDpt(0.0,0.0);
   m_platformPosition = ossimGpt(0.0,0.0);
   m_fileBaseName = "";
   m_imageNumber = -1;
   m_radialDistortion[0] = 0.0;
   m_radialDistortion[1] = 0.0;
   m_tangentialDistortion[0] = 0.0;
   m_tangentialDistortion[1] = 0.0;
   m_intrinsicMatrix = NEWMAT::Matrix(3,3);
   m_extrinsicMatrix = NEWMAT::Matrix(4,4);
   std::fill(m_intrinsicMatrix.Store(), m_intrinsicMatrix.Store() + 9, 0.0);
   std::fill(m_extrinsicMatrix.Store(), m_extrinsicMatrix.Store() + 16, 0.0);
   m_extrinsicMatrix[0][0] = 1.0;
   m_extrinsicMatrix[1][1] = 1.0;
   m_extrinsicMatrix[2][2] = 1.0;
   m_extrinsicMatrix[3][3] = 1.0;
   m_imageSize.makeNan();
   m_averageProjectedHeight = 0.0;
   m_pointMapList.clear();
}

bool ossimPpjFrameSensorFile::readFile(const ossimFilename& file)
{
   bool result = false;
   std::ifstream in(file.c_str(), std::ios::in|std::ios::binary);
   if(in.good()&&readStream(in))
   {

      result = true;
   }

   return result;
}

bool  ossimPpjFrameSensorFile::readStream(std::istream& is)
{
   reset();
   m_ppjXml = new ossimXmlDocument;
   if(m_ppjXml->read(is))
   {
      ossimRefPtr<ossimXmlNode> root = m_ppjXml->getRoot();
      if(root.valid()&&(root->getTag() == "pearlProjectionFile"))
      {
         ossimRefPtr<ossimXmlNode> image = root->findFirstNode("image");
         if(image.valid())
         {
            ossimRefPtr<ossimXmlNode> sensorData     = image->findFirstNode("sensorData");
            ossimRefPtr<ossimXmlNode> namingData     = image->findFirstNode("namingData");
            ossimRefPtr<ossimXmlNode> projectionData = image->findFirstNode("projectionData");
            if(namingData.valid())
            {
               ossimRefPtr<ossimXmlNode> fileBaseName = namingData->findFirstNode("fileBaseName");
               ossimRefPtr<ossimXmlNode> imageNumber  = namingData->findFirstNode("imageNumber");
               if(fileBaseName.valid())
               {
                  m_fileBaseName = fileBaseName->getText();
               }
               if(imageNumber.valid())
               {
                  m_imageNumber = imageNumber->getText().toInt64();
               }
            }
            if(projectionData.valid())
            {
               const ossimXmlNode::ChildListType& childNodes = projectionData->getChildNodes();
               ossim_uint32 nNodes = childNodes.size();
               ossim_uint32 idx = 0;
               ossim_uint32 averageHeightDivisor = 0;
               for(idx = 0; idx < nNodes; ++idx)
               {
                  if(childNodes[idx]->getAttributeValue("type").downcase() == "basic")
                  {
                     ossimRefPtr<ossimXmlNode> pixelPointX = childNodes[idx]->findFirstNode("pixelPointX");
                     ossimRefPtr<ossimXmlNode> pixelPointY = childNodes[idx]->findFirstNode("pixelPointY");
                     ossimRefPtr<ossimXmlNode> ecefX = childNodes[idx]->findFirstNode("ecefX");
                     ossimRefPtr<ossimXmlNode> ecefY = childNodes[idx]->findFirstNode("ecefY");
                     ossimRefPtr<ossimXmlNode> ecefZ = childNodes[idx]->findFirstNode("ecefZ");

                     if(pixelPointX.valid()&&pixelPointY.valid()&ecefX.valid()&&ecefY.valid()&&ecefZ.valid())
                     {
                        PointMap pointMap;
                        pointMap.m_type       = BASIC_POINT_TYPE;
                        pointMap.m_point      = ossimDpt3d(ecefX->getText().toDouble(), ecefY->getText().toDouble(), ecefZ->getText().toDouble());
                        pointMap.m_pixelPoint = ossimDpt(pixelPointX->getText().toDouble(),pixelPointY->getText().toDouble());

                        ossimGpt gpt(ossimEcefPoint(pointMap.m_point.x, pointMap.m_point.y, pointMap.m_point.z));
                        m_averageProjectedHeight += gpt.height();
                        ++averageHeightDivisor;
                        m_pointMapList.push_back(pointMap);
                     }
                  }
               }
               if(averageHeightDivisor > 0) m_averageProjectedHeight /= static_cast<double>(averageHeightDivisor); 
            }
            if(sensorData.valid())
            {
               ossimRefPtr<ossimXmlNode> sensorSize      = sensorData->findFirstNode("sensorSize");
               ossimRefPtr<ossimXmlNode> cameraIntrinsic = sensorData->findFirstNode("cameraIntrinsic");
               ossimRefPtr<ossimXmlNode> cameraExtrinsic = sensorData->findFirstNode("cameraExtrinsic");
               if(sensorSize.valid())
               {
                  ossimRefPtr<ossimXmlNode> width  = sensorSize->findFirstNode("SensorWidth");
                  ossimRefPtr<ossimXmlNode> height = sensorSize->findFirstNode("SensorHeight");
                  if(width.valid()&&height.valid())
                  {
                     m_imageSize.x = width->getText().toDouble();
                     m_imageSize.y = height->getText().toDouble();
                  }
                  else
                  {
                     m_ppjXml = 0;
                  }
               }
               else
               {
                  m_ppjXml = 0;
               }
               if(m_ppjXml.valid()&&cameraExtrinsic.valid())
               {
                  if(cameraExtrinsic->getChildNodes().size() == 4)
                  {
                     m_extrinsicMatrix = NEWMAT::Matrix(4,4);
                     std::vector<ossimString> inRow1 = cameraExtrinsic->getChildNodes()[0]->getText().split(" ");
                     std::vector<ossimString> inRow2 = cameraExtrinsic->getChildNodes()[1]->getText().split(" ");
                     std::vector<ossimString> inRow3 = cameraExtrinsic->getChildNodes()[2]->getText().split(" ");
                     std::vector<ossimString> inRow4 = cameraExtrinsic->getChildNodes()[3]->getText().split(" ");

                     if((inRow1.size() ==4)&&
                        (inRow2.size() ==4)&&
                        (inRow3.size() ==4)&&
                        (inRow4.size() ==4))
                     {
                        m_extrinsicMatrix << inRow1[0].toDouble() << inRow1[1].toDouble() << inRow1[2].toDouble() 
                                          << inRow1[3].toDouble() 
                                          << inRow2[0].toDouble() << inRow2[1].toDouble() << inRow2[2].toDouble() 
                                          << inRow2[3].toDouble()
                                          << inRow3[0].toDouble() << inRow3[1].toDouble() << inRow3[2].toDouble() 
                                          << inRow3[3].toDouble()
                                          << inRow4[0].toDouble() << inRow4[1].toDouble() << inRow4[2].toDouble() 
                                          << inRow4[3].toDouble();
                        NEWMAT::ColumnVector v(4);
                        v[0] = v[1] = v[2] = 0.0;
                        v[3] = 1.0;
                        NEWMAT::ColumnVector result = m_extrinsicMatrix*v;
                        m_platformPosition = ossimEcefPoint(result[0], result[1], result[2]);
                        //m_platformPosition;
                     }
                     else
                     {
                        m_ppjXml = 0;
                     }

                  } // end  if(cameraExtrinsic->getChildNodes().size() == 4)
                  else
                  {
                     m_ppjXml = 0;
                  }

                  if(m_ppjXml.valid())
                  {
                     if(cameraIntrinsic.valid())
                     {
                        m_intrinsicMatrix = NEWMAT::Matrix(3,3);
                        std::vector<ossimString> inRow1 = cameraIntrinsic->getChildNodes()[0]->getText().split(" ");
                        std::vector<ossimString> inRow2 = cameraIntrinsic->getChildNodes()[1]->getText().split(" ");
                        std::vector<ossimString> inRow3 = cameraIntrinsic->getChildNodes()[2]->getText().split(" ");
                        if((inRow1.size() ==3)&&
                           (inRow2.size() ==3)&&
                           (inRow3.size() ==3))
                        {
                           m_principalPoint.x = inRow1[2].toDouble();
                           m_principalPoint.y = inRow2[2].toDouble();
                           m_intrinsicMatrix << inRow1[0].toDouble() << inRow1[1].toDouble() << m_principalPoint.x
                                             << inRow2[0].toDouble() << inRow2[1].toDouble() << m_principalPoint.y
                                             << inRow3[0].toDouble() << inRow3[1].toDouble() << inRow3[2].toDouble(); 
                        }
                        else
                        {
                           m_ppjXml = 0;
                        }
                     } // end if(cameraIntrinsic .....)
                     else
                     {
                        m_ppjXml = 0;
                     }
                  } // end if m_ppjXml ...
               } // end if(m_ppjXml.valid()&&cameraExtrinsic.valid())
               else
               {
                  m_ppjXml = 0;
               }
            }
            else
            {
               m_ppjXml = 0;
            }
         }
         else
         {
            m_ppjXml = 0;
         }
      }
      else
      {
         m_ppjXml = 0;
      }
   }
   else
   {
      m_ppjXml = 0;
   }


   return m_ppjXml.valid();
}

// Hidden from use...
ossimPpjFrameSensorFile::ossimPpjFrameSensorFile(const ossimPpjFrameSensorFile& /* src */)
{
}

ossimPpjFrameSensorFile& ossimPpjFrameSensorFile::operator=(
   const ossimPpjFrameSensorFile& /* src */)
{
   return *this;
}

