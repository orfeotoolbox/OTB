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
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimImageDisplayWriter.cpp,v 1.3 2003/05/13 11:31:03 dburken Exp $
#include "ossimImageDisplayWriter.h"

RTTI_DEF3(ossimImageDisplayWriter, "ossimImageDisplayWriter", ossimImageWriter, ossimDisplayInterface, ossimConnectableObjectListener);

ossimImageDisplayWriter::ossimImageDisplayWriter(ossimObject* owner)
   :ossimImageWriter(owner,
                     1,
                     0,
                     true,
                     true),
    ossimConnectableObjectListener()
{
}

ossimImageDisplayWriter::ossimImageDisplayWriter(ossimObject* owner,
                                                 ossim_uint32 numberOfInputs,
                                                 ossim_uint32 numberOfOutputs,
                                                 bool inputListIsFixed,
                                                 bool outputListIsFixed)
   : ossimImageWriter(owner,
                      numberOfInputs,
                      numberOfOutputs,
                      inputListIsFixed,
                      outputListIsFixed),
     ossimConnectableObjectListener()
{
}
