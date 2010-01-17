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
// $Id: ossimNitfEmbeddedRpfHeader.h 14241 2009-04-07 19:59:23Z dburken $
#ifndef ossimNitfEmbeddedRpfHeader_HEADER
#define ossimNitfEmbeddedRpfHeader_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>
class ossimRpfHeader;

class OSSIMDLLEXPORT ossimNitfEmbeddedRpfHeader : public ossimNitfRegisteredTag
{
public:
   ossimNitfEmbeddedRpfHeader();
   virtual ~ossimNitfEmbeddedRpfHeader();
   /*!
    * This will return the name of the registered tag for this
    * user defined header.
    */
   virtual ossimString getRegisterTagName()const;

   /*!
    * This class will ignore the headerTag since it is the header
    * tag.
    */
   virtual void parseStream(std::istream &in);
   virtual void writeStream(std::ostream& out);

    /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;  
   
   virtual ossim_uint32 getSizeInBytes()const;
   
   virtual const ossimRpfHeader* getRpfHeader()const;
   

TYPE_DATA
private:
   ossimRpfHeader *theRpfHeader;
};

#endif
