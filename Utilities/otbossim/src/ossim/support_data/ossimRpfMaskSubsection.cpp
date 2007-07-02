#include <ossim/support_data/ossimRpfMaskSubsection.h>
#include <ossim/support_data/ossimRpfMaskSubheader.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfMaskSubsection& data)
{
   data.print(out);
   
   return out;
}

ossimRpfMaskSubsection::ossimRpfMaskSubsection()
   :theMaskSubheader(NULL)
{
   theMaskSubheader = new ossimRpfMaskSubheader;
}

ossimRpfMaskSubsection::~ossimRpfMaskSubsection()
{
   deleteAll();
}

ossimErrorCode ossimRpfMaskSubsection::parseStream(istream& in,
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

void ossimRpfMaskSubsection::print(ostream& out)const
{
   if(theMaskSubheader)
   {
      out << *theMaskSubheader;
   }
}

void ossimRpfMaskSubsection::deleteAll()
{
   if(theMaskSubheader)
   {
      delete theMaskSubheader;
      theMaskSubheader = NULL;
   }
}
