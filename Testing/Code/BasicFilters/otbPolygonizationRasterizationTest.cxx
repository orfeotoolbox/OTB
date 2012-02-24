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
#include "itkImageRegionConstIterator.h"

int otbPolygonizationRasterizationTest(int argc, char* argv[])
{
  typedef otb::Image<unsigned int, 2>                           ImageType;
  typedef otb::ImageFileReader<ImageType>                       ReaderType;

  typedef otb::VectorData<>                                     VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>             VDReaderType;

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
  ImageType::PixelType status = itk::NumericTraits<ImageType::PixelType>::Zero;
  unsigned long        numberOfPixelsWithDifferences = 0;
  
  itk::ImageRegionConstIterator<ImageType> itRef(reader->GetOutput(),
                                                 reader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<ImageType> itTest(rasterization->GetOutput(),
                                                  rasterization->GetOutput()->GetLargestPossibleRegion());
  itRef.GoToBegin();
  itTest.GoToBegin();
  
  while(!itRef.IsAtEnd()  && !itTest.IsAtEnd())
    {
    ImageType::PixelType diff = vcl_abs(itRef.Get() - itTest.Get());
    if( diff != itk::NumericTraits<ImageType::PixelType>::Zero)
      {
      status += diff;
      numberOfPixelsWithDifferences++;
      }
    ++itRef;
    ++itTest;
    }

  if (status != itk::NumericTraits<ImageType::PixelType>::Zero || numberOfPixelsWithDifferences != 0)
    {
    std::cout <<"Total difference "<< status << "for ["<< numberOfPixelsWithDifferences <<"] pixel(s)" <<std::endl;
    std::cout <<"Polygonization - rasterization bijectivity test failed." << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::cout <<"Bijectivity Test exit with success" << std::endl;
    }

  return EXIT_SUCCESS;
}
