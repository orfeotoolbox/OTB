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
#include "otbImageToVectorImageCastFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

int otbImageToVectorImageCastFilter(int argc, char * argv[])
{
try
    {
      const char * infilename = argv[1];
      const char * outfilename = argv[2];
      const unsigned int Dimension = 2;
      typedef double PixelType;
      
      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::VectorImage<PixelType,Dimension> VectorImageType;

      typedef otb::ImageToVectorImageCastFilter<ImageType,VectorImageType> VectorImageCastFilterType;
      typedef otb::ImageFileReader<ImageType> ReaderType;
      typedef otb::ImageFileWriter<VectorImageType> WriterType;

      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(infilename);
      VectorImageCastFilterType::Pointer cast = VectorImageCastFilterType::New();
      cast->SetInput(reader->GetOutput());
      WriterType::Pointer writer = WriterType::New();
      writer->SetFileName(outfilename);
      writer->SetInput(cast->GetOutput());
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
