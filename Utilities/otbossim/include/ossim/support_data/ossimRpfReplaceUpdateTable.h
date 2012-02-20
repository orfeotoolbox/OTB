//----------------------------------------------------------------------------
//
// File:     ossimRpfReplaceUpdateTable.h
// 
// License:  See top level LICENSE.txt file.
// 
// Author:   David Burken
//
// Description: See class description.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfReplaceUpdateTable.h 20328 2011-12-07 17:43:24Z dburken $

#ifndef ossimRpfReplaceUpdateTable_HEADER
#define ossimRpfReplaceUpdateTable_HEADER 1

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfReplaceUpdateRecord.h>
#include <iosfwd>
#include <string>
#include <vector>

/**
 * @class ossimRpfReplaceUpdateTable
 * @brief Holds a table of RPF replace/update section subheader records.
 * See MIL-STD-2411 for detailed information.
 */
class OSSIM_DLL ossimRpfReplaceUpdateTable : public ossimReferenced
{
public:

   /** @brief default constructor */
   ossimRpfReplaceUpdateTable();

   /** @brief copy constructor */
   ossimRpfReplaceUpdateTable(const ossimRpfReplaceUpdateTable& obj);

   /** @brief assignment operator */
   const ossimRpfReplaceUpdateTable& operator=(const ossimRpfReplaceUpdateTable& rhs);

   /**
    * @brief Method to add a record.
    * @param record to add.
    */
   void addRecord(const ossimRpfReplaceUpdateRecord& record);

   /** @brief Clears the table. */
   void clear();

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out Stream to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf.rpf." and key is "new_file" key becomes:
    * "nitf.rpf.replace_update_record0.new_file:
    * @return output stream.
    */
   std::ostream& print( std::ostream& out,
                        const std::string& prefix=std::string() ) const;
   
private:

   std::vector<ossimRpfReplaceUpdateRecord> m_table;
};

#endif /* #ifndef ossimRpfReplaceUpdateTable_HEADER */
