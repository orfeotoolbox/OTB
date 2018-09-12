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


#include "otbMultiChannelExtractROI.h"
#include "otbExtractROI.h"

//#include "otbStreamingStatisticsImageFilter.h"
#include "otbSystem.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkChangeLabelImageFilter.h"

#include "otbTileImageFilter.h"

#include <time.h>
#include <algorithm>
#include <climits>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStandardWriterWatcher.h"

// New includes

#include "otbStreamingStatisticsMapFromLabelImageFilter.h"
#include "otbLabelImageSmallRegionMergingFilter.h"

namespace otb
{
namespace Wrapper
{
class SmallRegionsMerging : public Application
{
public:
  typedef SmallRegionsMerging Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef FloatVectorImageType              ImageType;
  typedef ImageType::InternalPixelType      ImagePixelType;
  typedef UInt32ImageType                   LabelImageType;
  typedef LabelImageType::InternalPixelType LabelImagePixelType;

  typedef otb::MultiChannelExtractROI <ImagePixelType,ImagePixelType > MultiChannelExtractROIFilterType;
  typedef otb::ExtractROI<LabelImagePixelType,LabelImagePixelType> ExtractROIFilterType;

  //typedef otb::StreamingStatisticsImageFilter<LabelImageType> StatisticsImageFilterType;
  typedef otb::StreamingStatisticsMapFromLabelImageFilter<ImageType, LabelImageType> StatisticsMapFromLabelImageFilterType;

  typedef otb::LabelImageSmallRegionMergingFilter<LabelImageType> LabelImageSmallRegionMergingFilterType;

  typedef itk::ChangeLabelImageFilter<LabelImageType,LabelImageType> ChangeLabelImageFilterType;
 
  itkNewMacro(Self);
  itkTypeMacro(Merging, otb::Application);

private:
  ChangeLabelImageFilterType::Pointer m_ChangeLabelFilter;

  void DoInit() override
  {
    SetName("SmallRegionsMerging");
    SetDescription("This application performs the third (optional) step of the exact Large-Scale Mean-Shift segmentation workflow [1].");

    SetDocName("Small Region Merging");
    SetDocLongDescription("Given a segmentation result and the original image, it will"
                          " merge segments whose size in pixels is lower than minsize parameter"
                          " with the adjacent segments with the adjacent segment with closest"
                          " radiometry and acceptable size.\n\n"
                          "Small segments will be processed by increasing size: first all segments"
                          " for which area is equal to 1 pixel will be merged with adjacent"
                          " segments, then all segments of area equal to 2 pixels will be processed,"
                          " until segments of area minsize. For large images one can use the"
                          " tilesizex and tilesizey parameters for tile-wise processing, with the"
                          " guarantees of identical results.\n\n");
    SetDocLimitations("This application is more efficient if the labels are contiguous, starting from 0.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso( "Segmentation");
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage,  "in",    "Input image");
    SetParameterDescription( "in", "The input image, containing initial spectral signatures corresponding to the segmented image (inseg)." );
    AddParameter(ParameterType_InputImage,  "inseg",    "Segmented image");
    SetParameterDescription( "inseg", "Segmented image where each pixel value is the unique integer label of the segment it belongs to." );

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription( "out", "The output image. The output image is the segmented image where the minimal segments have been merged. An ecoding of uint32 is advised." );
    SetDefaultOutputPixelType("out",ImagePixelType_uint32);

    AddParameter(ParameterType_Int, "minsize", "Minimum Segment Size");
    SetParameterDescription("minsize", "Minimum Segment Size. If, after the segmentation, a segment is of size strictly lower than this criterion, the segment is merged with the segment that has the closest sepctral signature.");
    SetDefaultParameterInt("minsize", 50);
    SetMinimumParameterIntValue("minsize", 1);
    MandatoryOff("minsize");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in","smooth.tif");
    SetDocExampleParameterValue("inseg","segmentation.tif");
    SetDocExampleParameterValue("out","merged.tif");
    SetDocExampleParameterValue("minsize","20");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    clock_t tic = clock();
  
    unsigned int minSize     = GetParameterInt("minsize");

    //Acquisition of the input image dimensions
    ImageType::Pointer imageIn = GetParameterImage("in");

    LabelImageType::Pointer labelIn = GetParameterUInt32Image("inseg");

    // Compute statistics for each segment
    auto labelStatsFilter = StatisticsMapFromLabelImageFilterType::New();
    labelStatsFilter->SetInput(imageIn);
    labelStatsFilter->SetInputLabelImage(labelIn);
    AddProcess(labelStatsFilter->GetStreamer() , "Computing stats on input image ...");
    labelStatsFilter->Update();
    
    // Convert Map to Vector
    auto labelPopulationMap = labelStatsFilter->GetLabelPopulationMap();
    std::vector<double> labelPopulation;
    for (unsigned int i =0; i <= labelPopulationMap.rbegin()->first; i++)
      {
      labelPopulation.push_back(labelPopulationMap[i]);
      }
    auto meanValueMap = labelStatsFilter->GetMeanValueMap();
    std::vector<itk::VariableLengthVector<double> > meanValues;
    for (unsigned int i =0; i <= meanValueMap.rbegin()->first; i++)
      {
      meanValues.push_back(meanValueMap[i]);
      }
    
    auto regionMergingFilter = LabelImageSmallRegionMergingFilterType::New();
    regionMergingFilter->SetInput( labelIn );
    regionMergingFilter->SetLabelPopulation( labelPopulation );
    regionMergingFilter->SetLabelStatistic( meanValues );
    
    regionMergingFilter->SetMinSize( minSize);
    regionMergingFilter->Update();
    
    //Relabelling
    auto changeLabelFilter = ChangeLabelImageFilterType::New();
    changeLabelFilter->SetInput(labelIn);
    auto LUT = regionMergingFilter->GetLUT();

    for(unsigned int i = 0; i<LUT.size(); ++i)
      { 
      if(i!=LUT[i])
        {
        changeLabelFilter->SetChange(i,LUT[i]);
        }
      }
    SetParameterOutputImage("out", changeLabelFilter->GetOutput());
    RegisterPipeline();
    clock_t toc = clock();

    otbAppLogINFO(<<"Elapsed time: "<<(double)(toc - tic) / CLOCKS_PER_SEC<<" seconds");
  }
  
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SmallRegionsMerging)


