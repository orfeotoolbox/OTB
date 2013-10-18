//----------------------------------------------------------------------------
//
// File: ossimNitfJ2klraTag.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: NITF J2KLRA tag.
//
// See:  ISO/IEC BIIF Profile BPJ2K01.00 Table 9-3.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimNitfJ2klraTag_HEADER
#define ossimNitfJ2klraTag_HEADER 1

#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/base/ossimConstants.h>

#include <iosfwd>
#include <vector>

class OSSIM_DLL ossimNitfJ2klraTag : public ossimNitfRegisteredTag
{
public:
   enum 
   {
      ORIG_SIZE      = 1,
      NLEVELS_O_SIZE = 2,
      NBANDS_O_SIZE  = 5,
      NLAYERS_O_SIZE = 3,
      
      LAYER_ID_SIZE  = 3, // repeating
      BITRATE_SIZE   = 9, // repeating

      NLEVELS_I_SIZE = 2, // conditional
      NBANDS_I_SIZE  = 5, // conditional
      NLAYERS_I_SIZE = 3  // conditional
   };
   
   /** default constructor */
   ossimNitfJ2klraTag();

   /** @return "J2KLRA" as a std:;string. */
   virtual std::string getRegisterTagName() const;
  
   /**
    * Parse method.
    *
    * @param in Stream to parse.
    */
   virtual void parseStream(std::istream& in);
   
   /**
    * Write method.
    *
    * @param out Stream to write to.
    */
   virtual void writeStream(std::ostream& out);
   
   /** @return The size of this record in bytes (123) within a nitf file. */
   virtual ossim_uint32 getSizeInBytes()const;
   
   /**
    * Clears all string fields within the record to some default nothingness.
    */
   virtual void clearFields();

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out, const std::string& prefix) const;

   /** @return ORIG field as a number. */
   ossim_uint32 getOriginNumber() const;

   /** @return NLAYERS_O field as a number. */
   ossim_uint32 getNumberOfLayersOriginal() const;

protected:

   // Container for repeating fields.
   class ossimJ2klraLayer
   {
   public:
      /**
       * FIELD: LAYER_ID
       * required 3 byte field
       * 000 - 999
       * Layer ID Number. Repeating.
       */
      char m_layer_id[LAYER_ID_SIZE+1];
      
      /**
       * FIELD: BITRATE
       * required 9 byte
       * 00.000000 – 37.000000
       * Bitrate
       */
      char m_bitrate[BITRATE_SIZE+1];
   };

   /**
    * FIELD: ORIG
    *
    * Required 1 byte.
    *
    * 0 - Original NPJE
    * 1 – Parsed NPJE
    * 2 – Original EPJE*
    * 3 – Parsed EPJE*
    * 8 – Original other
    * 9 – Parsed other
    *
    * Original compressed data.
    */
   char m_orig[ORIG_SIZE+1];
   
   /**
    * FIELD: NLEVELS_O
    * required 2 bytes
    * 00 - 32
    * Number of wavelet levels in original image.
    */
   char m_levels_o[NLEVELS_O_SIZE+1];
   
   /**
    * FIELD: NBANDS_O
    * required 5 bytes
    * 00000 - 16384
    * Number of bands in original image.
    */
   char m_bands_o[NBANDS_O_SIZE+1];
   
   /**
    * FIELD: NLAYERS_O
    * required 3 bytes
    * 000 - 999
    * Number of layers in original image.
    */
   char m_layers_o[NLAYERS_O_SIZE+1];

   /** repeating fields */
   std::vector<ossimJ2klraLayer> m_layer;
   
   /**
    * FIELD: NLEVELS_I
    * Conditional 2 bytes if ORIG=1, 3, or 9.
    * 00 - 32
    * Number of wavelet levels in this image.
    */
   char m_nlevels_i[NLEVELS_I_SIZE+1];
   
   /**
    * FIELD: NBANDS_I
    * Conditional 5 bytes if ORIG=1, 3,or 9.
    * 00000 = 16384
    * Number of bands in this image.
    */
   char m_nbands_i[NBANDS_I_SIZE+1];
   
   /**
    * FIELD: NLAYERS_I
    * Conditional 3 bytes if ORIG=1, 3, or 9.
    * 000 - 999
    * Number of Layers in this image.
    */
   char m_nlayers_i[NLAYERS_I_SIZE+1];
   
TYPE_DATA   
};

#endif /* End of "#ifndef ossimNitfJ2klraTag_HEADER_HEADER" */
