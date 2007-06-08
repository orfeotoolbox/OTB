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

#include "otbVectorImageToImageListFilter.h"
#include "otbVectorImage.h"
#include "otbImageList.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbVectorImageToImageListFilter(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      char * infname   = argv[1];
      char * outfname1 = argv[2];
      char * outfname2 = argv[3];
      char * outfname3 = argv[4];


      typedef unsigned char PixelType;
      typedef otb::Image<PixelType, Dimension> ImageType;
      typedef otb::VectorImage<PixelType,Dimension> VectorImageType;
      typedef otb::ImageList<ImageType> ImageListType;

      // IO
      typedef otb::ImageFileReader<VectorImageType> ReaderType;
      typedef otb::ImageFileWriter<ImageType> WriterType;

      typedef otb::VectorImageToImageListFilter<VectorImageType,ImageListType> VectorImageToImageListFilterType;

      // Instantiating object
      VectorImageToImageListFilterType::Pointer filter = VectorImageToImageListFilterType::New();

      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(infname);
      reader->Update();

      filter->SetInput(reader->GetOutput());

      filter->Update();
      
      WriterType::Pointer writer = WriterType::New();
      writer->SetFileName(outfname1);
      writer->SetInput(filter->GetOutput()->GetNthElement(0));
      writer->Update();
      
      writer = WriterType::New();
      writer->SetFileName(outfname2);
      writer->SetInput(filter->GetOutput()->GetNthElement(1));
      writer->Update();

      writer = WriterType::New();
      writer->SetFileName(outfname3);
      writer->SetInput(filter->GetOutput()->GetNthElement(2));
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
