//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Unknown tag class declaration.
//
// Note: By "unknown" this means that the tag name was not found in any of
// the tag factories.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfUnknownTag.h 19682 2011-05-31 14:21:20Z dburken $
#ifndef ossimNitfUnknownTag_HEADER
#define ossimNitfUnknownTag_HEADER 1

#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfUnknownTag : public ossimNitfRegisteredTag
{
public:
   /** default constructor */
   ossimNitfUnknownTag();

   /** destructor */
   virtual ~ossimNitfUnknownTag();

   /**
    * @return "tagname" as an std::string.
    *
    * @note: This has to be set by maker as the tag is not in any factory.
    */
   virtual std::string getRegisterTagName() const;
  
   /**
    * Parse method.
    *
    * @param in Stream to parse.  This will read the entire tag length into
    * an array.
    */
   virtual void parseStream(std::istream& in);
   
   /**
    * Write method.
    *
    * @param out Stream to write to.
    */
   virtual void writeStream(std::ostream& out);
   
   /**
    * @return The size of this record in bytes within a nitf file.
    *
    * @ote: This has to be set by maker as the tag is not in any factory.
    */
   virtual ossim_uint32 getSizeInBytes()const;
   
   /**
    * Clears all string fields within the record to some default nothingness.
    */
   virtual void clearFields();
   
   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;

      /**
    * @param tagName Name of tag.
    *
    * @note Users should set tag name as this is an unknown tag.
    */
   void setTagName(const ossimString& tagName);

   /**
    * @param length Length of tag.
    *
    * @note The tag length must be set prior to calling "parseStream".
    *
    * @note Since you must have a length to read theTagData, if theTagData
    * exist it will be deleted by this method.
    * Subsequent "parseStream" calls will allocate theTagData as needed.
    */
   void setTagLength(ossim_uint32 length);
   
protected:

   /**
    * @return true if all characters in theTagData are printable.
    */
   bool tagDataIsAscii() const;

   std::string  theTagName;
   ossim_uint32 theTagLength;

   /**
    * Holds entire tag data(theTagLength) plus one byte for null terminator.
    */
   char*        theTagData;
   
TYPE_DATA   
};

#endif /* End of "#ifndef ossimNitfUnknownTag_HEADER" */
