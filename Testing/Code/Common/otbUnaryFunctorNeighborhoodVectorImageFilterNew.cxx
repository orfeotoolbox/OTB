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
#include "itkExceptionObject.h"

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include "otbVectorImage.h"


namespace Functor
{
  template <class TIter, class TOutput>
  class UnaryFunctorNeighborhoodVectorImageFilterFunctorNewTest
  {
  public:
    UnaryFunctorNeighborhoodVectorImageFilterFunctorNewTest() {};
    ~UnaryFunctorNeighborhoodVectorImageFilterFunctorNewTest() {};
    
    inline TOutput operator() (const TIter & it)
    {
      return(static_cast<TOutput>(it.GetCenterPixel()));
      
    }
  };
}


int otbUnaryFunctorNeighborhoodVectorImageFilterNew(int argc, char * argv[])
{
  try
    {
      typedef double InputPixelType;
      const int Dimension = 2;
      typedef otb::VectorImage<InputPixelType,Dimension> VectorImageType;
      typedef VectorImageType::PixelType PixelType;
      typedef itk::ConstNeighborhoodIterator<VectorImageType>   IterType;;
      typedef Functor::UnaryFunctorNeighborhoodVectorImageFilterFunctorNewTest<IterType, PixelType>  FunctorType;
      typedef otb::UnaryFunctorNeighborhoodVectorImageFilter<VectorImageType, VectorImageType, FunctorType> UnaryFunctorNeighborhoodVectorImageFilterType;
      
      // Instantiating object
      UnaryFunctorNeighborhoodVectorImageFilterType::Pointer object = UnaryFunctorNeighborhoodVectorImageFilterType::New();
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
