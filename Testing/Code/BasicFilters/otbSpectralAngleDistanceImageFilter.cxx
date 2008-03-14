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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSpectralAngleDistanceImageFilter.h"

int otbSpectralAngleDistanceImageFilter(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef otb::VectorImage<PixelType,Dimension> InputImageType;
      typedef otb::Image<PixelType,Dimension> OutputImageType;
      typedef otb::ImageFileReader<InputImageType> ReaderType;
      typedef otb::ImageFileWriter<OutputImageType> WriterType;
      typedef otb::SpectralAngleDistanceImageFilter<InputImageType,OutputImageType> DistanceFilterType;

      // Instantiating object
      DistanceFilterType::Pointer filter = DistanceFilterType::New();
      ReaderType::Pointer reader = ReaderType::New();
      WriterType::Pointer writer = WriterType::New();
      
      reader->SetFileName(argv[1]);
      writer->SetFileName(argv[2]);
      
      InputImageType::PixelType refPixel;
      
      reader->UpdateOutputInformation();
      refPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
      for(unsigned int i = 0; i<reader->GetOutput()->GetNumberOfComponentsPerPixel();++i)
	{
	  refPixel[i]=atoi(argv[3+i]);
	}
      filter->SetInput(reader->GetOutput());
      filter->SetReferencePixel(refPixel);

      writer->SetInput(filter->GetOutput());

      writer->Update();
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
