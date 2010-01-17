//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectRecord.cpp 14241 2009-04-07 19:59:23Z dburken $
#include <ossim/support_data/ossimRpfBoundaryRectRecord.h>
#include <string.h> // for memset
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfBoundaryRectRecord& data)
{
   out << setiosflags(ios::fixed)
       << setprecision(12)
       << "theProductDataType:             " << data.theProductDataType << endl
       << "theCompressionRatio:            " << data.theCompressionRatio << endl
       << "theScale:                       " << data.theScale << endl
       << "theZone:                        " << data.theZone << endl
       << "theProducer:                    " << data.theProducer << endl
       << data.theCoverage << endl
       << "theNumberOfFramesNorthSouth:    " << data.theNumberOfFramesNorthSouth << endl
       << "theNumberOfFramesEastWest:      " << data.theNumberOfFramesEastWest;
      
   return out;
}

ossimErrorCode ossimRpfBoundaryRectRecord::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;
      clearFields();
      
      in.read((char*)&theProductDataType, 5);
      in.read((char*)&theCompressionRatio, 5);
      in.read((char*)&theScale, 12);
      in.read((char*)&theZone, 1);
      in.read((char*)&theProducer, 5);
      
      theCoverage.parseStream(in, byteOrder);
      
      in.read((char*)&theNumberOfFramesNorthSouth, 4);
      in.read((char*)&theNumberOfFramesEastWest, 4);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theNumberOfFramesNorthSouth);
         anEndian.swap(theNumberOfFramesEastWest);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectRecord::clearFields()
{
   memset(theProductDataType, ' ', 5);
   memset(theCompressionRatio, ' ', 5);
   memset(theScale, ' ', 12);
   memset(theProducer, ' ', 5);
   theNumberOfFramesNorthSouth = 0;
   theNumberOfFramesEastWest = 0;
   theCoverage.clearFields();

   theProductDataType[5] = '\0';
   theCompressionRatio[5] = '\0';
   theScale[12] = '\0';
   theProducer[5] = '\0';
   theZone = ' ';
}

std::ostream& ossimRpfBoundaryRectRecord::print(std::ostream& out,
                                                ossimString prefix) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();
   
   out << setiosflags(ios::fixed)
       << setprecision(12)
       << prefix << "ProductDataType: "
       << theProductDataType << "\n"
       << prefix << "CompressionRatio: "
       << theCompressionRatio << "\n"
       << prefix << "Scale: "
       << theScale << "\n"
       << prefix << "Zone: "
       << theZone << "\n"
       << prefix << "Producer: "
       << theProducer << "\n";
   
   theCoverage.print(out, prefix);
   
   out << prefix << "NumberOfFramesNorthSouth: "
       << theNumberOfFramesNorthSouth << "\n"
       << prefix << "NumberOfFramesEastWest: "
       << theNumberOfFramesEastWest
       << "\n";

   // Reset flags.
   out.setf(f);

   return out;
}
