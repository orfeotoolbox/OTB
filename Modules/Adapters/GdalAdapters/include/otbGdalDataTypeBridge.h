/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbGdalDataTypeBridge_h
#define __otbGdalDataTypeBridge_h

#include <typeinfo>
#include "gdal.h"

namespace otb
{


namespace GdalDataTypeBridge
{

template<class Type>
GDALDataType GetGDALDataType()
{
   if (typeid(Type) == typeid(char))
   {
   return GDT_Byte;
   }
   else if (typeid(Type) == typeid(unsigned char))
   {
   return GDT_Byte;
   }
   else if (typeid(Type) == typeid(unsigned short))
   {
   return GDT_UInt16;
   }
   else if (typeid(Type) == typeid(short))
   {
   return GDT_Int16;
   }
   else if (typeid(Type) == typeid(int))
   {
   return GDT_Int32;
   }
   else if (typeid(Type) == typeid(unsigned int))
   {
   return GDT_UInt32;
   }
   else if (typeid(Type) == typeid(long))
   {
      if( sizeof(long) == 8 )
         {
         //itkWarningMacro(<< "Cast a long (64 bits) image into an int (32 bits) one.")
         }
      return GDT_Int32;
   }
   else if (typeid(Type) == typeid(unsigned long))
   {
      if( sizeof(unsigned long) == 8 )
         {
         //itkWarningMacro(<< "Cast an unsigned long (64 bits) image into an unsigned int (32 bits) one.")
         }
      return GDT_UInt32;
   }
   else if (typeid(Type) == typeid(float))
   {
   return GDT_Float32;
   }
   else if (typeid(Type) == typeid(double))
   {
   return GDT_Float64;
   }
   else
   {
   return GDT_Byte;
   }
}

} // End namespace GdalDataTypeBridge


} // End namespace otb

#endif
