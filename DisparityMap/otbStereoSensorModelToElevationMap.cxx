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
#include "otbStereoSensorModelToElevationMap.h"

#include "otbImage.h"
#include "otbStereoSensorModelToElevationMapFilter.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStandardWriterWatcher.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbVectorImage.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkMedianImageFilter.h"
#include <iostream>

#include "otbCommandLineArgumentParser.h"

namespace otb
{

typedef otb::Image<short,2> ImageType;
typedef otb::Image<float,2> HeightImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef itk::DiscreteGaussianImageFilter<ImageType,ImageType> GaussianFilterType;
typedef otb::StereoSensorModelToElevationFilter<ImageType,HeightImageType> StereoFilterType;
typedef otb::VectorImage<float,2> VectorImageType;
typedef otb::ImageList<HeightImageType> ImageListType;
typedef otb::ImageListToVectorImageFilter<ImageListType,VectorImageType> IL2VIFilterType;
typedef otb::StreamingImageFileWriter<VectorImageType> WriterType;

int StereoSensorModelToElevationMap::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("StereoSensorModelToElevationMap");
  
  descriptor->SetDescription("Produce an elevation map from a pair of stereo images along-track by implicit exploration of their epipolar lines.");
  
  descriptor->AddOption("Reference", "The reference image",
                        "ref", 1, true, ApplicationDescriptor::InputImage);
  
  descriptor->AddOption("Secondary", "The secondary image",
                        "sec", 1, true, ApplicationDescriptor::InputImage);
  
  descriptor->AddOption("OutputImage", "The output image",
                        "out", 1, true, ApplicationDescriptor::OutputImage);

  descriptor->AddOption("Radius","Radius (in pixels) of the metric computation window (default is 3).",
                        "r", 1, false, ApplicationDescriptor::Integer);

  descriptor->AddOption("CorrelationThreshold","Threshold bellow which correlation is considered invalid (default is 0.7).",
                        "ct", 1, false, ApplicationDescriptor::Real);

  descriptor->AddOption("VarianceThreshold","Threshold on the master patch variance bellow which no height exploration is performed (default is 4).",
                        "vt", 1, false, ApplicationDescriptor::Real);
  
  descriptor->AddOption("MinHeightOffset","Minimum height offset with respect to local initial height for height exploration (default is -20 meters).",
                        "minh", 1, false, ApplicationDescriptor::Real);

  descriptor->AddOption("MaxHeightOffset","Maximum height offset with respect to local initial height for height exploration (default is 20 meters).",
                        "maxh", 1, false, ApplicationDescriptor::Real);

  descriptor->AddOption("HeightStep","Step of height exploration (default is 1 meter).",
                        "step", 1, false, ApplicationDescriptor::Real);

  descriptor->AddOption("AverageElevation","If no DEM is used, provide the initial height value (default is 0 meters)",
                        "ae", 1, false, ApplicationDescriptor::Real);

  descriptor->AddOption("DEMDirectory","Use DEM tiles to derive initial height values (AverageElevation option is ignored in this case)","dem", 1, false, ApplicationDescriptor::String);

  descriptor->AddOption("GeoidFile","Use a geoid file along with the DEM tiles","geoid",1,false, ApplicationDescriptor::String);

  descriptor->AddOption("ReferenceGaussianSmoothing","(optional) Perform a gaussian smoothing of the reference image. Parameter is gaussian sigma (in pixels). Default is no smoothing.",
                        "rgs", 1, false, ApplicationDescriptor::Real);

  descriptor->AddOption("SecondaryGaussianSmoothing","(optional) Perform a gaussian smoothing of the secondary image. Parameter is gaussian sigma (in pixels). Default is no smoothing.",
                        "sgs", 1, false, ApplicationDescriptor::Real);

  descriptor->AddOption("SubstractInitialHeight","If activated, the elevation map will contain only the estimated height offset from the initial height (either average elevation or DEM)","s",0,false,ApplicationDescriptor::Boolean);

  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram", 1, false, otb::ApplicationDescriptor::Integer);

  return EXIT_SUCCESS;
}


int StereoSensorModelToElevationMap::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Parse arguments
  unsigned int radius = 3;

  if(parseResult->IsOptionPresent("Radius"))
    {
    radius = parseResult->GetParameterUInt("Radius");
    }

  std::cout<<"Radius:\t"<<radius<<std::endl;

  double correlationThreshold = 0.7;

  if(parseResult->IsOptionPresent("CorrelationThreshold"))
    {
    correlationThreshold = parseResult->GetParameterDouble("CorrelationThreshold");
    }

  std::cout<<"Correlation threshold:\t"<<correlationThreshold<<std::endl;

  double varianceThreshold = 4;

  if(parseResult->IsOptionPresent("VarianceThreshold"))
    {
    varianceThreshold = parseResult->GetParameterDouble("VarianceThreshold");
    }

  std::cout<<"Variance threshold:\t"<<varianceThreshold<<std::endl;

  double minHeightOffset = -20;

  if(parseResult->IsOptionPresent("MinHeightOffset"))
    {
    minHeightOffset = parseResult->GetParameterDouble("MinHeightOffset");
    }

  std::cout<<"Minimum height offset:\t"<<minHeightOffset<<std::endl;

  double maxHeightOffset = 20;

  if(parseResult->IsOptionPresent("MaxHeightOffset"))
    {
    maxHeightOffset = parseResult->GetParameterDouble("MaxHeightOffset");
    }

  std::cout<<"Maximum height offset:\t"<<maxHeightOffset<<std::endl;


  double heightStep = 20;

  if(parseResult->IsOptionPresent("HeightStep"))
    {
    heightStep = parseResult->GetParameterDouble("HeightStep");
    }

  std::cout<<"Height step:\t"<<heightStep<<std::endl;

  bool referenceSmoothing = false;
  double referenceSigma = 1.;

  if(parseResult->IsOptionPresent("ReferenceGaussianSmoothing"))
    {
    referenceSmoothing = true;
    referenceSigma = parseResult->GetParameterDouble("ReferenceGaussianSmoothing");
    }

  if(referenceSmoothing)
    {
    std::cout<<"Reference image gaussian smoothing with sigma = "<<referenceSigma<<" pixels."<<std::endl;
    }
  else
    {
    std::cout<<"Reference image gaussian smoothing disabled."<<std::endl;
    }

  bool secondarySmoothing = false;
  double secondarySigma = 1.;

  if(parseResult->IsOptionPresent("SecondaryGaussianSmoothing"))
    {
    secondarySmoothing = true;
    secondarySigma = parseResult->GetParameterDouble("SecondaryGaussianSmoothing");
    }

  if(secondarySmoothing)
    {
    std::cout<<"Secondary image gaussian smoothing with sigma = "<<secondarySigma<<" pixels."<<std::endl;
    }
  else
    {
    std::cout<<"Secondary image gaussian smoothing disabled."<<std::endl;
    }
  
  double averageElevation = 0.;

  if(parseResult->IsOptionPresent("AverageElevation"))
    {
    averageElevation = parseResult->GetParameterDouble("AverageElevation");
    }

  std::string demDirectory = "";
  std::string geoidFile = "";
  bool useDEM = false;
  bool useGeoid = false;

  if(parseResult->IsOptionPresent("DEMDirectory"))
    {
    demDirectory = parseResult->GetParameterString("DEMDirectory");
    useDEM = true;
    }

  if(parseResult->IsOptionPresent("GeoidFile"))
    {
    geoidFile = parseResult->GetParameterString("GeoidFile");
    useGeoid = true;
    }

  if(useDEM)
    {
    std::cout<<"Initial elevation extracted from DEM ("<<demDirectory<<")"<<std::endl;
    
    if(useGeoid)
      {
      std::cout<<"Using geoid ("<<geoidFile<<")"<<std::endl;
      }
    }
  else
    {
    std::cout<<"Initial elevation is constant and equal to "<<averageElevation<<std::endl;
    }

  bool subtractInitialHeight = parseResult->IsOptionPresent("SubtractInitialHeight");

  if(subtractInitialHeight)
    {
    std::cout<<"Output elevation map will contain offset from the initial height."<<std::endl;
    }

  std::string refFileName = parseResult->GetParameterString("Reference");
  std::string secFileName = parseResult->GetParameterString("Secondary");
  std::string outFileName = parseResult->GetParameterString("OutputImage");
  
  ReaderType::Pointer masterReader = ReaderType::New();
  masterReader->SetFileName(refFileName);
  
  ReaderType::Pointer slaveReader = ReaderType::New();
  slaveReader->SetFileName(secFileName);

  GaussianFilterType::ArrayType sigma1;
  sigma1.Fill(referenceSigma);
  
  GaussianFilterType::ArrayType sigma2;
  sigma2.Fill(secondarySigma);


  GaussianFilterType::Pointer gaussian1 = GaussianFilterType::New();
  gaussian1->SetInput(masterReader->GetOutput());
  gaussian1->SetVariance(sigma1);          

  GaussianFilterType::Pointer gaussian2 = GaussianFilterType::New();
  gaussian2->SetInput(slaveReader->GetOutput());
  gaussian2->SetVariance(sigma2);

  StereoFilterType::Pointer stereoFilter = StereoFilterType::New();

  if(referenceSmoothing)
    {
    stereoFilter->SetMasterInput(gaussian1->GetOutput());
    }
  else
    {
    stereoFilter->SetMasterInput(masterReader->GetOutput());
    }
  if(secondarySmoothing)
    {
    stereoFilter->SetSlaveInput(gaussian2->GetOutput());
    }
  else
    {
    stereoFilter->SetSlaveInput(slaveReader->GetOutput());
    }

  stereoFilter->SetUseDEM(useDEM);
  if(useDEM)
    {
    stereoFilter->SetDEMDirectory(demDirectory);
  
    if(useGeoid)
      {
      stereoFilter->SetGeoidFile(geoidFile);
      }
    }
  else
    {
    stereoFilter->SetAverageElevation(averageElevation);
    }
  
  stereoFilter->SetLowerElevation(minHeightOffset);
  stereoFilter->SetHigherElevation(maxHeightOffset);
  stereoFilter->SetElevationStep(heightStep);
  stereoFilter->SetRadius(radius);

  ImageListType::Pointer il = ImageListType::New();

  il->PushBack(stereoFilter->GetOutput());
  il->PushBack(stereoFilter->GetCorrelationOutput());

  IL2VIFilterType::Pointer il2vi = IL2VIFilterType::New();
  il2vi->SetInput(il);

  WriterType::Pointer writer = WriterType::New();
  
  otb::StandardWriterWatcher watcher(writer,stereoFilter,"Performing stereo-matching");

  writer->SetInput(il2vi->GetOutput());
  writer->SetFileName(outFileName);

  unsigned int ram = 256;
  if (parseResult->IsOptionPresent("AvailableMemory"))
    {
    ram = parseResult->GetParameterUInt("AvailableMemory");
    }
  writer->SetAutomaticTiledStreaming(ram);

  writer->Update();

  return EXIT_SUCCESS;
}
}
