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
// $Id: ossimVpfDatabaseHeader.cpp,v 1.2 2002/06/19 16:05:00 gpotts Exp $
#include "ossimVpfDatabaseHeader.h"
#include "base/common/ossimErrorCodes.h"
#include "vpf_util/vpftable.h"
ossimVpfDatabaseHeader::ossimVpfDatabaseHeader()
{
}


ossimVpfDatabaseHeader::~ossimVpfDatabaseHeader()
{
}

ossimErrorCode ossimVpfDatabaseHeader::open(const ossimFilename& databaseHeaderTable)
{
   vpf_table_type tableTypeData;

   if(is_vpf_table(const_cast<char*>(databaseHeaderTable.c_str())))
   {
      tableTypeData = vpf_open_table(const_cast<char*>(databaseHeaderTable.c_str()),
                                     (storage_type)DISK,
                                     "rb",
                                     NULL);
      if(isDatabaseHeaderTable(tableTypeData))
      {
         
      }
      else
      {
         return ossimErrorCodes::OSSIM_ERROR;
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

bool ossimVpfDatabaseHeader::isDatabaseHeaderTable(vpf_table_type& tableType)
{
   return true;
}
