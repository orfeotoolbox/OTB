//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTag.h 22013 2012-12-19 17:37:20Z dburken $
#ifndef ossimNitfRegisteredTag_HEADER
#define ossimNitfRegisteredTag_HEADER 1

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimKeywordlist.h>
#include <string>

class ossimKeywordlist;
class ossimString;

class OSSIM_DLL ossimNitfRegisteredTag : public ossimObject ,
                                         public ossimPropertyInterface
{
public:
   /** @brief default constructor */
   ossimNitfRegisteredTag();
   
   /** @brief Constructor that takes name and size. */
   ossimNitfRegisteredTag(const std::string& tagName, ossim_uint32 tagLength);

   /** @brief destructor */
   virtual ~ossimNitfRegisteredTag();
   
   /**
    * @brief This will return the name of the registered tag for this user
    * defined header.
    *
    * @note Deprecated - Use getTagName()
    */
   virtual std::string getRegisterTagName() const;
   
   /**
    * @brief This will return the name of the registered tag for this user
    * defined header.
    */
   virtual const std::string& getTagName() const;

   /**
    * @param tagName Name of tag.
    *
    * @note Users should set tag name as this is an unknown tag.
    */
   virtual void setTagName(const std::string& tagName);
 
   /**
    * @brief Returns the length in bytes of the tag from the CEL or REL field.
    * 
    * @note Depricated use: getTagLength()
    *
    * The entire TRE length is 11 plus this(the size of the CEL or REL field).
    *
    * @return Length of REDATA or CEDATA.
    */
   virtual ossim_uint32 getSizeInBytes()const;

  /**
    * @brief Returns the length in bytes of the tag from the CEL or REL field.
    *
    * The entire TRE length is 11 plus this(the size of the CEL or REL field).
    *
    * @return Length of REDATA or CEDATA.
    */
   virtual ossim_uint32 getTagLength()const;

  /**
    * @brief Set the tag length.
    *
    * @param length Length of tag.
    */
   virtual void setTagLength(ossim_uint32 length);
    
   /**
    * This will allow the user defined data to parse the stream.
    */
   virtual void parseStream(std::istream& in)=0;
   virtual void writeStream(std::ostream& out)=0;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    * @brief Default interface to populate keyword list with metadata.
    * expample:  country_code: US
    * This implementation does nothing.  Derived classes should implement as
    * they see fit.
    * 
    * @param kwl Keywordlist to populate with metadata.
    *
    * @param prefix Optional prefix to put in front of key, like: "image1.".
    */
   virtual void getMetadata(ossimKeywordlist& kwl,
                            const char* prefix=0) const;

   /**
    * @brief Pure virtual print method that outputs a key/value type format
    * adding prefix to keys.
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   
   virtual bool saveState(ossimKeywordlist& kwl, const ossimString& prefix)const;
   
protected:
   std::string  m_tagName;
   ossim_uint32 m_tagLength;
   
TYPE_DATA
};

#endif
