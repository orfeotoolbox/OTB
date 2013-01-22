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
#include "itkMacro.h"

#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToSingleImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageList.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBandMathImageFilter.h"

int otbImageListToSingleImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;

  typedef unsigned char                          PixelType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::ImageList<ImageType>              ImageListType;

  typedef otb::ImageListToSingleImageFilter<ImageListType::ImageType> ImageListToSingleImageFilter;

  // Instantiating object
  ImageListToSingleImageFilter::Pointer filter = ImageListToSingleImageFilter::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}



int otbImageListToSingleImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  char *             infname   = argv[1];
  char *             outfname1 = argv[2];

  typedef unsigned char                          PixelType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::ImageList<ImageType>              ImageListType;

  // IO
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType> VectorImageToImageListFilterType;

  typedef otb::ImageListToSingleImageFilter<ImageListType::ImageType> ImageListToSingleImageFilter;

  typedef otb::BandMathImageFilter<ImageType> BandMathImageFilterType ;


  // Instantiating object
  VectorImageToImageListFilterType::Pointer filter1 = VectorImageToImageListFilterType::New();

  ImageListToSingleImageFilter::Pointer filter2_1 = ImageListToSingleImageFilter::New();
  ImageListToSingleImageFilter::Pointer filter2_2 = ImageListToSingleImageFilter::New();

  BandMathImageFilterType::Pointer filter3 = BandMathImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter1->SetInput(reader->GetOutput());
  filter1->UpdateOutputInformation();

  filter2_1->SetExtractedImagePosition(1);
  filter2_1->SetInput(filter1->GetOutput());

  filter2_2->SetExtractedImagePosition(2);
  filter2_2->SetInput(filter1->GetOutput());

  filter3->SetNthInput(0,filter2_1->GetOutput());
  filter3->SetNthInput(1,filter2_2->GetOutput());
  filter3->SetExpression("b1/b2");
  filter3->SetExpression(" if( b2 > 1.0, b1/b2, 0)" );


  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname1);
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->SetInput(filter3->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
