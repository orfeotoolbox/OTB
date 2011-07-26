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
#include "otbMultiResolutionPyramid.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbPerBandVectorImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkShrinkImageFilter.h"
#include "otbImageFileReader.h"
#include "otbObjectList.h"
#include "otbStreamingImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"
#include "otbStandardWriterWatcher.h"

namespace otb
{

int MultiResolutionPyramid::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Multi-resolution pyramid tool");
  descriptor->SetDescription("Build a multi-resolution pyramid of the image.");
  descriptor->AddInputImage();
  descriptor->AddOption("NumberOfLevels","Number of levels in the pyramid (default is 1)","level",1,false, ApplicationDescriptor::Integer);
  descriptor->AddOption("ShrinkFactor","Subsampling factor (default is 2)","sfactor",1,false, ApplicationDescriptor::Integer);
  descriptor->AddOption("VarianceFactor","Before subsampling, image is smoothed with a gaussian kernel of variance VarianceFactor*ShrinkFactor. Higher values will result in more blur, lower in more aliasing (default is 0.6)","vfactor",1,false,ApplicationDescriptor::Real);
  descriptor->AddOption("FastScheme","If used, this option allows to speed-up computation by iteratively subsampling previous level of pyramid instead of processing the full input image each time. Please note that this may result in extra blur or extra aliasing.","fast",0,false,ApplicationDescriptor::Integer);
  descriptor->AddOption("OutputPrefixAndExtextension","prefix for the output files, and extension","out",2,true,ApplicationDescriptor::String);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram", 1, false, otb::ApplicationDescriptor::Integer);
  return EXIT_SUCCESS;
}

int MultiResolutionPyramid::Execute(otb::ApplicationOptionsResult* parseResult)
{
  const unsigned int Dimension = 2;
  typedef otb::Image<unsigned short, Dimension> USImageType;
  typedef otb::VectorImage<unsigned short, Dimension> USVectorImageType;
  typedef otb::Image<short, Dimension> SImageType;
  typedef otb::VectorImage<short, Dimension> SVectorImageType;
  typedef otb::ImageFileReader<USVectorImageType> ReaderType;
  typedef itk::DiscreteGaussianImageFilter<USImageType,SImageType> SmoothingImageFilterType;
  typedef otb::PerBandVectorImageFilter<USVectorImageType,SVectorImageType,
    SmoothingImageFilterType>                                               SmoothingVectorImageFilterType;
  typedef itk::ShrinkImageFilter<SVectorImageType,USVectorImageType> ShrinkFilterType;
  typedef otb::StreamingImageFileWriter<USVectorImageType> WriterType;

  unsigned int nbLevels = 1;
  
  if(parseResult->IsOptionPresent("NumberOfLevels"))
    {
    nbLevels = parseResult->GetParameterUInt("NumberOfLevels");
    }

  unsigned int shrinkFactor = 2;

  if(parseResult->IsOptionPresent("ShrinkFactor"))
    {
    shrinkFactor = parseResult->GetParameterUInt("ShrinkFactor");
    }

  double varianceFactor = 0.6;
  
  if(parseResult->IsOptionPresent("VarianceFactor"))
    {
    varianceFactor = parseResult->GetParameterDouble("VarianceFactor");
    }

  bool fastScheme = parseResult->IsOptionPresent("FastScheme");

  std::string prefix = parseResult->GetParameterString("OutputPrefixAndExtextension",0);
  std::string ext = parseResult->GetParameterString("OutputPrefixAndExtextension",1);

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());

  unsigned int currentLevel = 1;
  unsigned int currentFactor = shrinkFactor;

  while(currentLevel <= nbLevels)
    {
    std::cout<<"Processing level "<<currentLevel<<" with shrink factor "<<currentFactor<<std::endl;

    if(fastScheme && currentLevel > 1)
      {
      itk::OStringStream oss;
      oss<<prefix<<"_"<<currentLevel-1<<"."<<ext;
      reader = ReaderType::New();
      reader->SetFileName(oss.str().c_str());
      }


    SmoothingVectorImageFilterType::Pointer smoothingFilter = SmoothingVectorImageFilterType::New();
    smoothingFilter->SetInput(reader->GetOutput());

    // According to
    // http://www.ipol.im/pub/algo/gjmr_line_segment_detector/
    // This is a good balance between blur and aliasing
    double variance = varianceFactor * static_cast<double>(currentFactor);
    smoothingFilter->GetFilter()->SetVariance(variance);

    ShrinkFilterType::Pointer shrinkFilter = ShrinkFilterType::New();
    shrinkFilter->SetInput(smoothingFilter->GetOutput());
    shrinkFilter->SetShrinkFactors(currentFactor);

    itk::OStringStream oss;
    oss<<prefix<<"_"<<currentLevel<<"."<<ext;

    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(shrinkFilter->GetOutput());
    writer->SetFileName(oss.str().c_str());

    unsigned int ram = 256;
    if (parseResult->IsOptionPresent("AvailableMemory"))
      {
      ram = parseResult->GetParameterUInt("AvailableMemory");
      }
    writer->SetAutomaticTiledStreaming(ram);
    
    oss.str("");
    oss<<"Writing "<<prefix<<"_"<<currentLevel<<"."<<ext;

    otb::StandardWriterWatcher watcher(writer,shrinkFilter,oss.str().c_str());

    writer->Update();

    if(!fastScheme)
      {
      currentFactor*=shrinkFactor;
      }
    
    ++currentLevel;
    }

  return EXIT_SUCCESS;
}

}

