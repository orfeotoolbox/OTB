//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfDatabaseHeader.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimVpfDatabaseHeader_HEADER
#define ossimVpfDatabaseHeader_HEADER
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimErrorContext.h>


#include <ossim/vpfutil/vpftable.h>

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
