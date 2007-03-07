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
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class definition for InterleaveTypeLUT.
//*******************************************************************
//  $Id: ossimInterleaveTypeLut.cpp,v 1.1.1.1 2001/02/05 20:13:25 dburken Exp $

#include "ossimInterleaveTypeLut.h"

static const int TABLE_SIZE = 4;

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimInterleaveTypeLut::ossimInterleaveTypeLut()
   :
      ossimLookUpTable(TABLE_SIZE)
{
   //***
   // Complete initialization of data member "theTable".
   // Note:  Interleave types defined in constants.h file.
   //***
   theTable[0].theKey   = OSSIM_BIL;
   theTable[0].theValue = "bil";
   theTable[1].theKey   = OSSIM_BIP;
   theTable[1].theValue = "bip";
   theTable[2].theKey   = OSSIM_BSQ;
   theTable[2].theValue = "bsq";
   theTable[3].theKey   = OSSIM_BSQ_MULTI_FILE;
   theTable[3].theValue = "bsq_multi_file";
}
