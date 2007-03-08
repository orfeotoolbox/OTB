//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Andrew Huang
//
// Description:
// 
// Class declaration for ossimERSFileWriter.  Meta data class for
// writing an ERS header file.
//
//----------------------------------------------------------------------------
// $Id: ossimERSFileWriter.h 10317 2007-01-19 18:11:58Z dburken $
#ifndef ossimERSFileWriter_H
#define ossimERSFileWriter_H

#include <fstream>
#include <ossim/imaging/ossimMetadataFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/support_data/ossimERS.h>


/**
 * Class for writing a "ERS" style header.
 */
class OSSIM_DLL ossimERSFileWriter : public ossimMetadataFileWriter
{
public:

   /** default constructor */
   ossimERSFileWriter();

   /** virtual destructor */
   virtual ~ossimERSFileWriter();

   /**
    * Initializes the state of the writer.  Users should use this to set
    * the ERS header fields that are not taken from theInputConnection prior
    * to calling execute.
    *
    * @return true on success, false on failure.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix = 0);

   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * Appends the writers image types to the "metadatatypeList".
    * 
    * @param metadatatypeList stl::vector<ossimString> list to append to.
    *
    * @note Appends to the list, does not clear it first.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList) const;

   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * @param imageType string representing image type.
    *
    * @return true if "metadataType" is supported by writer.
    */
   virtual bool hasMetadataType(const ossimString& metadataType)const;   
   
private:
   
   virtual bool writeFile();

   ossimERS theHdr;
   
TYPE_DATA
};

#endif /* End of #ifndef ossimERSFileWriter_H */
