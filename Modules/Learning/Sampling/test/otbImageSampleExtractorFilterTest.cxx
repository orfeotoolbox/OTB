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

#include "otbImageSampleExtractorFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkPhysicalPointImageSource.h"
#include "itkTimeProbe.h"
#include <fstream>

int otbImageSampleExtractorFilterNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::ImageSampleExtractorFilter<InputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbImageSampleExtractorFilter(int argc, char* argv[])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::ImageSampleExtractorFilter<InputImageType> FilterType;

  if (argc < 3)
    {
    std::cout << "Usage : "<<argv[0]<< "  input_vector  output" << std::endl;
    }

  std::string vectorPath(argv[1]);
  std::string outputPath(argv[2]);

  otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(vectorPath);
  otb::ogr::DataSource::Pointer output =
    otb::ogr::DataSource::New(outputPath,otb::ogr::DataSource::Modes::Overwrite);

  InputImageType::RegionType region;
  region.SetSize(0,99);
  region.SetSize(1,50);

  InputImageType::PointType origin;
  origin.Fill(0.5);

  InputImageType::SpacingType spacing;
  spacing[0] = 1.0;
  spacing[1] = -1.0;

  std::string classFieldName("label");
  std::string outputPrefix("measure_");

  typedef itk::PhysicalPointImageSource<InputImageType> ImageSourceType;
  ImageSourceType::Pointer imgSource = ImageSourceType::New();
  imgSource->SetSize(region.GetSize());
  imgSource->SetSpacing(spacing);
  imgSource->SetOrigin(origin);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(imgSource->GetOutput());
  filter->SetLayerIndex(2);
  filter->SetSamplePositions(vectors);
  filter->SetOutputSamples(output);
  filter->SetClassFieldName(classFieldName);
  filter->SetOutputFieldPrefix(outputPrefix);

  itk::TimeProbe chrono;
  chrono.Start();

  filter->Update();

  chrono.Stop();
  std::cout << "Extraction took "<< chrono.GetTotal() << " sec" << std::endl;

  return EXIT_SUCCESS;
}

int otbImageSampleExtractorFilterUpdate(int argc, char* argv[])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::ImageSampleExtractorFilter<InputImageType> FilterType;

  if (argc < 3)
    {
    std::cout << "Usage : "<<argv[0]<< "  input_vector  output" << std::endl;
    }

  std::string vectorPath(argv[1]);
  std::string outputPath(argv[2]);

  otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(vectorPath);
  otb::ogr::DataSource::Pointer output =
    otb::ogr::DataSource::New(outputPath,otb::ogr::DataSource::Modes::Overwrite);

  InputImageType::RegionType region;
  region.SetSize(0,99);
  region.SetSize(1,50);

  InputImageType::PointType origin;
  origin.Fill(0.5);

  InputImageType::SpacingType spacing;
  spacing[0] = 1.0;
  spacing[1] = -1.0;

  std::string classFieldName("label");
  std::string outputPrefix("measure_");

  // first copy the input layer #3 to output
  otb::ogr::Layer inLayer = vectors->GetLayer(2);
  OGRSpatialReference * oSRS = inLayer.GetSpatialRef()->Clone();
  output->CreateLayer( inLayer.GetName(),
                       oSRS,
                       inLayer.GetLayerDefn().GetGeomType());
  otb::ogr::Layer dstLayer = output->GetLayer(0);
  OGRFieldDefn labelField(classFieldName.c_str(),OFTString);
  dstLayer.CreateField(labelField, true);
  const  OGRErr err = dstLayer.ogr().StartTransaction();

  if (err == OGRERR_NONE) {

  otb::ogr::Layer::const_iterator featIt = inLayer.begin();
  for(; featIt!=inLayer.end(); ++featIt)
    {
    otb::ogr::Feature dstFeature(dstLayer.GetLayerDefn());
    dstFeature.SetFrom( *featIt, TRUE );
    dstLayer.CreateFeature( dstFeature );
    }

  const OGRErr err2 = dstLayer.ogr().CommitTransaction();
  if (err2 == OGRERR_NONE)
    {

  output->Clear();

  otb::ogr::DataSource::Pointer outputUpdate =
    otb::ogr::DataSource::New(outputPath,otb::ogr::DataSource::Modes::Update_LayerUpdate);

  typedef itk::PhysicalPointImageSource<InputImageType> ImageSourceType;
  ImageSourceType::Pointer imgSource = ImageSourceType::New();
  imgSource->SetSize(region.GetSize());
  imgSource->SetSpacing(spacing);
  imgSource->SetOrigin(origin);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(imgSource->GetOutput());
  filter->SetLayerIndex(0);
  filter->SetSamplePositions(outputUpdate);
  filter->SetOutputSamples(outputUpdate);
  filter->SetClassFieldName(classFieldName);
  filter->SetOutputFieldPrefix(outputPrefix);

  itk::TimeProbe chrono;
  chrono.Start();

  filter->Update();

  chrono.Stop();
  std::cout << "Extraction took "<< chrono.GetTotal() << " sec" << std::endl;
    }
  else {
    std::cout<< "Unable to commit transaction for OGR layer " << dstLayer.ogr().GetName() << "." << std::endl;
    return EXIT_FAILURE;
  }

  }
  else {
    std::cout << "Unable to start transaction for OGR layer " << dstLayer.ogr().GetName() << "." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
