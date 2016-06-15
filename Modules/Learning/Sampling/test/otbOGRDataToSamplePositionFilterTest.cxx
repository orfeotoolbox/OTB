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

#include "otbOGRDataToSamplePositionFilter.h"
#include "otbPatternSampler.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include <fstream>

// hard-coded rates for the multi-layer OGRDataSource (polygon/lines/points)
otb::SamplingRateCalculator::MapRateType GetRatesForMinimumSamples(unsigned int index)
{
  otb::SamplingRateCalculator::MapRateType ratesByClass;
  otb::SamplingRateCalculator::TripletType triplet;
  if (index == 0)
    {
    // Polygons
    triplet.Tot = 104;
    triplet.Required = 104;
    triplet.Rate = 1.0;
    ratesByClass[std::string("1")] = triplet;
    triplet.Tot = 160;
    triplet.Required = 104;
    triplet.Rate = 0.65;
    ratesByClass[std::string("2")] = triplet;
    triplet.Tot = 211;
    triplet.Required = 104;
    triplet.Rate = 0.49289;
    ratesByClass[std::string("3")] = triplet;
    }
  else if (index == 1)
    {
    // Lines
    triplet.Tot = 63;
    triplet.Required = 27;
    triplet.Rate = 0.42857;
    ratesByClass[std::string("1")] = triplet;
    triplet.Tot = 100;
    triplet.Required = 27;
    triplet.Rate = 0.27;
    ratesByClass[std::string("2")] = triplet;
    triplet.Tot = 27;
    triplet.Required = 27;
    triplet.Rate = 1.0;
    ratesByClass[std::string("3")] = triplet;
    }
  else if (index == 2)
    {
    // Points
    triplet.Tot = 2;
    triplet.Required = 1;
    triplet.Rate = 0.5;
    ratesByClass[std::string("1")] = triplet;
    triplet.Tot = 1;
    triplet.Required = 1;
    triplet.Rate = 1.0;
    ratesByClass[std::string("2")] = triplet;
    triplet.Tot = 2;
    triplet.Required = 1;
    triplet.Rate = 0.5;
    ratesByClass[std::string("3")] = triplet;
    }
  return ratesByClass;
}

int TestPositionContainers(otb::ogr::DataSource *output, otb::ogr::DataSource *baseline)
{
  double epsilon=0.01;
  otb::ogr::Layer baselineLayer = baseline->GetLayer(0);
  otb::ogr::Layer outputLayer = output->GetLayer(0);

  otb::ogr::Layer::iterator itBase = baselineLayer.begin();
  for (;itBase != baselineLayer.end(); ++itBase)
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
      otb::ogr::Layer::iterator itOutput = outputLayer.begin();
      for (;itOutput != outputLayer.end(); ++itOutput)
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

int otbOGRDataToSamplePositionFilterNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::Image<unsigned char> MaskImageType;
  typedef otb::OGRDataToSamplePositionFilter<InputImageType , MaskImageType> FilterType;
  
  FilterType::Pointer filter = FilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbOGRDataToSamplePositionFilter(int argc, char* argv[])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::Image<unsigned char> MaskImageType;

  if (argc < 5)
    {
    std::cout << "Usage : "<<argv[0]<< " input_vector_path LayerIndex output_path baseline_path" << std::endl;
    }

  std::string vectorPath(argv[1]);
  int LayerIndex = atoi(argv[2]);
  std::string outputPath(argv[3]);
  std::string baselineVectorPath(argv[4]);

  otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(vectorPath);

  // --------------------- Prepare input data --------------------------------
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

  otb::SamplingRateCalculator::MapRateType ratesByClass =
    GetRatesForMinimumSamples(LayerIndex);

  otb::ogr::DataSource::Pointer output =
    otb::ogr::DataSource::New(outputPath,otb::ogr::DataSource::Modes::Overwrite);

  itk::MetaDataDictionary dict;
  inputImage->SetMetaDataDictionary(dict);
  mask->SetMetaDataDictionary(dict);

  //--------------------------------------------------------------
  typedef otb::OGRDataToSamplePositionFilter<
    InputImageType,MaskImageType> SelectionFilterType;

  SelectionFilterType::Pointer selector = SelectionFilterType::New();
  selector->SetInput(inputImage);
  selector->SetMask(mask);
  selector->SetOGRData(vectors);
  selector->SetOutputPositionContainerAndRates(output,ratesByClass);
  selector->SetFieldName(fieldName);
  selector->SetLayerIndex(LayerIndex);

  selector->Update();

  otb::ogr::DataSource::Pointer baseline = otb::ogr::DataSource::New(baselineVectorPath, otb::ogr::DataSource::Modes::Read);

  return TestPositionContainers(output,baseline);
}

int otbOGRDataToSamplePositionFilterPattern(int argc, char* argv[])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::Image<unsigned char> MaskImageType;

  if (argc < 4)
    {
    std::cout << "Usage : "<<argv[0]<< " input_vector_path output_path baseline_path" << std::endl;
    }

  std::string vectorPath(argv[1]);
  int LayerIndex = 0;
  std::string outputPath(argv[2]);
  std::string baselineVectorPath(argv[3]);

  otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(vectorPath);

  // --------------------- Prepare input data --------------------------------
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

  otb::SamplingRateCalculator::MapRateType ratesByClass =
    GetRatesForMinimumSamples(LayerIndex);

  otb::ogr::DataSource::Pointer output =
    otb::ogr::DataSource::New(outputPath,otb::ogr::DataSource::Modes::Overwrite);

  //--------------------------------------------------------------
  typedef otb::OGRDataToSamplePositionFilter<
    InputImageType,MaskImageType,otb::PatternSampler> SelectionFilterType;

  SelectionFilterType::Pointer selector = SelectionFilterType::New();
  selector->SetInput(inputImage);
  selector->SetMask(mask);
  selector->SetOGRData(vectors);
  selector->SetOutputPositionContainerAndRates(output,ratesByClass);
  selector->SetFieldName(fieldName);
  selector->SetLayerIndex(LayerIndex);

  // set sampling patterns for polygon layer
  std::string patternClass1("1111");
  std::string patternClass2("1001011011111011011011111011100100111001101110101111110110111110101101011110000110111111100111111110010001011010101101111110011101110011001111111110000101000110");
  std::string patternClass3("1000011011111001011011001011100100110001101100101110110110101110101101001100000110011111100111011000010001011010101100011000011100100011001111110110000101000110101101101000001001010010011001010100101000010010110");

  otb::PatternSampler::ParameterType param1, param2, param3;
  param1.Seed = 0UL;
  param2 = param1;
  param3 = param1;
  otb::PatternSampler::ImportPatterns(patternClass1,param1);
  otb::PatternSampler::ImportPatterns(patternClass2,param2);
  otb::PatternSampler::ImportPatterns(patternClass3,param3);

  (selector->GetSamplers())[std::string("1")]->SetParameters(param1);
  (selector->GetSamplers())[std::string("2")]->SetParameters(param2);
  (selector->GetSamplers())[std::string("3")]->SetParameters(param3);

  selector->Update();

  otb::ogr::DataSource::Pointer baseline = otb::ogr::DataSource::New(baselineVectorPath, otb::ogr::DataSource::Modes::Read);

  return TestPositionContainers(output,baseline);
}
