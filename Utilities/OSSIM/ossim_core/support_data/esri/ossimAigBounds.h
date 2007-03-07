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
// $Id: ossimAigBounds.h,v 1.4 2002/08/27 18:25:36 gpotts Exp $
#ifndef ossimAigBounds_HEADER
#define ossimAigBounds_HEADER
#include <iostream>

class ossimAigBounds
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimAigBounds& data)
      {
         out << "ll_x:  " << data.ll_x << std::endl
             << "ll_y:  " << data.ll_y << std::endl
             << "ur_x:  " << data.ur_x << std::endl
             << "ur_y:  " << data.ur_y;
         
         return out;
      }
   ossimAigBounds()
      {
         reset();
      }

   void reset()
      {
         ll_x = 0.0;
         ll_y = 0.0;
         ur_x = 0.0;
         ur_y = 0.0;
      }
   bool writeStream(std::ostream& out);
   double ll_x;
   double ll_y;
   double ur_x;
   double ur_y;
};


#endif
