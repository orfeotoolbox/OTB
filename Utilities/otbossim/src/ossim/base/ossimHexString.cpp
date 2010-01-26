//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// 
//********************************************************************
// $Id: ossimHexString.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/base/ossimHexString.h>

ossim_int32 ossimHexString::toInt32()const
{
   return static_cast<ossim_int32>(toUint32());
}

ossim_int16 ossimHexString::toInt16()const
{
   return static_cast<ossim_int16>(toUint32());
}

ossim_int8 ossimHexString::toChar()const
{
   return static_cast<ossim_int8>(toUint32());
}

ossim_uint8 ossimHexString::toUchar()const
{   
   return static_cast<ossim_uint8>(toUint32());
}

ossim_uint16 ossimHexString::toUint16()const
{
   return static_cast<ossim_uint16>(toUint32());
}

ossim_uint32 ossimHexString::toUint32()const
{
   ossim_uint32 temp=0;

   if(length())
   {
      ossim_uint32 i = 0;
      
      if(((*this)[0] == '0')&&
         (((*this)[1] == 'x')||
          ((*this)[1] == 'X')))
      {
         i = 2;
      }
         
      for(; i <length(); ++i)
      {
         temp <<=4;
         if( ((*this)[(int)i] >= '0')&&((*this)[(int)i] <= '9'))
         {
            temp |= (ossim_uint32)((*this)[(int)i] - '0');
         }
         else if( ((*this)[(int)i] >= 'A') && ((*this)[(int)i] <= 'F'))
         {
            temp |= (ossim_uint32)(10+((*this)[(int)i] - 'A'));
         }
         else if( ((*this)[(int)i] <= 'f') && ((*this)[(int)i] >= 'a'))
         {
            temp |= (ossim_uint32)(10 + ((*this)[(int)i] - 'a'));
         }
      }
   }
   
   return temp;
}

void ossimHexString::assign(ossim_uint8 value)
{
   *this = "";
   
   ossim_int8 v1 = (ossim_int8)(value&0x0F);
   ossim_int8 v2 = (ossim_int8)((value>>4)&0x0F);
   if(v2 <=9)
   {
      (*this) += (ossim_int8)(v2+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v2-10 +'A');
   }

   if(v1 <=9)
   {
      (*this) += (ossim_int8)(v1+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v1-10 +'A');
   }   
}

void ossimHexString::assign(ossim_uint16 value)
{
   *this = "";
   
   ossim_int8 v1 = (ossim_int8)(value&0x000F);
   value >>=4;
   ossim_int8 v2 = (ossim_int8)((value)&0x000F);
   value >>=4;
   ossim_int8 v3 = (ossim_int8)((value)&0x000F);
   value >>=4;
   ossim_int8 v4 = (ossim_int8)((value)&0x000F);

   if(v4 <=9)
   {
      (*this) += (ossim_int8)(v4+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v4-10 +'A');
   }

   if(v3 <=9)
   {
      (*this) += (ossim_int8)(v3+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v3-10 +'A');
   }

   if(v2 <=9)
   {
      (*this) += (ossim_int8)(v2+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v2-10 +'A');
   }

   if(v1 <=9)
   {
      (*this) += (ossim_int8)(v1+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v1-10 +'A');
   }
}

void ossimHexString::assign(ossim_uint32 value)
{
   *this = "";

   ossim_int8 v1 = (ossim_int8)(value&0x0000000F);
   value >>=4;
   ossim_int8 v2 = (ossim_int8)((value)&0x0000000F);
   value >>=4;
   ossim_int8 v3 = (ossim_int8)((value)&0x0000000F);
   value >>=4;
   ossim_int8 v4 = (ossim_int8)((value)&0x0000000F);
   value >>=4;
   ossim_int8 v5 = (ossim_int8)((value)&0x0000000F);
   value >>=4;
   ossim_int8 v6 = (ossim_int8)((value)&0x0000000F);
   value >>=4;
   ossim_int8 v7 = (ossim_int8)((value)&0x0000000F);
   value >>=4;
   ossim_int8 v8 = (ossim_int8)((value)&0x0000000F);

   if(v8 <=9)
   {
      (*this) += (ossim_int8)(v8+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v8-10 +'A');
   }
   if(v7 <=9)
   {
      (*this) += (ossim_int8)(v7+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v7-10 +'A');
   }
   if(v6 <=9)
   {
      (*this) += (ossim_int8)(v6+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v6-10 +'A');
   }
   if(v5 <=9)
   {
      (*this) += (ossim_int8)(v5+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v5-10 +'A');
   }

   if(v4 <=9)
   {
      (*this) += (ossim_int8)(v4+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v4-10 +'A');
   }

   if(v3 <=9)
   {
      (*this) += (ossim_int8)(v3+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v3-10 +'A');
   }

   if(v2 <=9)
   {
      (*this) += (ossim_int8)(v2+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v2-10 +'A');
   }

   if(v1 <=9)
   {
      (*this) += (ossim_int8)(v1+'0');
   }
   else
   {
      (*this) += (ossim_int8)(v1-10 +'A');
   }
}
