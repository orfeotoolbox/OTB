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

#include "itkPoint.h"
#include "itkPointSetToListAdaptor.h"
#include "itkSubsample.h"
#include "otbSVMClassifier.h"

int otbSVMClassifierNew(int argc, char* argv[] )
{

  try 
    {

    typedef double InputPixelType;
    typedef int LabelPixelType;
    typedef itk::PointSet< InputPixelType, 2 > PointSetType ;
    
    typedef itk::Statistics::PointSetToListAdaptor< PointSetType >
      DataSampleType;
    
    
    typedef otb::SVMClassifier< DataSampleType, LabelPixelType > ClassifierType ;
    
    ClassifierType::Pointer classifier = ClassifierType::New();
    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Unknown exception !" << std::endl; 
    return EXIT_FAILURE;
    } 
  
  
  return EXIT_SUCCESS;
}







