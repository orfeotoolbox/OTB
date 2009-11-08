//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimCmyVector.h 14789 2009-06-29 16:48:14Z dburken $
#include <iostream>
#include <ossim/base/ossimConstants.h>

class ossimJpegYCbCrVector;
class ossimHsiVector;
class ossimHsvVector;
class ossimRgbVector;

class OSSIMDLLEXPORT ossimCmyVector
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimCmyVector& cmy)
      {
         out << "<" << cmy.theC << ", " << cmy.theM <<", " << cmy.theY << ">" << std::endl;
         
         return out;
      }
   ossimCmyVector(unsigned char c,
                  unsigned char m,
                  unsigned char y)
      :
         theC(c),
         theM(m),
         theY(y)
      {}
   ossimCmyVector(const ossimRgbVector& rgb);

   ossim_uint8 getC()const
      {
         return theC;
      }
   ossim_uint8 getM()const
      {
         return theC;
      }
   ossim_uint8 getY()const
      {
         return theC;
      }
   const ossimCmyVector& operator = (const ossimRgbVector& rgb);
protected:
   ossim_uint8 theC;
   ossim_uint8 theM;
   ossim_uint8 theY;
};
