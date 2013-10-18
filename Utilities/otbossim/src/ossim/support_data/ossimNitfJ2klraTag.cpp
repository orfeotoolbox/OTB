//----------------------------------------------------------------------------
//
// File: ossimNitfJ2klraTag.cpp
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


#include <ossim/support_data/ossimNitfJ2klraTag.h>
#include <ossim/base/ossimString.h>

#include <cstring> /* for memcpy */
#include <iomanip>
#include <istream>
#include <ostream>

RTTI_DEF1(ossimNitfJ2klraTag, "ossimNitfJ2klraTag", ossimNitfRegisteredTag);

ossimNitfJ2klraTag::ossimNitfJ2klraTag()
   : m_layer(1)
{
   clearFields();
}

std::string ossimNitfJ2klraTag::getRegisterTagName() const
{
   return std::string("J2KLRA");
}

void ossimNitfJ2klraTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(m_orig, ORIG_SIZE);
   in.read(m_levels_o, NLEVELS_O_SIZE);
   in.read(m_bands_o, NBANDS_O_SIZE);
   in.read(m_layers_o, NLAYERS_O_SIZE);

   ossim_uint32 layers = getNumberOfLayersOriginal();
   if ( layers && (layers < 1000) ) // 999 max
   {
      m_layer.resize(layers);
      for ( ossim_uint32 i = 0; i < layers; ++i )
      {
         in.read(m_layer[i].m_layer_id, LAYER_ID_SIZE);
         in.read(m_layer[i].m_bitrate, BITRATE_SIZE);
         m_layer[i].m_layer_id[LAYER_ID_SIZE] = '\0';
         m_layer[i].m_bitrate[BITRATE_SIZE]   = '\0';
      }
   }
   else
   {
      m_layer.clear();
   }

   // Conditional:
   ossim_uint32 orig = getOriginNumber();
   if ( (orig == 1) || ( orig == 3 ) || ( orig == 9 ) )
   {
      in.read(m_nlevels_i, NLEVELS_I_SIZE);
      in.read(m_nbands_i, NBANDS_I_SIZE);
      in.read(m_nlayers_i, NLAYERS_I_SIZE);
   }   
}

void ossimNitfJ2klraTag::writeStream(std::ostream& out)
{
   out.write(m_orig, ORIG_SIZE);
   out.write(m_levels_o, NLEVELS_O_SIZE);
   out.write(m_bands_o, NBANDS_O_SIZE);
   out.write(m_layers_o, NLAYERS_O_SIZE);
   
   ossim_uint32 size = m_layer.size();
   for ( ossim_uint32 i = 0; i < size; ++i )
   {
      out.write(m_layer[i].m_layer_id, LAYER_ID_SIZE);
      out.write(m_layer[i].m_bitrate, BITRATE_SIZE);
   }

   // Conditional:
   ossim_uint32 orig = getOriginNumber();
   if ( (orig == 1) || ( orig == 3 ) || ( orig == 9 ) )
   {
      out.write(m_nlevels_i, NLEVELS_I_SIZE);
      out.write(m_nbands_i, NBANDS_I_SIZE);
      out.write(m_nlayers_i, NLAYERS_I_SIZE);
   }
}

ossim_uint32 ossimNitfJ2klraTag::getSizeInBytes()const
{
   ossim_uint32 result = 11 + (m_layer.size() * 12);

   // Conditional:
   ossim_uint32 orig = getOriginNumber();
   if ( (orig == 1) || ( orig == 3 ) || ( orig == 9 ) )
   {
      result += 10;
   }
   return result;
}

void ossimNitfJ2klraTag::clearFields()
{
   memset(m_orig, '0', ORIG_SIZE);

   memset(m_levels_o, '0', NLEVELS_O_SIZE);
   memset(m_bands_o, '0', NBANDS_O_SIZE);
   memset(m_layers_o, '0', NLAYERS_O_SIZE);

   m_layer.clear();

   memset(m_nlevels_i, '0', NLEVELS_I_SIZE);
   memset(m_nbands_i, '0', NBANDS_I_SIZE);
   memset(m_nlayers_i, '0', NLAYERS_I_SIZE);
   
   m_orig[ORIG_SIZE]= '\0';
   
   m_levels_o[NLEVELS_O_SIZE]= '\0';
   m_bands_o[NBANDS_O_SIZE]= '\0';
   m_layers_o[NLAYERS_O_SIZE]= '\0';

   m_nlevels_i[NLEVELS_I_SIZE]= '\0';
   m_nbands_i[NBANDS_I_SIZE] = '\0';
   m_nlayers_i[NLAYERS_I_SIZE] = '\0';
}

std::ostream& ossimNitfJ2klraTag::print(std::ostream& out,
                                        const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(ios::left)
       << pfx << std::setw(24) << "CETAG:"     << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"       << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "ORIG:"      << m_orig << "\n"
       << pfx << std::setw(24) << "NLEVELS_O:" << m_levels_o << "\n"
       << pfx << std::setw(24) << "NBANDS_O:"  << m_bands_o << "\n"
       << pfx << std::setw(24) << "NLAYERS_O:" << m_layers_o << "\n";

   ossim_uint32 size = m_layer.size();
   for ( ossim_uint32 i = 0; i < size; ++i )
   {
      out << pfx << "LAYER_ID[" << i << std::setw(14) << "]:" << m_layer[i].m_layer_id << "\n"
          << pfx << "BITRATE[" << i << std::setw(15) << "]:" << m_layer[i].m_bitrate << "\n";
   }

   // Conditional:
   ossim_uint32 orig = getOriginNumber();
   if ( (orig == 1) || ( orig == 3 ) || ( orig == 9 ) )
   {
      out << pfx << std::setw(24) << "NLEVELS_I:"     << m_nlevels_i << "\n"
          << pfx << std::setw(24) << "NBANDS_I_SIZE:" << m_nbands_i << "\n"
          << pfx << std::setw(24) << "NLAYERS_I:"     << m_nlayers_i << "\n";
   }
   return out;
}
   
ossim_uint32 ossimNitfJ2klraTag::getOriginNumber() const
{
   return ossimString(m_orig).toUInt32();
}
   
ossim_uint32 ossimNitfJ2klraTag::getNumberOfLayersOriginal() const
{
   return ossimString(m_layers_o).toUInt32();
}
