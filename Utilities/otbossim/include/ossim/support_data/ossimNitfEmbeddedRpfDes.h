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
// $Id: ossimNitfEmbeddedRpfDes.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimNitfEmbeddedRpfDes_HEADER
#define ossimNitfEmbeddedRpfDes_HEADER

#include <iosfwd>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
class ossimNitfFileHeader;

class OSSIMDLLEXPORT ossimNitfEmbeddedRpfDes : public ossimNitfRegisteredTag
{
public:
   ossimNitfEmbeddedRpfDes();
   /*!
    * This will return the name of the registered tag for this
    * user defined header.
    */
   virtual ossimString getRegisterTagName()const{return "RPFDES";}

   virtual void parseStream(std::istream &in);

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
protected:
   virtual ~ossimNitfEmbeddedRpfDes();

TYPE_DATA
private:

};

#endif
