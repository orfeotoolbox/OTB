#include <ossim/support_data/ossimRpfColorGrayscaleSubheader.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfColorGrayscaleSubheader& data)
{
   data.print(out);

   return out;
}

ossimRpfColorGrayscaleSubheader::ossimRpfColorGrayscaleSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfColorGrayscaleSubheader::parseStream(istream& in,
                                                            ossimByteOrder /* byteOrder */)
{
   if(in)
   {
      clearFields();
      
      theStartOffset = in.tellg();
      in.read((char*)&theNumberOfColorGreyscaleOffsetRecords, 1);
      in.read((char*)&theNumberOfColorConverterOffsetRecords, 1);

      char tempString[12];
      in.read(tempString, 12);
      theColorGrayscaleFilename = ossimString(tempString,
                                              (char*)(&tempString[12]));
      theEndOffset = in.tellg();
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfColorGrayscaleSubheader::print(ostream& out)const
{
   out << "theNumberOfColorGreyscaleOffsetRecords:    " << (int)theNumberOfColorGreyscaleOffsetRecords << endl
       << "theNumberOfColorConverterOffsetRecords:    " << (int)theNumberOfColorConverterOffsetRecords << endl
       << "theColorGrayscaleFilename:                 " << theColorGrayscaleFilename;
   
}

void ossimRpfColorGrayscaleSubheader::clearFields()
{
   theStartOffset = 0;
   theEndOffset   = 0;
   
   theNumberOfColorGreyscaleOffsetRecords = 0;
   theNumberOfColorConverterOffsetRecords = 0;
   theColorGrayscaleFilename              = "";
}

