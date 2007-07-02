//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigStatistics.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ostream>
#include <ossim/base/ossimEndian.h>
#include <ossim/support_data/ossimAigStatistics.h>

ossimAigStatistics::ossimAigStatistics()
   :theMin(0.0),
    theMax(0.0),
    theMean(0.0),
    theStandardDev(0.0)
{
}

void ossimAigStatistics::reset()
{
   theMin         = 0.0;
   theMax         = 0.0;
   theMean        = 0.0;
   theStandardDev = 0.0;
}

bool ossimAigStatistics::writeStream(std::ostream& out)
{
   ossimEndian endian;
   double tempDouble;
   
   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      tempDouble = theMin;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);

      tempDouble = theMax;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      
      tempDouble = theMean;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      
      tempDouble = theStandardDev;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
   }
   else
   {
      out.write((char*)(&theMin), 8);
      out.write((char*)(&theMax), 8);
      out.write((char*)(&theMean), 8);
      out.write((char*)(&theStandardDev), 8);
   }

   return true;
}
