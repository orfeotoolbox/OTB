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
#include "otbNonMaxRemovalByDirectionFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbNonMaxRemovalByDirectionFilter(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::ImageFileReader<ImageType> ReaderType;
      typedef otb::ImageFileWriter<ImageType> WriterType;
      typedef otb::NonMaxRemovalByDirectionFilter<ImageType,ImageType,ImageType> NonMaxRemovalByDirectionFilterType;

      // Instantiating object
      ReaderType::Pointer modReader = ReaderType::New();
      ReaderType::Pointer dirReader = ReaderType::New();
      WriterType::Pointer writer = WriterType::New();
      modReader->SetFileName(argv[1]);
      dirReader->SetFileName(argv[2]);
      writer->SetFileName(argv[3]);
      NonMaxRemovalByDirectionFilterType::Pointer filter = NonMaxRemovalByDirectionFilterType::New();
      filter->SetInput(modReader->GetOutput());
      filter->SetInputDirection(dirReader->GetOutput());
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
