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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfDatabaseHeader.h,v 1.3 2002/09/05 16:46:56 gpotts Exp $
#ifndef ossimVpfDatabaseHeader_HEADER
#define ossimVpfDatabaseHeader_HEADER
#include "base/data_types/ossimFilename.h"
#include "base/context/ossimErrorContext.h"


#include "vpf_util/vpftable.h"

class ossimVpfDatabaseHeader
{
public:
   ossimVpfDatabaseHeader();
   virtual ~ossimVpfDatabaseHeader();
   virtual ossimErrorCode open(const ossimFilename& databaseHeaderTable);
private:
   bool isDatabaseHeaderTable(vpf_table_type& tableType);
};

#endif
