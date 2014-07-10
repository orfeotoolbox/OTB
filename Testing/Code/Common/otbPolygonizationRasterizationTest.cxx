/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"

#include "otbLabelImageToVectorDataFilter.h"
#include "otbVectorDataToLabelImageFilter.h"
#include "otbStandardOneLineFilterWatcher.h"

#include "itkNumericTraits.h"
#include "itkImageRegionConstIteratorWithIndex.h"

int otbPolygonizationRasterizationTest(int itkNotUsed(argc), char * argv[])
{
  typedef unsigned int                                          PixelType;
  typedef otb::Image<PixelType, 2>                              ImageType;
  typedef otb::ImageFileReader<ImageType>                       ReaderType;

  typedef otb::VectorData<>                                     VectorDataType;

  typedef otb::LabelImageToVectorDataFilter<ImageType>          LabelImageToVDFilterType;
  typedef otb::VectorDataToLabelImageFilter<VectorDataType,
                                            ImageType>          RasterizationFilterType;

  // Read the label image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  // Instanciate  a LabelImageToVectorData filter
  LabelImageToVDFilterType::Pointer polygonize = LabelImageToVDFilterType::New();
  polygonize->SetInput(reader->GetOutput());
  polygonize->Update();

  // rasterize
  RasterizationFilterType::Pointer  rasterization = RasterizationFilterType::New();
  rasterization->AddVectorData(polygonize->GetOutput());
  rasterization->SetOutputParametersFromImage(reader->GetOutput());
  rasterization->SetBurnAttribute("DN");
  rasterization->Update();

  // Compare the input label image and the output of the rasterization
  // filter, they must be exactly similar
  itk::ImageRegionConstIteratorWithIndex<ImageType> itRef(reader->GetOutput(),
                                                          reader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIteratorWithIndex<ImageType> itTest(rasterization->GetOutput(),
                                                           rasterization->GetOutput()->GetLargestPossibleRegion());

  for(itRef.GoToBegin(), itTest.GoToBegin();
      !itRef.IsAtEnd()  && !itTest.IsAtEnd();
      ++itRef, ++itTest)
    {
    if (itRef.Get() != itTest.Get())
      {
      std::cerr << "Pixel at position " << itRef.GetIndex() << " differs : in=" << itRef.Get() << " while out=" << itTest.Get() << std::endl;
      return EXIT_FAILURE;
      }
    }
  return EXIT_SUCCESS;
}
