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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageList.h"
#include "otbImage.h"

int otbImageList(int argc, char * argv[])
{
  try
    {
      const char * inputFilename = argv[1];
      const char * outputFilename = argv[2];
      const unsigned int Dimension = 2;

      typedef unsigned char InputPixelType;
      typedef otb::Image<InputPixelType,Dimension> InputImageType;
      typedef otb::ImageFileReader<InputImageType> ReaderType;
      typedef otb::ImageFileWriter<InputImageType> WriterType;
      typedef otb::ImageList<InputImageType> ImageListType;

      // Reading image
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(inputFilename);
      reader->Update();

      // Instantiating ImageList object
      ImageListType::Pointer imageList = ImageListType::New();

      // Appending one image to the list
      imageList->PushBack(reader->GetOutput());
     
      // Getting the image from the list and writing it to file
      WriterType::Pointer writer = WriterType::New();
      writer->SetFileName(outputFilename);
      writer->SetInput(imageList->Back());
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
