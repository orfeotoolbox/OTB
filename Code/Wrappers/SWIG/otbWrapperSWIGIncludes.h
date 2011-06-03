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
#ifndef __otbWrapperSWIGIncludes_h
#define __otbWrapperSWIGIncludes_h

#include "otbWrapperParameter.h"

typedef itk::LightObject          itkLightObject;
typedef itk::LightObject::Pointer itkLightObject_Pointer;
typedef itk::Object               itkObject;
typedef itk::Object::Pointer      itkObject_Pointer;

typedef otb::Wrapper::Parameter                        Parameter;
typedef otb::Wrapper::Parameter::Pointer::SmartPointer Parameter_Pointer;


#endif
