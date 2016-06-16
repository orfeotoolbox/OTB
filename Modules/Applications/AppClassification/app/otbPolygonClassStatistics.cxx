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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataToClassStatisticsFilter.h"
#include "otbStatisticsXMLFileWriter.h"
#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"

namespace otb
{
namespace Wrapper
{

class PolygonClassStatistics : public Application
{
public:
  /** Standard class typedefs. */
  typedef PolygonClassStatistics        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(PolygonClassStatistics, otb::Application);

  /** Filters typedef */
  typedef otb::OGRDataToClassStatisticsFilter<FloatVectorImageType,UInt8ImageType> FilterType;
  
  typedef otb::StatisticsXMLFileWriter<FloatVectorImageType::PixelType> StatWriterType;

  typedef otb::GeometriesSet GeometriesType;

  typedef otb::GeometriesProjectionFilter ProjectionFilterType;

private:
  PolygonClassStatistics()
    {
   
    }

  void DoInit()
  {
    SetName("PolygonClassStatistics");
    SetDescription("Computes statistics on a training polygon set.");

    // Documentation
    SetDocName("Polygon Class Statistics");
    SetDocLongDescription("The application processes a set of geometries "
      "intended for training (they should have a field giving the associated "
      "class). The geometries are analysed against a support image to compute "
      "statistics : \n"
      "  - number of samples per class\n"
      "  - number of samples per geometry\n"
      "An optional raster mask can be used to discard samples. Different types"
      " of geometry are supported : polygons, lines, points. The behaviour is "
      "different for each type of geometry :\n"
      "  - polygon: select pixels whose center is inside the polygon\n"
      "  - lines  : select pixels intersecting the line\n"
      "  - points : select closest pixel to the point\n");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage,  "in",   "InputImage");
    SetParameterDescription("in", "Support image that will be classified");

    AddParameter(ParameterType_InputImage,  "mask",   "InputMask");
    SetParameterDescription("mask", "Validity mask (only pixels corresponding to a mask value greater than 0 will be used for statistics)");
    MandatoryOff("mask");
    
    AddParameter(ParameterType_InputFilename, "vec", "Input vectors");
    SetParameterDescription("vec","Input geometries to analyse");
    
    AddParameter(ParameterType_OutputFilename, "out", "Output Statistics");
    SetParameterDescription("out","Output file to store statistics (XML format)");

    AddParameter(ParameterType_String, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class name in the input vectors.");
    MandatoryOff("field");
    SetParameterString("field", "class");
    
    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "support_image.tif");
    SetDocExampleParameterValue("vec", "variousVectors.sqlite");
    SetDocExampleParameterValue("field", "label");
    SetDocExampleParameterValue("out","polygonStat.xml");
  }

  void DoUpdateParameters()
  {
    // Nothing to do
  }

  void DoExecute()
  {
  otb::ogr::DataSource::Pointer vectors = 
    otb::ogr::DataSource::New(this->GetParameterString("vec"));
  std::string fieldName = this->GetParameterString("field");

  // Reproject geometries
  FloatVectorImageType::Pointer inputImg = this->GetParameterImage("in");
  std::string imageProjectionRef = inputImg->GetProjectionRef();
  FloatVectorImageType::ImageKeywordlistType imageKwl =
    inputImg->GetImageKeywordlist();
  std::string vectorProjectionRef =
    vectors->GetLayer(GetParameterInt("layer")).GetProjectionRef();

  otb::ogr::DataSource::Pointer reprojVector = vectors;
  GeometriesType::Pointer inputGeomSet;
  ProjectionFilterType::Pointer geometriesProjFilter;
  GeometriesType::Pointer outputGeomSet;
  bool doReproj = true;
  // don't reproject for these cases
  if (vectorProjectionRef.empty() ||
      (imageProjectionRef == vectorProjectionRef) ||
      (imageProjectionRef.empty() && imageKwl.GetSize() == 0))
    doReproj = false;

  if (doReproj)
    {
    inputGeomSet = GeometriesType::New(vectors);
    reprojVector = otb::ogr::DataSource::New();
    outputGeomSet = GeometriesType::New(reprojVector);
    // Filter instanciation
    geometriesProjFilter = ProjectionFilterType::New();
    geometriesProjFilter->SetInput(inputGeomSet);
    if (imageProjectionRef.empty())
      {
      geometriesProjFilter->SetOutputKeywordList(inputImg->GetImageKeywordlist()); // nec qd capteur
      }
    geometriesProjFilter->SetOutputProjectionRef(imageProjectionRef);
    geometriesProjFilter->SetOutput(outputGeomSet);
    otbAppLogINFO("Reprojecting input vectors...");
    geometriesProjFilter->Update();
    }

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(this->GetParameterImage("in"));
  if (IsParameterEnabled("mask") && HasValue("mask"))
    {
    filter->SetMask(this->GetParameterImage<UInt8ImageType>("mask"));
    }
  filter->SetOGRData(reprojVector);
  filter->SetFieldName(fieldName);
  filter->SetLayerIndex(this->GetParameterInt("layer"));

  AddProcess(filter->GetStreamer(),"Analyse polygons...");
  filter->Update();
  
  FilterType::ClassCountMapType &classCount = filter->GetClassCountOutput()->Get();
  FilterType::PolygonSizeMapType &polySize = filter->GetPolygonSizeOutput()->Get();
  
  StatWriterType::Pointer statWriter = StatWriterType::New();
  statWriter->SetFileName(this->GetParameterString("out"));
  statWriter->AddInputMap<FilterType::ClassCountMapType>("samplesPerClass",classCount);
  statWriter->AddInputMap<FilterType::PolygonSizeMapType>("samplesPerVector",polySize);
  statWriter->Update();
  }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::PolygonClassStatistics)
