/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbGdalDataTypeBridge_h
#define otbGdalDataTypeBridge_h

#include <typeinfo>
#include "gdal.h"

namespace otb
{


namespace GdalDataTypeBridge
{

template <class Type>
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
    if (sizeof(long) == 8)
    {
      // itkWarningMacro(<< "Cast a long (64 bit) image to an int (32 bit) one.")
    }
    return GDT_Int32;
  }
  else if (typeid(Type) == typeid(unsigned long))
  {
    if (sizeof(unsigned long) == 8)
    {
      // itkWarningMacro(<< "Cast an unsigned long (64 bit) image to an unsigned int (32 bit) one.")
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
