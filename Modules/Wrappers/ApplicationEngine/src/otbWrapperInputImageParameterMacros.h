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
#ifndef __otbWrapperInputImageParameterMacros__h
#define __otbWrapperInputImageParameterMacros__h

#define otbGetImageMacro(image)                       \
  image##Type *                                       \
  InputImageParameter::Get##image ()                  \
  {                                                   \
    return this->GetImage< image##Type > ();          \
  }


#define otbCastImageMacro(InputImageType, OutputImageType, theMethod)   \
  template<> OutputImageType *                                          \
  InputImageParameter::CastImage<InputImageType , OutputImageType>()    \
  {                                                                     \
    return this->theMethod<InputImageType , OutputImageType>();         \
  }

#define otbGenericCastImageMacro(InputImageType, theMethod, prefix)     \
  otbCastImageMacro(InputImageType, UInt8##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, UInt16##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Int16##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, UInt32##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Int32##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Float##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Double##prefix##ImageType, theMethod)


#endif
