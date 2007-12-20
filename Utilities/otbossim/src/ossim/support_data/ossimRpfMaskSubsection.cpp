#include <ossim/support_data/ossimRpfMaskSubsection.h>
#include <ossim/support_data/ossimRpfMaskSubheader.h>
#include <ossim/base/ossimErrorCodes.h>

std::ostream& operator <<(std::ostream& out,
                          const ossimRpfMaskSubsection& data)
{
   data.print(out);
   return out;
}

ossimRpfMaskSubsection::ossimRpfMaskSubsection()
   :theMaskSubheader(0)
{
   theMaskSubheader = new ossimRpfMaskSubheader;
}

ossimRpfMaskSubsection::~ossimRpfMaskSubsection()
{
   if(theMaskSubheader)
   {
      delete theMaskSubheader;
      theMaskSubheader = 0;
   }
}

ossimErrorCode ossimRpfMaskSubsection::parseStream(std::istream& in,
                                                   ossimByteOrder byteOrder)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   if(in&&theMaskSubheader)
   {
      result = theMaskSubheader->parseStream(in, byteOrder);
   }
   else
   {
      result = ossimErrorCodes::OSSIM_ERROR;
   }
   
   return result;
}

void ossimRpfMaskSubsection::print(std::ostream& out)const
{
   if(theMaskSubheader)
   {
      out << *theMaskSubheader;
   }
}
