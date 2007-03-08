//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimRpfImageDescriptionSubheader.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfImageDescriptionSubheader.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfImageDescriptionSubheader& data)
{
   data.print(out);

   return out;
}

ossimRpfImageDescriptionSubheader::ossimRpfImageDescriptionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfImageDescriptionSubheader::parseStream(istream& in, ossimByteOrder byteOrder)
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

void ossimRpfImageDescriptionSubheader::print(ostream& out)const
{
   out << "theNumberOfSpectralGroups:               " << theNumberOfSpectralGroups << endl
       << "theNumberOfSubframeTables:               " << theNumberOfSubframeTables << endl
       << "theNumberOfSpectralBandTables:           " << theNumberOfSpectralBandTables << endl
       << "theNumberOfSpectralBandLinesPerImageRow: " << theNumberOfSpectralBandLinesPerImageRow << endl
       << "theNumberOfSubframesHorizontal:          " << theNumberOfSubframesHorizontal << endl
       << "theNumberOfSubframesVertical:            " << theNumberOfSubframesVertical << endl
       << "theNumberOfOutputColumnsPerSubframe:     " << theNumberOfOutputColumnsPerSubframe << endl
       << "theNumberOfOutputRowsPerSubframe:        " << theNumberOfOutputRowsPerSubframe << endl
       << "theSubframeMaskTableOffset:              " << theSubframeMaskTableOffset << endl
       << "theTransparencyMaskTableOffset:          " << theTransparencyMaskTableOffset;
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
