//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Oscar Kramer (SoBe Software)
// Description: Container class for LandSat5 Fast Format header files (RevB & C)
//
// ********************************************************************
// $Id: ossimFfL5.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimFfL5_HEADER
#define ossimFfL5_HEADER


#include <ossim/support_data/ossimFfL7.h>
#include <ossim/support_data/ossimFfRevb.h>

using namespace std;

// Forward class declarations.

// ***************************************************************************
// CLASS: ossimFfL5
// ***************************************************************************

class OSSIM_DLL ossimFfL5 : public ossimFfL7
{
   friend ostream& operator<<(ostream& os, const ossimFfL5& head);

public:
   ossimFfL5();
   ossimFfL5(const char* headerFile);
   ossimRefPtr<ossimFfRevb> revb();
   const ossimRefPtr<ossimFfRevb> revb()const;
 
protected:
   virtual ~ossimFfL5();

private:
   void    readHeaderRevB(const ossimString& header_name);
   int     convertGeoPoint(const char* sptr, ossimGpt& geo_point);

   ossimRefPtr<ossimFfRevb> theRevb;
};

#endif
