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
#include "itkBinaryMedianImageFilter.h"

#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"

#include "otbImageFileReader.h"
#include "otbImageKeywordlist.h"

int otbOSSIMImageMetaDataReaderTest (int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputAsciiFilenameOtbImage  = argv[2];
  const char * outputAsciiFilenameOtbVectorImage  = argv[3];

  typedef unsigned char  	                                InputPixelType;
  typedef unsigned char  	                                OutputPixelType;
  const   unsigned int        	                        Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
  typedef otb::ImageFileReader< InputImageType  >         ImageReaderType;

  ImageReaderType::Pointer image_reader = ImageReaderType::New();
  image_reader->SetFileName( inputFilename  );

  typedef itk::BinaryMedianImageFilter<InputImageType,InputImageType>  MedianFilterType;
  MedianFilterType::Pointer image_medianFilter = MedianFilterType::New();


  image_medianFilter->SetInput(image_reader->GetOutput());
  image_medianFilter->GetOutput()->UpdateOutputInformation();

  otb::ImageKeywordlist otb_tmp_image;
  /*	itk::ExposeMetaData< otb::ImageKeywordlist >(image_medianFilter->GetOutput()->GetMetaDataDictionary(),
	otb::MetaDataKey::OSSIMKeywordlistKey,
	otb_tmp_image);*/

  otb_tmp_image = image_reader->GetOutput()->GetImageKeywordlist();

  ossimKeywordlist ossim_kwl_image;
  otb_tmp_image.convertToOSSIMKeywordlist(ossim_kwl_image);

  std::cout << " -> otbImage Ossim key word list copy : "<<ossim_kwl_image<<std::endl;

  std::ofstream file;
  file.open(outputAsciiFilenameOtbImage);
  file << "--- OSSIM KEYWORDLIST ---" << std::endl;
  file << ossim_kwl_image;
  file.close();

  typedef otb::VectorImage< InputPixelType,  Dimension >  InputVectorImageType;
  typedef otb::ImageFileReader< InputVectorImageType  >   VectorImageReaderType;

  VectorImageReaderType::Pointer vector_image_reader = VectorImageReaderType::New();
  vector_image_reader->SetFileName( inputFilename  );
  vector_image_reader->GenerateOutputInformation ();

  /*	itk::ExposeMetaData< otb::ImageKeywordlist >(vector_image_reader->GetOutput()->GetMetaDataDictionary(),
	otb::MetaDataKey::OSSIMKeywordlistKey,
	otb_tmp_vector_image);*/
  otb::ImageKeywordlist otb_tmp_vector_image;
  otb_tmp_vector_image = vector_image_reader->GetOutput()->GetImageKeywordlist();

  ossimKeywordlist ossim_kwl_vector_image;
  otb_tmp_vector_image.convertToOSSIMKeywordlist(ossim_kwl_vector_image);

  std::cout << " -> otbVectorImage Ossim key word list copy : "<<ossim_kwl_vector_image<<std::endl;

  //	std::ofstream file;
  file.open(outputAsciiFilenameOtbVectorImage);
  file << "--- OSSIM KEYWORDLIST ---" << std::endl;
  file << ossim_kwl_vector_image;
  file.close();


  return EXIT_SUCCESS;
}
