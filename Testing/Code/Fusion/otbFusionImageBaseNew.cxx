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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbFusionImageBase.h"

namespace Functor
{
  template <class TInputMultiSpectral, class TInputMultiSpectralInterp, class TInputPanchro, class TOutput>
  class NewFunctorTest
  {
  public:
    NewFunctorTest() {};
    ~NewFunctorTest() {};
    
    inline TOutput operator() (const TInputMultiSpectral & A, const TInputMultiSpectralInterp & B, const TInputPanchro & C)
    {
      return(static_cast<TOutput>(A[0]) + static_cast<TOutput>(B[0]) + static_cast<TOutput>(C));
      
    }
  };
}

int otbFusionImageBaseNew( int argc, char * argv[] )
{
  try 
    { 
      const unsigned int Dimension = 2;
      typedef unsigned char InputPixelType;
      typedef unsigned char OutputPixelType;
      
      typedef otb::Image<InputPixelType,Dimension>       InputPanchroImageType;
      typedef otb::VectorImage<InputPixelType,Dimension> InputMultiSpectralImageType;
      typedef otb::VectorImage<InputPixelType,Dimension> InputMultiSpectralInterpImageType;
      typedef otb::Image<OutputPixelType,Dimension>      OutputImageType;
      
      typedef otb::FusionImageBase<InputMultiSpectralImageType, 
	                           InputMultiSpectralInterpImageType, 
                                   InputPanchroImageType, 
                                   OutputImageType, 
	                           Functor::NewFunctorTest<InputMultiSpectralImageType::PixelType,
                         	                           InputMultiSpectralInterpImageType::PixelType,
	                                                   InputPanchroImageType::PixelType, 
                                	                   OutputImageType::PixelType> 
	                           >  FusionImageBaseType;
      
      // Instantiation
      FusionImageBaseType::Pointer base = FusionImageBaseType::New();
      
    } 
  
  catch( itk::ExceptionObject & err ) 
    { 
      std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
      std::cout << err << std::endl; 
      return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
      std::cout << "Exception levee inconnue !" << std::endl; 
      return EXIT_FAILURE;
    } 
  
  
  return EXIT_SUCCESS;
}


