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
// $Id: ossimERSFileWriter.h 15766 2009-10-20 12:37:09Z gpotts $
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
   
protected:
   /** virtual destructor */
   virtual ~ossimERSFileWriter();
   
   virtual bool writeFile();

   ossimERS theHdr;
   
TYPE_DATA
};

#endif /* End of #ifndef ossimERSFileWriter_H */
