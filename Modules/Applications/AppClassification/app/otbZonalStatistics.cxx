/*
 * Copyright (C) 2017 National Research Institute of Science and
 * Technology for Environment and Agriculture (IRSTEA)
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
  typedef Int32ImageType                LabelImageType;
  typedef LabelImageType::ValueType     LabelValueType;
  typedef otb::VectorData<double, 2>    VectorDataType;
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType,
                                                   FloatVectorImageType>             VectorDataReprojFilterType;
  typedef otb::VectorDataToLabelImageFilter<VectorDataType,
                                            LabelImageType>                   RasterizeFilterType;
  typedef VectorDataType::DataTreeType  DataTreeType;
  typedef itk::PreOrderTreeIterator<DataTreeType>
  TreeIteratorType;
  typedef VectorDataType::DataNodeType  DataNodeType;
  typedef DataNodeType::PolygonListPointerType
  PolygonListPointerType;
  typedef otb::StreamingStatisticsMapFromLabelImageFilter<FloatVectorImageType,
                                                          LabelImageType>                   StatsFilterType;
  typedef otb::StatisticsXMLFileWriter<FloatVectorImageType::PixelType>
  StatsWriterType;
  typedef otb::LabelImageToVectorDataFilter<LabelImageType>
  LabelImageToVectorFilterType;
  typedef itk::BinaryThresholdImageFilter<LabelImageType,
                                          LabelImageType>                   ThresholdFilterType;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(ZonalStatistics, Application);

  void DoInit()
  {

    SetName("ZonalStatistics");
    SetDescription("This application computes zonal statistics");

    // Documentation
    SetDocName("ZonalStatistics");
    SetDocLongDescription("This application computes zonal statistics from label image, or vector data. "
                          "The application inputs one input multiband image, and a label input. "
                          "If the label input is a raster, the output statistics are exported in a XML file. If the label "
                          "input is a vector data, the output statistics are exported in a new vector data with statistics "
                          "in the attribute table. The computed statistics are mean, min, max, and standard deviation.");
    SetDocLimitations("The shapefile must fit in memory");
    SetDocAuthors("Remi Cresson");

    AddDocTag(Tags::Manip);
    AddDocTag(Tags::Analysis);

    // Input image
    AddParameter(ParameterType_InputImage, "in",   "Input Image");
    AddParameter(ParameterType_Float,      "inbv", "Background value to ignore in statistics computation");
    MandatoryOff                          ("inbv");

    // Input zone mode
    AddParameter(ParameterType_Choice, "inzone", "Type of input for the zone definitions");
    AddChoice("inzone.vector",     "Input objects from vector data");
    AddChoice("inzone.labelimage", "Input objects from label image");

    // Input for vector mode
    AddParameter(ParameterType_InputVectorData,  "inzone.vector.in",        "Input vector data");
    AddParameter(ParameterType_Bool,             "inzone.vector.reproject", "Reproject the input vector");


    // Input for label image mode
    AddParameter(ParameterType_InputImage, "inzone.labelimage.in",     "Input label image");
    AddParameter(ParameterType_Int,        "inzone.labelimage.nodata", "No-data value for the input label image");
    MandatoryOff                          ("inzone.labelimage.nodata");


    // Output stats mode
    AddParameter(ParameterType_Choice, "out", "Format of the output stats");
    AddChoice("out.vector", "Output vector data");
    AddParameter(ParameterType_OutputVectorData, "out.vector.filename", "Filename for the output vector data");
    AddChoice("out.xml", "Output XML file");
    AddParameter(ParameterType_String, "out.xml.filename", "");
//    AddChoice("out.raster", "Output raster image");
//    AddParameter(ParameterType_OutputImage, "out.raster.filename", "");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "input.tif");
    SetDocExampleParameterValue("inzone.vector.in", "myvector.shp");
    SetDocExampleParameterValue("out.vector.filename", "myvector_with_stats.shp");


  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  // Returns a string of the kind "prefix_i"
  const std::string CreateFieldName(const std::string & prefix, const unsigned int i)
  {
    std::stringstream ss;
    ss << prefix << "_" << i;
    return ss.str();
  }

  // Returns a null pixel which has the same number of components per pixels as img
  FloatVectorImageType::PixelType NullPixel(FloatVectorImageType::Pointer & img)
  {
    const unsigned int nBands = img->GetNumberOfComponentsPerPixel();
    FloatVectorImageType::PixelType pix(nBands);
    pix.Fill(0);
    return pix;
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
  }

  void RemoveNoDataEntry()
  {
    m_CountMap = m_StatsFilter->GetLabelPopulationMap();
    m_MeanMap = m_StatsFilter->GetMeanValueMap();
    m_StdMap = m_StatsFilter->GetStandardDeviationValueMap();
    m_MinMap = m_StatsFilter->GetMinValueMap();
    m_MaxMap = m_StatsFilter->GetMaxValueMap();
    if (( GetParameterAsString("inzone") == "labelimage" && HasUserValue("inzone.labelimage.nodata"))
        || (GetParameterAsString("inzone") == "vector")      )
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
    m_ThresholdFilter = ThresholdFilterType::New();
    m_ThresholdFilter->SetInput(GetParameterInt32Image("inzone.labelimage.in"));
    m_ThresholdFilter->SetInsideValue(0);
    m_ThresholdFilter->SetOutsideValue(1);
    m_ThresholdFilter->SetLowerThreshold(m_IntNoData);
    m_ThresholdFilter->SetUpperThreshold(m_IntNoData);

    // Vectorize the image
    m_LabelImageToVectorFilter = LabelImageToVectorFilterType::New();
    m_LabelImageToVectorFilter->SetInput(GetParameterInt32Image("inzone.labelimage.in"));
    m_LabelImageToVectorFilter->SetInputMask(m_ThresholdFilter->GetOutput());
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
    LabelValueType internalFID = -1;
    m_NewVectorData = VectorDataType::New();
    DataNodeType::Pointer root = m_NewVectorData->GetDataTree()->GetRoot()->Get();
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
          currentGeometry->SetFieldAsDouble("count", m_CountMap[internalFID] );
          for (unsigned int band = 0 ; band < m_InputImage->GetNumberOfComponentsPerPixel() ; band++)
            {
            currentGeometry->SetFieldAsDouble(CreateFieldName("mean",  band), m_MeanMap[internalFID][band] );
            currentGeometry->SetFieldAsDouble(CreateFieldName("stdev", band), m_StdMap [internalFID][band] );
            currentGeometry->SetFieldAsDouble(CreateFieldName("min",   band), m_MinMap [internalFID][band] );
            currentGeometry->SetFieldAsDouble(CreateFieldName("max",   band), m_MaxMap [internalFID][band] );
            }
          m_NewVectorData->GetDataTree()->Add(currentGeometry, folder);
          }
        }
      ++itVector;
      } // next feature

    SetParameterOutputVectorData("out.vector.filename", m_NewVectorData);
  }

  void WriteXMLStatsFile()
  {
    // Write statistics in XML file
    const std::string outXMLFile = this->GetParameterString("out.xml.filename");
    otbAppLogINFO("Writing " + outXMLFile)
      StatsWriterType::Pointer statWriter = StatsWriterType::New();
    statWriter->SetFileName(outXMLFile);
    statWriter->AddInputMap<StatsFilterType::LabelPopulationMapType>("count",m_CountMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("mean",m_MeanMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("std",m_StdMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("min",m_MinMap);
    statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("max",m_MaxMap);
    statWriter->Update();
  }

  void DoExecute()
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
      {
      PrepareForLabelImageInput();
      }
    else if (GetParameterAsString("inzone") == "vector")
      {
      PrepareForVectorDataInput();
      }
    else
      {
      otbAppLogFATAL("Unknown zone definition mode");
      }
    // Remove the no-data entry
    RemoveNoDataEntry();
    // Generate output
    if (GetParameterAsString("out") == "vector")
      {
      if (m_FromLabelImage)
        {
        GenerateVectorDataFromLabelImage();
        }
      WriteVectorData();
      }
    else if (GetParameterAsString("out") == "xml")
      {
      WriteXMLStatsFile();
      }
    else
      {
      otbAppLogFATAL("Unknown output mode");
      }

  }

  VectorDataType::Pointer m_VectorDataSrc;
  VectorDataType::Pointer m_NewVectorData;
  VectorDataReprojFilterType::Pointer m_VectorDataReprojectionFilter;
  RasterizeFilterType::Pointer m_RasterizeFilter;
  StatsFilterType::Pointer m_StatsFilter;
  LabelImageToVectorFilterType::Pointer m_LabelImageToVectorFilter;
  ThresholdFilterType::Pointer m_ThresholdFilter;
  FloatVectorImageType::Pointer m_InputImage;
  LabelValueType m_IntNoData = itk::NumericTraits<LabelValueType>::max();
  bool m_FromLabelImage = false;
  StatsFilterType::LabelPopulationMapType m_CountMap;
  StatsFilterType::PixelValueMapType m_MeanMap;
  StatsFilterType::PixelValueMapType m_StdMap;
  StatsFilterType::PixelValueMapType m_MinMap;
  StatsFilterType::PixelValueMapType m_MaxMap;

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::ZonalStatistics )
