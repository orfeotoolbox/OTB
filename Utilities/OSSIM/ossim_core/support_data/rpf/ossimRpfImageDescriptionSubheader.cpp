//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfImageDescriptionSubheader.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfImageDescriptionSubheader.h"
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

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
