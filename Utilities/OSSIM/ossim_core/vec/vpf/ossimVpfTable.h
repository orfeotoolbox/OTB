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
// $Id: ossimVpfTable.h,v 1.7 2005/10/17 18:42:27 gpotts Exp $
#ifndef ossimVpfTable_HEADER
#define ossimVpfTable_HEADER
#include <iostream>
#include <vector>


#include <base/data_types/ossimString.h>
#include <base/context/ossimErrorContext.h>
#include <base/data_types/ossimFilename.h>
#include <vec/vpf/vpf.h>

class OSSIM_DLL ossimVpfTable
{
public:
	friend OSSIM_DLL std::ostream& operator <<(std::ostream& out,
                               const ossimVpfTable& data);
   
   ossimVpfTable();
   virtual ~ossimVpfTable();

   /*!
    * Will open the table.  Will return OSSIM_OK if it was successful
    * or OSSIM_ERROR if unsuccessful.
    */
   virtual bool openTable(const ossimFilename& tableName);

   /*!
    * Closes the table.
    */
   virtual void closeTable();

   /*!
    * This code is going to be cut paste from the vpf_dump_table
    * found in vpf_util/vpf_dump_table.c.  This function only allows
    * you to output to a file.  We need to re-write it to go to a stream.
    * the best way to do this is to cut and paste the code.
    */
   virtual void print(std::ostream& out)const;

  int getNumberOfRows()const;
  int getNumberOfColumns()const;
  ossimString getColumnName(int idx)const;
   virtual bool isClosed()const;
   /*!
    * This method will set the row pointer back to
    * the start of the table.
    */
   virtual void reset()const;

   virtual bool goToRow(long row)const;
   vpf_table_type* getVpfTableData()
      {
         return theTableInformation;
      }

	  std::vector<ossimString> getColumnValues(const ossimString& columnName)const;
	  std::vector<ossimString> getColumnValues(long columnNumber)const;
   
   ossimString getColumnValueAsString(row_type& row,
                                      long columnNumber)const;
   ossimString getColumnValueAsString(ossim_int32 rowNumber,
                                      long columnNumber)const;
   ossimString getColumnValueAsString(const ossimString& columnName);
   ossim_int32 getColumnPosition(const ossimString& columnName)const;

protected:
   /*!
    * this structure is in vpf_util/vpftable.h file.
    * it holds all the access information to the table.
    */
   vpf_table_type* theTableInformation;

   /*!
    * Will hold the complete path and name to this table.
    */
   ossimFilename   theTableName;
};

#endif
