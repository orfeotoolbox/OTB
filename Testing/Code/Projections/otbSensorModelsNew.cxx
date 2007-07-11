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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"
#include <iostream>

#include "otbSensorModelBase.h"
#include "otbForwardSensorModel.h"
#include "otbInverseSensorModel.h"

int otbSensorModelsNew( int argc, char* argv[] )
{
  try 
    {        
        typedef otb::SensorModelBase<double> SensorModelBaseType;
        SensorModelBaseType::Pointer lSensorModelBase = SensorModelBaseType::New();

        typedef otb::InverseSensorModel<double> InverseSensorModelType;
        InverseSensorModelType::Pointer lInverseSensorModel = InverseSensorModelType::New();

        typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
        ForwardSensorModelType::Pointer lForwardSensorModel = ForwardSensorModelType::New();
    } 
  catch( itk::ExceptionObject & err ) 
    { 
      std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
      std::cout << err << std::endl; 
      return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
      std::cout << "Unknown exception thrown !" << std::endl; 
      return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}


