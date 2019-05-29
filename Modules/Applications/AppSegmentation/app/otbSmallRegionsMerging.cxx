/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbStreamingStatisticsMapFromLabelImageFilter.h"
#include "otbLabelImageSmallRegionMergingFilter.h"
#include "itkChangeLabelImageFilter.h"

#include "otbStopwatch.h"

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

  typedef otb::StreamingStatisticsMapFromLabelImageFilter
      <ImageType, LabelImageType> StatisticsMapFromLabelImageFilterType;

  typedef otb::LabelImageSmallRegionMergingFilter<LabelImageType> 
                                  LabelImageSmallRegionMergingFilterType;

  typedef itk::ChangeLabelImageFilter<LabelImageType,LabelImageType> 
                                  ChangeLabelImageFilterType;
 
  itkNewMacro(Self);
  itkTypeMacro(Merging, otb::Application);

private:
  ChangeLabelImageFilterType::Pointer m_ChangeLabelFilter;

  void DoInit() override
  {
    SetName("SmallRegionsMerging");
    SetDescription("This application merges small regions of a segmentation "
      "result.");

    SetDocLongDescription("Given a segmentation result and the original image,"
                          " it will merge segments whose size in pixels is"
                          " lower than minsize parameter with the adjacent"
                          " segments with the adjacent segment with closest"
                          " radiometry and acceptable size. \n\n"
                          "Small segments will be processed by increasing size:"
                          " first all segments for which area is equal to 1"
                          " pixel will be merged with adjacent segments, then"
                          " all segments of area equal to 2 pixels will be"
                          " processed, until segments of area minsize.");
    
    SetDocLimitations( "None") ;
    
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Segmentation");
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage,  "in", "Input image");
    SetParameterDescription( "in", "The input image, containing initial"
      " spectral signatures corresponding to the segmented image (inseg)." );
    AddParameter(ParameterType_InputImage, "inseg", "Segmented image");
    SetParameterDescription( "inseg", "Segmented image where each pixel value"
    " is the unique integer label of the segment it belongs to." );

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription( "out", "The output image. The output image is the"
    " segmented image where the minimal segments have been merged." );
    SetDefaultOutputPixelType("out",ImagePixelType_uint32);

    AddParameter(ParameterType_Int, "minsize", "Minimum Segment Size");
    SetParameterDescription("minsize", "Minimum Segment Size. If, after the "
    " segmentation, a segment is of size strictly lower than this criterion,"
    " the segment is merged with the segment that has the closest sepctral"
    " signature.");
    
    SetDefaultParameterInt("minsize", 50);
    SetMinimumParameterIntValue("minsize", 1);
    MandatoryOff("minsize");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in","smooth.tif");
    SetDocExampleParameterValue("inseg","segmentation.tif");
    SetDocExampleParameterValue("out","merged.tif");
    SetDocExampleParameterValue("minsize","50");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    // Start Timer for the application
    auto Timer = Stopwatch::StartNew();
    
    unsigned int minSize     = GetParameterInt("minsize");

    //Acquisition of the input image dimensions
    ImageType::Pointer imageIn = GetParameterImage("in");

    LabelImageType::Pointer labelIn = GetParameterUInt32Image("inseg");

    // Compute statistics for each segment
    auto labelStatsFilter = StatisticsMapFromLabelImageFilterType::New();
    labelStatsFilter->SetInput(imageIn);
    labelStatsFilter->SetInputLabelImage(labelIn);
    AddProcess(labelStatsFilter->GetStreamer() , "Computing stats on input"
      " image ...");
    labelStatsFilter->Update();

    // Convert Map to Unordered map

    auto  labelPopulationMap = labelStatsFilter->GetLabelPopulationMap();
    std::unordered_map< unsigned int,double> labelPopulation;
    for (auto population : labelPopulationMap)
      {
      labelPopulation[population.first]=population.second;
      }
    auto  meanValueMap = labelStatsFilter->GetMeanValueMap();
    std::unordered_map< unsigned int, itk::VariableLengthVector<double> > 
      meanValues;
    for (const auto & mean : meanValueMap)
      {
      meanValues[mean.first] = mean.second;
      }  
      
    // Compute the LUT from the original label image to the merged output 
    // label image.
    auto regionMergingFilter = LabelImageSmallRegionMergingFilterType::New();
    regionMergingFilter->SetInputLabelImage( labelIn );
    regionMergingFilter->SetLabelPopulation( labelPopulation );
    regionMergingFilter->SetLabelStatistic( meanValues );
    regionMergingFilter->SetMinSize( minSize);
    
    AddProcess(regionMergingFilter, "Computing LUT ...");
    regionMergingFilter->Update();
    
    // Relabelling using the LUT
    auto changeLabelFilter = ChangeLabelImageFilterType::New();
    changeLabelFilter->SetInput(labelIn);
    
    const auto & LUT = regionMergingFilter->GetLUT();

    for (auto const & label : LUT)
    {
      if (label.first != label.second)
      {
        changeLabelFilter->SetChange(label.first, label.second);
      }
    }
    SetParameterOutputImage("out", changeLabelFilter->GetOutput());
    RegisterPipeline();

    Timer.Stop();
    otbAppLogINFO( "Total elapsed time: "<< 
      float(Timer.GetElapsedMilliseconds())/1000 <<" seconds.");

  }
  
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SmallRegionsMerging)


