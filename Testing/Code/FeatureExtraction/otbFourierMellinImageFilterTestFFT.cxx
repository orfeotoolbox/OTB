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

#include "otbImage.h"
#include "itkImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVnlFFTRealToComplexConjugateImageFilter.h"
#include <complex>

int otbFourierMellinImageFilterTestFFT(int argc, char* argv[])
{
  try 
    {

        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        typedef double                                          InputPixelType;
        typedef std::complex<InputPixelType>                    OutputPixelType;
  	const   unsigned int        	                        Dimension = 2;

        typedef itk::VnlFFTRealToComplexConjugateImageFilter<InputPixelType,Dimension> FourierImageFilterType;

  	typedef otb::Image< InputPixelType, Dimension >         InputImageType;
  	typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;
    
  	FourierImageFilterType::Pointer FourierTransform = FourierImageFilterType::New();

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );

	FourierTransform->SetInput( reader->GetOutput() );
        writer->SetInput( FourierTransform->GetOutput() );
        
        writer->Update(); 
	
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "itk::Exception detected: "  << err.GetDescription();
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "unknown exception detected !" << std::endl; 
    return EXIT_FAILURE;
    } 
  
  return EXIT_SUCCESS;
}
