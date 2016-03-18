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

#include "otbSamplingRateCalculator.h"
#include "otbOGRDataToResampledOGRData.h"
#include "otbOGRDataToClassStatisticsFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include <fstream>

int otbOGRDataToResampledOGRDataNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::Image<unsigned char> MaskImageType;
  typedef otb::OGRDataToResampledOGRData<MaskImageType> FilterType;
  
  FilterType::Pointer filter = FilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbOGRDataToResampledOGRData(int argc, char* argv[])
{

  typedef otb::VectorImage<float> InputImageType;
  typedef otb::Image<unsigned char> MaskImageType;
  typedef otb::OGRDataToClassStatisticsFilter<InputImageType,MaskImageType> FilterType;
  
  if (argc < 3)
    {
    std::cout << "Usage : "<<argv[0]<< " input_vector  output_stats" << std::endl;
    }
  
  
  std::string vectorPath(argv[1]);
  std::string samplingVectorsPath(argv[2]);
  int LayerIndex = atoi(argv[3]);
  std::string outputPath(argv[4]);
  std::string baselineVectorPath(argv[5]);
  
  
  otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(vectorPath);
  
  InputImageType::RegionType region;
  region.SetSize(0,99);
  region.SetSize(1,50);
  
  InputImageType::PointType origin;
  origin.Fill(0.5);
  
  InputImageType::SpacingType spacing;
  spacing[0] = 1.0;
  spacing[1] = -1.0;
  
  InputImageType::PixelType pixel(3);
  pixel.Fill(1);
  
  InputImageType::Pointer inputImage = InputImageType::New();
  inputImage->SetNumberOfComponentsPerPixel(3);
  inputImage->SetLargestPossibleRegion(region);
  inputImage->SetOrigin(origin);
  inputImage->SetSpacing(spacing);
  // Don't allocate the input image, the filter should not need it
  //inputImage->Allocate();
  //inputImage->FillBuffer(pixel);
  
  MaskImageType::Pointer mask = MaskImageType::New();
  mask->SetRegions(region);
  mask->SetOrigin(origin);
  mask->SetSpacing(spacing);
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
  filter->SetLayerIndex(LayerIndex);
  
  filter->Update();
  
  FilterType::ClassCountMapType &classCount = filter->GetClassCountOutput()->Get();

    
   //-------------------------------------------------------------- 
  typedef otb::OGRDataToResampledOGRData<MaskImageType> ResamplerFilterType;  
  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  rateCalculator->SetClassCount(classCount);
  rateCalculator->setMinimumNbofSamplesByClass();
  RateCalculatorype::mapRateType ratesbyClass = rateCalculator->GetRatesbyClass();
    
  
  ResamplerFilterType::Pointer resampler = ResamplerFilterType::New();
  resampler->SetOutputVectorDataPath(outputPath);
  resampler->SetMask(mask);
  resampler->SetOGRData(vectors);
  resampler->SetRatesbyClass(ratesbyClass);
  resampler->SetFieldName(fieldName);
  resampler->SetLayerIndex(LayerIndex);
  //resampler->SetMaxSamplingTabSize(81);
  //resampler->SetOutputSamplingVectorsPath(samplingVectorsPath);
  resampler->SetInputSamplingVectorsPath(samplingVectorsPath);
  
  resampler->Update();
  
  
  //TEST itself
  double epsilon=0.01;
  
  otb::ogr::DataSource::Pointer baseline = otb::ogr::DataSource::New(baselineVectorPath, otb::ogr::DataSource::Modes::Read); 
  otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(outputPath, otb::ogr::DataSource::Modes::Read);
  
  otb::ogr::Layer::iterator itBase = baseline->GetLayer(0).begin();
  for (;itBase != baseline->GetLayer(0).end(); ++itBase)
  {

    const OGRGeometry* cstpgeomBase = itBase->GetGeometry();
    OGRGeometry* pgeomBase = cstpgeomBase->clone();
    OGRPoint* castPointBase = dynamic_cast<OGRPoint*>(pgeomBase);
    if (castPointBase == NULL)
    {
        std::cerr << "Could not dynamic_cast pgeomBase" << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
       bool found=false; 
       otb::ogr::Layer::iterator itOutput = output->GetLayer(0).begin();
       for (;itBase != output->GetLayer(0).end(); ++itOutput)
       {
           const OGRGeometry* cstpgeomOutput = itOutput->GetGeometry();
           OGRGeometry* pgeomOutput = cstpgeomOutput->clone();
           OGRPoint* castPointOutput = dynamic_cast<OGRPoint*>(pgeomOutput);
           if (castPointOutput == NULL)
           {
              std::cerr << "Could not dynamic_cast pgeomOutput" << std::endl;
              return EXIT_FAILURE;
           }
           else
           {
              if ( (fabs(castPointBase->getX()-castPointOutput->getX())<epsilon) && (fabs(castPointBase->getY()-castPointOutput->getY())<epsilon) )
              {
                 found=true;
                 break;
              }
           }
       
       }
       if(!found)
       {
           unsigned long featureId = itBase->ogr().GetFID();
           std::cerr << "Could not find point ("<< castPointBase->getX() << "," << castPointBase->getY() << "); feature ID = " << featureId << "." << std::endl;
           return EXIT_FAILURE;
       }
    }
  
        
  }
  

  return EXIT_SUCCESS;
}
