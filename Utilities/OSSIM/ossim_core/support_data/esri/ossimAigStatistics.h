//*******************************************************************
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
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigStatistics.h,v 1.1 2002/08/12 15:26:04 gpotts Exp $
#ifndef ossimAigStatistics_HEADER
#define ossimAigStatistics_HEADER
#include <iostream>

class ossimAigStatistics
{
public:
   ossimAigStatistics()
      {
         reset();
      }

   void reset()
      {
         theMin         = 0.0;
         theMax         = 0.0;
         theMean        = 0.0;
         theStandardDev = 0.0;
      }

   bool writeStream(ostream& out);

   double theMin;
   double theMax;
   double theMean;
   double theStandardDev;
};

#endif
