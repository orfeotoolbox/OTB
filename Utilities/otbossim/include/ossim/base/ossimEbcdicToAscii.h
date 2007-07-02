//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Utility class to convert to/from EBCDIC/ASCII
//
// ASCII = American National Standard Code for Information Interchange
//
// EBCDIC = Extended Binary Coded Decimal Interchange Code  
//
// $Id: ossimEbcdicToAscii.h 9094 2006-06-13 19:12:40Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimEbcdicToAscii_HEADER
#define ossimEbcdicToAscii_HEADER

#include <ossim/base/ossimConstants.h>

class OSSIM_DLL ossimEbcdicToAscii
{
public:

   /** default constructor */
   ossimEbcdicToAscii();

   /** destructor */
   ~ossimEbcdicToAscii();
   
   /**
    * @brief Converts ascii character c to ebcdic character.
    *
    * @param c ascii character to convert.
    *
    * @return ebcdic character.
    */
   ossim_uint8 asciiToEbcdic(ossim_uint8 c) const;

   /**
    * @brief Converts ebcdic character c to ascii character.
    *
    * @param c ebcdic character to convert.
    *
    * @return ascii character.
    */
   ossim_uint8 ebcdicToAscii(ossim_uint8 c) const;
   
   /**
    * @brief Converts ebcdic character string "str" to ascii characters.
    *
    * @param str ebcdic character string to convert.
    *
    * @param size Number of characters to convert.  Note that "str" should be
    * at least >= size.
    */
   void ebcdicToAscii(char* str, ossim_uint32 size) const;
};

#endif /* ossimEbcdicToAcsii_HEADER */
