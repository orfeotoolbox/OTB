//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigStatistics.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAigStatistics_HEADER
#define ossimAigStatistics_HEADER

#include <iosfwd>

class ossimAigStatistics
{
public:
   ossimAigStatistics();

   void reset();

   bool writeStream(std::ostream& out);

   double theMin;
   double theMax;
   double theMean;
   double theStandardDev;
};

#endif
