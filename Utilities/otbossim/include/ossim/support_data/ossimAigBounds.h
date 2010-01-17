//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigBounds.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAigBounds_HEADER
#define ossimAigBounds_HEADER

#include <iosfwd>

class ossimAigBounds
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimAigBounds& data);

   ossimAigBounds();

   void reset();

   bool writeStream(std::ostream& out);
   double ll_x;
   double ll_y;
   double ur_x;
   double ur_y;
};


#endif
