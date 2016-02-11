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

#include "otbOGRDataToClassStatisticsFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

int otbOGRDataToClassStatisticsFilterNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::Image<unsigned char> MaskImageType;
  typedef otb::OGRDataToClassStatisticsFilter<InputImageType,MaskImageType> FilterType;
  
  FilterType::Pointer filter = FilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbOGRDataToClassStatisticsFilter(int argc, char* argv[])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::Image<unsigned char> MaskImageType;
  typedef otb::OGRDataToClassStatisticsFilter<InputImageType,MaskImageType> FilterType;
  
  if (argc < 3)
    {
    std::cout << "Usage : "<<argv[0]<< " input_vector  output_stats" << std::endl;
    }
  
  std::string vectorPath(argv[1]);
  std::string outputPath(argv[2]);
  
  otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(vectorPath);
  
  InputImageType::RegionType region;
  region.SetSize(0,100);
  region.SetSize(1,50);
  
  InputImageType::PointType origin;
  origin.Fill(0.5);
  
  InputImageType::PixelType pixel(3);
  pixel.Fill(1);
  
  InputImageType::Pointer inputImage = InputImageType::New();
  inputImage->SetNumberOfComponentsPerPixel(3);
  inputImage->SetRegions(region);
  inputImage->SetOrigin(origin);
  inputImage->Allocate();
  inputImage->FillBuffer(pixel);
  
  MaskImageType::Pointer mask = MaskImageType::New();
  mask->SetRegions(region);
  mask->SetOrigin(origin);
  mask->Allocate();
  itk::ImageRegionIterator<MaskImageType> it(mask,region);
  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd() ; ++it, ++count)
    {
    it.Set(count % 2);
    }
    
  std::string fieldName("Label");
  
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(inputImage);
  filter->SetMask(mask);
  filter->SetOGRData(vectors);
  filter->SetFieldName(fieldName);
  
  filter->Update();
  
  FilterType::ClassCountMapType &classCount = filter->GetClassCountOutput()->Get();
  FilterType::PolygonSizeMapType &polySize = filter->GetPolygonSizeOutput()->Get();
  
  FilterType::ClassCountMapType::const_iterator itClass = classCount.begin();
  
  
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}
