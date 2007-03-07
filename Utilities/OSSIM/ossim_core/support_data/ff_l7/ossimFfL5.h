// *******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Oscar Kramer (SoBe Software)
// Description: Container class for LandSat5 Fast Format header files (RevB & C)
//
// ********************************************************************
// $Id: ossimFfL5.h,v 1.1 2005/11/10 15:03:51 gpotts Exp $

#ifndef ossimFfL5_HEADER
#define ossimFfL5_HEADER


#include <support_data/ff_l7/ossimFfL7.h>

using namespace std;

// Forward class declarations.

// ***************************************************************************
// CLASS: ossimFfL5
// ***************************************************************************

class OSSIMDLLEXPORT ossimFfL5 : public ossimFfL7
{
   friend ostream& operator<<(ostream& os, const ossimFfL5& head);

public:
   ossimFfL5();
   ossimFfL5(const char* headerFile);
   virtual ~ossimFfL5();
   
private:
   void    readHeaderRevB(const ossimString& header_name);
   int     convertGeoPoint(const char* sptr, ossimGpt& geo_point);
};

#endif
