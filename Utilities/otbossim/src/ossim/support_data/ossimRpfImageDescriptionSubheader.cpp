//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimRpfImageDescriptionSubheader.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <istream>
#include <ostream>
#include <ossim/support_data/ossimRpfImageDescriptionSubheader.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

std::ostream& operator <<(std::ostream& out,
                          const ossimRpfImageDescriptionSubheader& data)
{
   return data.print(out);
}

ossimRpfImageDescriptionSubheader::ossimRpfImageDescriptionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfImageDescriptionSubheader::parseStream(
   std::istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      theStartOffset = in.tellg();
      in.read((char*)&theNumberOfSpectralGroups, 2);
      in.read((char*)&theNumberOfSubframeTables, 2);
      in.read((char*)&theNumberOfSpectralBandTables, 2);
      in.read((char*)&theNumberOfSpectralBandLinesPerImageRow, 2);
      in.read((char*)&theNumberOfSubframesHorizontal, 2);
      in.read((char*)&theNumberOfSubframesVertical, 2);
      in.read((char*)&theNumberOfOutputColumnsPerSubframe, 4);
      in.read((char*)&theNumberOfOutputRowsPerSubframe, 4);
      in.read((char*)&theSubframeMaskTableOffset, 4);
      in.read((char*)&theTransparencyMaskTableOffset, 4);

      theEndOffset = in.tellg();
      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theNumberOfSpectralGroups);
         anEndian.swap(theNumberOfSubframeTables);
         anEndian.swap(theNumberOfSpectralBandTables);
         anEndian.swap(theNumberOfSpectralBandLinesPerImageRow);
         anEndian.swap(theNumberOfSubframesHorizontal);
         anEndian.swap(theNumberOfSubframesVertical);
         anEndian.swap(theNumberOfOutputColumnsPerSubframe);
         anEndian.swap(theNumberOfOutputRowsPerSubframe);
         anEndian.swap(theSubframeMaskTableOffset);
         anEndian.swap(theTransparencyMaskTableOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

std::ostream& ossimRpfImageDescriptionSubheader::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "NumberOfSpectralGroups: "
       << theNumberOfSpectralGroups << "\n"
       << prefix << "NumberOfSubframeTables: "
       << theNumberOfSubframeTables << "\n"
       << prefix << "NumberOfSpectralBandTables: "
       << theNumberOfSpectralBandTables << "\n"
       << prefix << "NumberOfSpectralBandLinesPerImageRow: "
       << theNumberOfSpectralBandLinesPerImageRow << "\n"
       << prefix << "NumberOfSubframesHorizontal: "
       << theNumberOfSubframesHorizontal << "\n"
       << prefix << "NumberOfSubframesVertical: "
       << theNumberOfSubframesVertical << "\n"
       << prefix << "NumberOfOutputColumnsPerSubframe: "
       << theNumberOfOutputColumnsPerSubframe << "\n"
       << prefix << "NumberOfOutputRowsPerSubframe: "
       << theNumberOfOutputRowsPerSubframe << "\n"
       << prefix << "SubframeMaskTableOffset: "
       << theSubframeMaskTableOffset << "\n"
       << prefix << "TransparencyMaskTableOffset: "
       << theTransparencyMaskTableOffset << std::endl;
   return out;
}

void ossimRpfImageDescriptionSubheader::clearFields()
{
   theStartOffset                          = 0;
   theEndOffset                            = 0;
   
   theNumberOfSpectralGroups               = 0;
   theNumberOfSubframeTables               = 0;
   theNumberOfSpectralBandTables           = 0;
   theNumberOfSpectralBandLinesPerImageRow = 0;
   theNumberOfSubframesHorizontal          = 0;
   theNumberOfSubframesVertical            = 0;
   theNumberOfOutputColumnsPerSubframe     = 0;
   theNumberOfOutputRowsPerSubframe        = 0;
   theSubframeMaskTableOffset              = 0;
   theTransparencyMaskTableOffset          = 0;
}
