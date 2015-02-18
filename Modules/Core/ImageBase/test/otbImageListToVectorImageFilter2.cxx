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

#include "otbImageListToVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageFileReader.h"

int otbImageListToVectorImageFilter2(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  char * infname1 = argv[1];
  char * infname2 = argv[2];
  char * infname3 = argv[3];
//   char * outfname = argv[4];

  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
//   typedef VectorImageType::Pointer              VectorImagePointerType;
  typedef otb::ImageList<ImageType> ImageListType;

  // IO
  typedef otb::ImageFileReader<ImageType> ReaderType;
//   typedef otb::ImageFileWriter<VectorImageType> WriterType;

  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType> ImageListToVectorImageFilterType;

  // Instantiating object
  ImageListToVectorImageFilterType::Pointer filter = ImageListToVectorImageFilterType::New();

  // Building image list

  ReaderType::Pointer reader1, reader2, reader3;

  reader1 = ReaderType::New();
  reader1->SetFileName(infname1);

  reader2 = ReaderType::New();
  reader2->SetFileName(infname2);

  reader3 = ReaderType::New();
  reader3->SetFileName(infname3);

  ImageListType::Pointer imageList = ImageListType::New();
  imageList->PushBack(reader1->GetOutput());
  imageList->PushBack(reader2->GetOutput());
  imageList->PushBack(reader3->GetOutput());

  filter->SetInput(imageList);

  VectorImageType::Pointer m_OutputImage = VectorImageType::New();
  m_OutputImage = filter->GetOutput();

  m_OutputImage->UpdateOutputInformation();

  unsigned int nbComp1 = m_OutputImage->GetNumberOfComponentsPerPixel();

  imageList->PopBack();

  m_OutputImage->UpdateOutputInformation();

  unsigned int nbComp2 = m_OutputImage->GetNumberOfComponentsPerPixel ();
  std::cout << "component 1: " << nbComp1 << std::endl;
  std::cout << "component 2: " << nbComp2 << std::endl;

  if (nbComp2 != nbComp1 - 1) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
