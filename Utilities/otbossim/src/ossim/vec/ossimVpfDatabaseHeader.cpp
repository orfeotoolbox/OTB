//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimVpfDatabaseHeader.cpp 17206 2010-04-25 23:20:40Z dburken $
#include <ossim/vec/ossimVpfDatabaseHeader.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/vpfutil/vpftable.h>
ossimVpfDatabaseHeader::ossimVpfDatabaseHeader()
{
}


ossimVpfDatabaseHeader::~ossimVpfDatabaseHeader()
{
}

ossimErrorCode ossimVpfDatabaseHeader::open(const ossimFilename& databaseHeaderTable)
{
   vpf_table_type tableTypeData;

   if( is_vpf_table( databaseHeaderTable.c_str() ) )
   {
      tableTypeData = vpf_open_table(databaseHeaderTable.c_str(),
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

bool ossimVpfDatabaseHeader::isDatabaseHeaderTable(vpf_table_type& /* tableType */)
{
   return true;
}
