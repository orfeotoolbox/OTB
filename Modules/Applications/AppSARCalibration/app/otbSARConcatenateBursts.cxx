/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbMultiChannelExtractROI.h"
#include "otbImageList.h"

#include "otbSarConcatenateBurstsImageFilter.h"

namespace otb
{
namespace Wrapper
{
class SARConcatenateBursts : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARConcatenateBursts     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARConcatenateBursts, otb::Application);

  typedef otb::ImageList<FloatVectorImageType>  ImageListType;
  typedef MultiChannelExtractROI<FloatVectorImageType::InternalPixelType,
				 FloatVectorImageType::InternalPixelType>       ExtractROIFilterType;
  typedef ObjectList<ExtractROIFilterType>                                      ExtractROIFilterListType;
 
  typedef otb::SarConcatenateBurstsImageFilter<FloatVectorImageType> BurstFusionFilterType;

private:
  void DoInit() override
  {
    SetName("SARConcatenateBursts");
    SetDescription("Concatenate a list of Bursts to provide a whole SAR Deburst Image.");

    // Documentation
    SetDocName("SAR Concatenate Bursts");
    SetDocLongDescription("This application performs a burst concatenation and provides a SAR Deburst Image. "
			  "It reads the input image list (single bursts) "
			  "and generates a whole SAR image with deburst operations.");
    SetDocLimitations("Only Sentinel1 IW SLC products are supported for now. In order to concatenate several" 
		      " bursts, all valid lines of each burst are required as inputs." 
		      "ie : Careful with ROI extraction inside a Burst.");
    SetDocAuthors("OTB-Team");

    AddDocTag(Tags::SAR);
    AddDocTag(Tags::Calibration);
    
    AddParameter(ParameterType_InputImageList,  "il",   "Input bursts list");
    SetParameterDescription("il", "The list of bursts to concatenate.");

    AddParameter(ParameterType_InputImage,  "insar",   "Input Sentinel1 IW SLC Image (only metadata used)");
    SetParameterDescription("insar", "Raw Sentinel1 IW SLC image, or any extract of such made by OTB (geom file needed).");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "The concatenated and debursted output image.");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "Burst0.png Burst1.png");
    SetDocExampleParameterValue("out", "otbConcatenateBursts.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {}

  void DoExecute() override
  {
    // Instanciate filters
    ExtractROIFilterListType::Pointer m_ExtractorList = ExtractROIFilterListType::New();
    ImageListType::Pointer m_ImageList = ImageListType::New();
    BurstFusionFilterType::Pointer fusionFilter = BurstFusionFilterType::New();
    
    // Get the input complex image list
    FloatVectorImageListType*  inList = GetParameterImageList("il");
    // Get the input complex image
    FloatVectorImageType*  in = GetParameterImage("insar");
    in->UpdateOutputInformation();

    std::vector<std::pair<unsigned long,unsigned long> > lines;
    std::vector<std::pair<unsigned long,unsigned long> > samples;

    // Check number of inputs with number of bursts into SLC image (must be the same)
    unsigned int nbBursts = 1;
    try
      {
	nbBursts = std::stoi(in->GetImageKeywordlist().GetMetadataByKey("support_data.geom.bursts.number"));
      }
    catch( ... )
      {
	// Throw an execption
	throw std::runtime_error("Failed to retrieve bursts.number value from .geom file.");
      }

    nbBursts = inList->Size();
    
    /*if (inList->Size() != nbBursts)
      {
	throw std::runtime_error("Failed to concatenate bursts. Some bursts are missing.");
	}*/ 

    // Coniguration for fusion filter
    fusionFilter->SetSLCImageKeyWorList(in->GetImageKeywordlist());
    fusionFilter->getDeburstLinesAndSamples(lines, samples);
       
    // Split each input burst to keep only interested region
    for( unsigned int i=0; i<inList->Size(); i++ )
      {
	FloatVectorImageType::Pointer vectIm = inList->GetNthElement(i);
	vectIm->UpdateOutputInformation();

	unsigned long originOffset_samples = static_cast<long>(vectIm->GetOrigin()[0]-0.5);
	unsigned long originOffset_lines = static_cast<long>(vectIm->GetOrigin()[1]-0.5);

	// Retrieve start and size for each burst
	std::pair<unsigned long,unsigned long> line = lines[i];
	std::pair<unsigned long,unsigned long> sample = samples[i];

	unsigned long minSamples = std::min(sample.second, static_cast<unsigned long>(vectIm->GetLargestPossibleRegion().GetSize()[0]-1));
	unsigned long minLines = std::min(line.second, static_cast<unsigned long>(vectIm->GetLargestPossibleRegion().GetSize()[1]-1));

	unsigned long startL = line.first - originOffset_lines;
	unsigned long sizeL = minLines - line.first + 1;
	unsigned long startS = sample.first - originOffset_samples;
	unsigned long sizeS = minSamples - sample.first + 1;
	// Readjust if origin is superior to the first selected line/sample for the current burst
	if (line.first < originOffset_lines) 
	  {
	    startL = 0;
	    sizeL =  minLines - line.first - originOffset_lines + 1;
	  }
	if (sample.first < originOffset_samples) 
	  {
	    startS = 0;
	    sizeS =  minSamples - sample.first - originOffset_samples + 1;
	  }

	ExtractROIFilterType::Pointer extractor = ExtractROIFilterType::New();
	extractor->SetInput(vectIm);
	extractor->SetStartX(startS);
	extractor->SetStartY(startL);
	extractor->SetSizeX(sizeS);
	extractor->SetSizeY(sizeL);
	extractor->UpdateOutputInformation();

	m_ExtractorList->PushBack( extractor );
	m_ImageList->PushBack( extractor->GetOutput() );
	
      }

    BurstFusionFilterType::SizeType layout;
    layout[0] = 1;
    layout[1] = nbBursts;
    fusionFilter->SetLayout(layout);

    for (unsigned int i=0; i<(layout[0]*layout[1]); i++)
      {
	fusionFilter->SetInput(i,m_ImageList->GetNthElement(i));
      }

    SetParameterOutputImage("out", fusionFilter->GetOutput());

    RegisterPipeline();
  }


};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SARConcatenateBursts)
