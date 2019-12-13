/*
 * Copyright (C) 2017 IRSTEA
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

#include "itkFixedArray.h"
#include "itkObjectFactory.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"
#include "otbWrapperApplicationFactory.h"

// Application engine
#include "otbStandardFilterWatcher.h"

// Process objects
#include "otbVectorDataToLabelImageFilter.h"
#include "otbVectorDataIntoImageProjectionFilter.h"
#include "otbStreamingStatisticsMapFromLabelImageFilter.h"
#include "otbStatisticsXMLFileWriter.h"

// Raster --> Vector
#include "otbLabelImageToVectorDataFilter.h"
#include "itkBinaryThresholdImageFilter.h"

#include "otbFunctorImageFilter.h"

namespace otb
{

namespace Wrapper
{

class ZonalStatistics : public Application
{
public:
  /** Standard class typedefs. */
  typedef ZonalStatistics               Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /* Typedefs */
  typedef Int32ImageType            LabelImageType;
  typedef LabelImageType::ValueType LabelValueType;
  typedef otb::VectorData<double, 2>                                                     VectorDataType;
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, FloatVectorImageType> VectorDataReprojFilterType;
  typedef otb::VectorDataToLabelImageFilter<VectorDataType, LabelImageType>              RasterizeFilterType;
  typedef VectorDataType::DataTreeType            DataTreeType;
  typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;
  typedef VectorDataType::DataNodeType            DataNodeType;
  typedef DataNodeType::PolygonListPointerType    PolygonListPointerType;
  typedef otb::StreamingStatisticsMapFromLabelImageFilter<FloatVectorImageType, LabelImageType> StatsFilterType;
  typedef otb::StatisticsXMLFileWriter<FloatVectorImageType::PixelType> StatsWriterType;
  typedef otb::LabelImageToVectorDataFilter<LabelImageType>             LabelImageToVectorFilterType;
  typedef itk::BinaryThresholdImageFilter<LabelImageType, LabelImageType> ThresholdFilterType;

  template <class TInput, class TOutput>
  struct EncoderFunctorType
  {
    StatsFilterType::LabelPopulationMapType* m_CountMap;
    StatsFilterType::PixelValueMapType*      m_MeanMap;
    StatsFilterType::PixelValueMapType*      m_StdMap;
    StatsFilterType::PixelValueMapType*      m_MinMap;
    StatsFilterType::PixelValueMapType*      m_MaxMap;
    size_t                                   m_NbInputComponents;
    LabelValueType                           m_InNoData;
    LabelValueType                           m_OutBvValue;
    static constexpr size_t                  m_NbStatsPerBand{4};
    static constexpr size_t                  m_NbGlobalStats{1};

    size_t OutputSize(const std::array<size_t, 1>&) const
    {
      return m_NbInputComponents * m_NbStatsPerBand + m_NbGlobalStats;
    }

    size_t OutputSize() const
    {
      return m_NbInputComponents * m_NbStatsPerBand + m_NbGlobalStats;
    }

    TOutput operator()(TInput const& pix)
    {
      TOutput outPix(OutputSize());
      outPix.Fill(m_OutBvValue);
      if (pix != m_InNoData)
      {
        outPix[0] = (*m_CountMap)[pix];
        for (size_t i = 0; i < m_NbInputComponents; ++i)
        {
          outPix[i * m_NbStatsPerBand + 1] = (*m_MeanMap)[pix][i];
          outPix[i * m_NbStatsPerBand + 2] = (*m_StdMap)[pix][i];
          outPix[i * m_NbStatsPerBand + 3] = (*m_MinMap)[pix][i];
          outPix[i * m_NbStatsPerBand + 4] = (*m_MaxMap)[pix][i];
        }
      }
      return outPix;
    }

    bool operator!=(const EncoderFunctorType& other)
    {
      if (m_CountMap != other.m_CountMap || m_MeanMap != other.m_MeanMap || m_StdMap != other.m_StdMap || m_MinMap != other.m_MinMap ||
          m_MaxMap != other.m_MaxMap || m_NbInputComponents != other.m_NbInputComponents || m_InNoData != other.m_InNoData ||
          m_OutBvValue != other.m_OutBvValue)
        return true;
      else
        return false;
    }
  };
  typedef otb::FunctorImageFilter<EncoderFunctorType<LabelValueType, FloatVectorImageType::PixelType>> EncoderFilterType;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(ZonalStatistics, Application);

  void DoInit() override
  {

    SetName("ZonalStatistics");
    SetDescription("This application computes zonal statistics");

    // Documentation
    SetDocLongDescription(
        "This application computes zonal statistics from label image, or vector data. "
        "The application inputs one input multiband image, and another input for zones definition. "
        "Zones can be defined with a label image (inzone.labelimage.in) or a vector data layer "
        "(inzone.vector.in). The following statistics are computed over each zones: mean, min, max, "
        "and standard deviation. Statistics can be exported in a vector layer (if the input zone "
        "definition is a label image, it will be vectorized) or in a XML file");
    SetDocLimitations(
        "1) The inzone.vector.in must fit in memory (if \"inzone\" is \"vector\"). 2) The vectorized label "
        "image must also fit in memory (if \"out\" is \"vector\"): if not, consider using \"out\" to "
        "\"xml\"");
    SetDocAuthors("Remi Cresson, Jordi Inglada");
    SetDocSeeAlso("ComputeImagesStatistics");

    AddDocTag(Tags::Manip);
    AddDocTag(Tags::Analysis);

    // Input image
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    AddParameter(ParameterType_Float, "inbv", "Background value to ignore in statistics computation");
    MandatoryOff("inbv");

    // Input zone mode
    AddParameter(ParameterType_Choice, "inzone", "Type of input for the zone definitions");
    AddChoice("inzone.vector", "Input objects from vector data");
    AddChoice("inzone.labelimage", "Input objects from label image");

    // Input for vector mode
    AddParameter(ParameterType_InputVectorData, "inzone.vector.in", "Input vector data");
    AddParameter(ParameterType_Bool, "inzone.vector.reproject", "Reproject the input vector");


    // Input for label image mode
    AddParameter(ParameterType_InputImage, "inzone.labelimage.in", "Input label image");
    AddParameter(ParameterType_Int, "inzone.labelimage.nodata", "No-data value for the input label image");
    MandatoryOff("inzone.labelimage.nodata");


    // Output stats mode
    AddParameter(ParameterType_Choice, "out", "Format of the output stats");
    AddChoice("out.vector", "Output vector data");
    AddParameter(ParameterType_OutputVectorData, "out.vector.filename", "Filename for the output vector data");
    AddChoice("out.xml", "Output XML file");
    AddParameter(ParameterType_String, "out.xml.filename", "Filename for the output xml file");
    AddChoice("out.raster", "Output raster image");
    AddParameter(ParameterType_OutputImage, "out.raster.filename", "File name for the raster image");
    AddParameter(ParameterType_Float, "out.raster.bv", "Background value for the output raster");
    MandatoryOff("out.raster.bv");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "input.tif");
    SetDocExampleParameterValue("inzone.vector.in", "myvector.shp");
    SetDocExampleParameterValue("out.vector.filename", "myvector_with_stats.shp");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  // Returns a string of the kind "prefix_i"
  const std::string CreateFieldName(const std::string& prefix, const unsigned int i)
  {
    std::stringstream ss;
    ss << prefix << "_" << i;
    return ss.str();
  }

  // Returns a null pixel which has the same number of components per pixels as img
  FloatVectorImageType::PixelType NullPixel(FloatVectorImageType::Pointer& img)
  {
    const unsigned int              nBands = img->GetNumberOfComponentsPerPixel();
    FloatVectorImageType::PixelType pix(nBands);
    pix.Fill(0);
    return pix;
  }

  void GetStats()
  {
    m_CountMap = m_StatsFilter->GetLabelPopulationMap();
    m_MeanMap  = m_StatsFilter->GetMeanValueMap();
    m_StdMap   = m_StatsFilter->GetStandardDeviationValueMap();
    m_MinMap   = m_StatsFilter->GetMinValueMap();
    m_MaxMap   = m_StatsFilter->GetMaxValueMap();
  }

  void PrepareForLabelImageInput()
  {
    otbAppLogINFO("Zone definition: label image");
    // Computing stats
    m_StatsFilter->SetInputLabelImage(GetParameterInt32Image("inzone.labelimage.in"));
    m_StatsFilter->Update();
    // In this zone definition mode, the user can provide a no-data value for the labels
    if (HasUserValue("inzone.labelimage.nodata"))
      m_IntNoData = GetParameterInt("inzone.labelimage.nodata");
    GetStats();
  }

  void PrepareForVectorDataInput()
  {
    otbAppLogINFO("Zone definition: vector");
    otbAppLogINFO("Loading vector data...");
    m_VectorDataSrc = GetParameterVectorData("inzone.vector.in");
    // Reproject vector data
    if (GetParameterInt("inzone.vector.reproject") != 0)
    {
      ReprojectVectorDataIntoInputImage();
    }
    RasterizeInputVectorData();
    // Computing stats
    m_StatsFilter->SetInputLabelImage(m_RasterizeFilter->GetOutput());
    m_StatsFilter->Update();
    GetStats();
  }

  void ReprojectVectorDataIntoInputImage()
  {
    otbAppLogINFO("Vector data reprojection enabled");
    m_VectorDataReprojectionFilter = VectorDataReprojFilterType::New();
    m_VectorDataReprojectionFilter->SetInputVectorData(m_VectorDataSrc.GetPointer());
    m_VectorDataReprojectionFilter->SetInputImage(m_InputImage);
    AddProcess(m_VectorDataReprojectionFilter, "Reproject vector data");
    m_VectorDataReprojectionFilter->Update();
    m_VectorDataSrc = m_VectorDataReprojectionFilter->GetOutput();
  }

  void RasterizeInputVectorData()
  {
    // Rasterize vector data
    m_RasterizeFilter = RasterizeFilterType::New();
    m_RasterizeFilter->AddVectorData(m_VectorDataSrc);
    m_RasterizeFilter->SetOutputOrigin(m_InputImage->GetOrigin());
    m_RasterizeFilter->SetOutputSpacing(m_InputImage->GetSignedSpacing());
    m_RasterizeFilter->SetOutputSize(m_InputImage->GetLargestPossibleRegion().GetSize());
    m_RasterizeFilter->SetOutputProjectionRef(m_InputImage->GetProjectionRef());
    m_RasterizeFilter->SetBurnAttribute("________");
    m_RasterizeFilter->SetDefaultBurnValue(0);
    m_RasterizeFilter->SetGlobalWarningDisplay(false);
    m_RasterizeFilter->SetBackgroundValue(m_IntNoData);
    AddProcess(m_RasterizeFilter, "Rasterize input vector data");
  }

  void RemoveNoDataEntry()
  {
    if ((GetParameterAsString("inzone") == "labelimage" && HasUserValue("inzone.labelimage.nodata")) || (GetParameterAsString("inzone") == "vector"))
    {
      otbAppLogINFO("Removing entries for label value " << m_IntNoData);

      m_CountMap.erase(m_IntNoData);
      m_MeanMap.erase(m_IntNoData);
      m_StdMap.erase(m_IntNoData);
      m_MinMap.erase(m_IntNoData);
      m_MaxMap.erase(m_IntNoData);
    }
  }


  void GenerateVectorDataFromLabelImage()
  {
    // Mask for label image
    m_InputThresholdFilter = ThresholdFilterType::New();
    m_InputThresholdFilter->SetInput(GetParameterInt32Image("inzone.labelimage.in"));
    m_InputThresholdFilter->SetInsideValue(0);
    m_InputThresholdFilter->SetOutsideValue(1);
    m_InputThresholdFilter->SetLowerThreshold(m_IntNoData);
    m_InputThresholdFilter->SetUpperThreshold(m_IntNoData);
    m_InputThresholdFilter->UpdateOutputInformation();
    AddProcess(m_InputThresholdFilter, "Threshold label image");
    // Vectorize the image
    m_LabelImageToVectorFilter = LabelImageToVectorFilterType::New();
    m_LabelImageToVectorFilter->SetInput(GetParameterInt32Image("inzone.labelimage.in"));
    m_LabelImageToVectorFilter->SetInputMask(m_InputThresholdFilter->GetOutput());
    m_LabelImageToVectorFilter->SetFieldName("polygon_id");
    AddProcess(m_LabelImageToVectorFilter, "Vectorize label image");
    m_LabelImageToVectorFilter->Update();
    // The source vector data is the vectorized label image
    m_VectorDataSrc = m_LabelImageToVectorFilter->GetOutput();
  }

  void WriteVectorData()
  {
    // Add statistics fields
    otbAppLogINFO("Writing output vector data");
    LabelValueType internalFID     = -1;
    m_NewVectorData                = VectorDataType::New();
    DataNodeType::Pointer root     = m_NewVectorData->GetDataTree()->GetRoot()->Get();
    DataNodeType::Pointer document = DataNodeType::New();
    document->SetNodeType(otb::DOCUMENT);
    m_NewVectorData->GetDataTree()->Add(document, root);
    DataNodeType::Pointer folder = DataNodeType::New();
    folder->SetNodeType(otb::FOLDER);
    m_NewVectorData->GetDataTree()->Add(folder, document);
    m_NewVectorData->SetProjectionRef(m_VectorDataSrc->GetProjectionRef());
    TreeIteratorType itVector(m_VectorDataSrc->GetDataTree());
    itVector.GoToBegin();

    while (!itVector.IsAtEnd())
    {
      if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
      {

        DataNodeType::Pointer currentGeometry = itVector.Get();
        if (m_FromLabelImage)
          internalFID = currentGeometry->GetFieldAsInt("polygon_id");
        else
          internalFID++;

        // Add the geometry with the new fields
        if (m_CountMap.count(internalFID) > 0)
        {
          currentGeometry->SetFieldAsDouble("count", m_CountMap[internalFID]);
          for (unsigned int band = 0; band < m_InputImage->GetNumberOfComponentsPerPixel(); band++)
          {
            currentGeometry->SetFieldAsDouble(CreateFieldName("mean", band), m_MeanMap[internalFID][band]);
            currentGeometry->SetFieldAsDouble(CreateFieldName("stdev", band), m_StdMap[internalFID][band]);
            currentGeometry->SetFieldAsDouble(CreateFieldName("min", band), m_MinMap[internalFID][band]);
            currentGeometry->SetFieldAsDouble(CreateFieldName("max", band), m_MaxMap[internalFID][band]);
          }
          m_NewVectorData->GetDataTree()->Add(currentGeometry, folder);
        }
      }
      ++itVector;
    } // next feature

    SetParameterOutputVectorData("out.vector.filename", m_NewVectorData);
  }

  void SetOutBvValue()
  {
    if (HasUserValue("out.raster.bv"))
    {
      m_OutBvValue = GetParameterFloat("out.raster.bv");
    }
    else if (HasUserValue("inbv"))
    {
      m_OutBvValue = GetParameterFloat("inbv");
    }
    else
    {
      m_OutBvValue = m_IntNoData;
    }
  }

  void WriteRasterData()
  {

    otbAppLogINFO("Writing output raster data");
    SetOutBvValue();
    m_OutputThresholdFilter = ThresholdFilterType::New();
    m_EncoderFilter         = EncoderFilterType::New();
    if (m_FromLabelImage)
    {
      m_EncoderFilter->SetInput(GetParameterInt32Image("inzone.labelimage.in"));
    }
    else
    {
      m_EncoderFilter->SetInput(m_RasterizeFilter->GetOutput());
    }

    m_EncoderFilter->GetModifiableFunctor().m_CountMap          = &m_CountMap;
    m_EncoderFilter->GetModifiableFunctor().m_MeanMap           = &m_MeanMap;
    m_EncoderFilter->GetModifiableFunctor().m_StdMap            = &m_StdMap;
    m_EncoderFilter->GetModifiableFunctor().m_MinMap            = &m_MinMap;
    m_EncoderFilter->GetModifiableFunctor().m_MaxMap            = &m_MaxMap;
    m_EncoderFilter->GetModifiableFunctor().m_NbInputComponents = m_InputImage->GetNumberOfComponentsPerPixel();
    m_EncoderFilter->GetModifiableFunctor().m_InNoData          = m_IntNoData;
    m_EncoderFilter->GetModifiableFunctor().m_OutBvValue        = m_OutBvValue;

    otbAppLogINFO("Output raster image will have " << (m_EncoderFilter->GetFunctor()).OutputSize() << " bands\n");
    AddProcess(m_EncoderFilter, "Encode output raster image");
    SetParameterOutputImage("out.raster.filename", m_EncoderFilter->GetOutput());
  }

  void WriteXMLStatsFile()
  {
    // Write statistics in XML file
    const std::string                                               outXMLFile = this->GetParameterString("out.xml.filename");
    otbAppLogINFO("Writing " + outXMLFile) StatsWriterType::Pointer statWriter = StatsWriterType::New();
    statWriter->SetFileName(outXMLFile);
    statWriter->AddInputMap<StatsFilterType::LabelPopulationMapType>("count", m_CountMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("mean", m_MeanMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("std", m_StdMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("min", m_MinMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("max", m_MaxMap);
    statWriter->Update();
  }

  void DoExecute() override
  {
    // Get input image
    m_InputImage = GetParameterImage("in");
    // Statistics filter
    m_StatsFilter = StatsFilterType::New();
    m_StatsFilter->SetInput(m_InputImage);
    if (HasUserValue("inbv"))
    {
      m_StatsFilter->SetUseNoDataValue(true);
      m_StatsFilter->SetNoDataValue(GetParameterFloat("inbv"));
    }
    m_StatsFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
    AddProcess(m_StatsFilter->GetStreamer(), "Computing statistics");
    // Select zone definition mode
    m_FromLabelImage = (GetParameterAsString("inzone") == "labelimage");
    if (m_FromLabelImage)
      PrepareForLabelImageInput();
    else if (GetParameterAsString("inzone") == "vector")
      PrepareForVectorDataInput();
    else
      otbAppLogFATAL("Unknown zone definition mode");
    // Remove the no-data entry
    RemoveNoDataEntry();
    // Generate output
    if (GetParameterAsString("out") == "xml")
    {
      DisableParameter("out.vector.filename");
      DisableParameter("out.raster.filename");
      EnableParameter("out.xml.filename");
      WriteXMLStatsFile();
    }
    else // vector or raster
    {
      if (m_FromLabelImage)
        GenerateVectorDataFromLabelImage();
      if (GetParameterAsString("out") == "vector")
      {
        EnableParameter("out.vector.filename");
        DisableParameter("out.raster.filename");
        DisableParameter("out.xml.filename");
        WriteVectorData();
      }
      else if (GetParameterAsString("out") == "raster")
      {
        DisableParameter("out.vector.filename");
        EnableParameter("out.raster.filename");
        DisableParameter("out.xml.filename");
        WriteRasterData();
      }
      else
        otbAppLogFATAL("Unknown output mode");
    }
  }

  VectorDataType::Pointer                 m_VectorDataSrc;
  VectorDataType::Pointer                 m_NewVectorData;
  VectorDataReprojFilterType::Pointer     m_VectorDataReprojectionFilter;
  RasterizeFilterType::Pointer            m_RasterizeFilter;
  StatsFilterType::Pointer                m_StatsFilter;
  LabelImageToVectorFilterType::Pointer   m_LabelImageToVectorFilter;
  ThresholdFilterType::Pointer            m_InputThresholdFilter;
  ThresholdFilterType::Pointer            m_OutputThresholdFilter;
  FloatVectorImageType::Pointer           m_InputImage;
  LabelValueType                          m_IntNoData      = itk::NumericTraits<LabelValueType>::max();
  LabelValueType                          m_OutBvValue     = itk::NumericTraits<LabelValueType>::max();
  bool                                    m_FromLabelImage = false;
  StatsFilterType::LabelPopulationMapType m_CountMap;
  StatsFilterType::PixelValueMapType      m_MeanMap;
  StatsFilterType::PixelValueMapType      m_StdMap;
  StatsFilterType::PixelValueMapType      m_MinMap;
  StatsFilterType::PixelValueMapType      m_MaxMap;
  EncoderFilterType::Pointer              m_EncoderFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ZonalStatistics)
