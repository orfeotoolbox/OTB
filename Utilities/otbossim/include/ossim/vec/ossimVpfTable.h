//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfTable.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimVpfTable_HEADER
#define ossimVpfTable_HEADER
#include <iostream>
#include <vector>


#include <ossim/base/ossimString.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/vec/vpf.h>

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
