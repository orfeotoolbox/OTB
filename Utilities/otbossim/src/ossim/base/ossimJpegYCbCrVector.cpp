#include <ossim/base/ossimJpegYCbCrVector.h>
#include <ossim/base/ossimCommon.h>

#include <ossim/base/ossimRgbVector.h>

ossimJpegYCbCrVector::ossimJpegYCbCrVector(const ossimRgbVector& rgbColor)
{
   theBuf[0] = static_cast<unsigned char>(clamp(ossim::round<int>(0.299*rgbColor.getR() +
                                          0.587*rgbColor.getG() +
                                          0.114*rgbColor.getB())));
   
   theBuf[1] = static_cast<unsigned char>(clamp(ossim::round<int>((-0.1687)*rgbColor.getR() -
                                                0.3313*rgbColor.getG() +
                                                0.5*rgbColor.getB() + 128)));
   
   theBuf[2] = static_cast<unsigned char>(clamp(ossim::round<int>(0.5*rgbColor.getR() -
                                                      .4187*rgbColor.getG() -
                                                      .0813*rgbColor.getB() + 128)));
   
}

ossimJpegYCbCrVector& ossimJpegYCbCrVector::operator =(const ossimRgbVector& rgbColor)
{
   theBuf[0] = static_cast<unsigned char>(clamp(ossim::round<int>(0.299*rgbColor.getR() +
                                                0.587*rgbColor.getG() +
                                                0.114*rgbColor.getB())));
   
   theBuf[1] = static_cast<unsigned char>(clamp(ossim::round<int>((-0.1687)*rgbColor.getR() -
                                                      0.3313*rgbColor.getG() +
                                                      0.5*rgbColor.getB() + 128)));
   
   theBuf[2] = static_cast<unsigned char>(clamp(ossim::round<int>(0.5*rgbColor.getR() -
                                                      .4187*rgbColor.getG() -
                                                      .0813*rgbColor.getB() + 128)));
   
   return *this;
}
