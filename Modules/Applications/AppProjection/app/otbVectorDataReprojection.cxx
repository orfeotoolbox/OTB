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
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperNumericalParameter.h"
#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"


// MapProjection handler
#include "otbWrapperMapProjectionParametersHandler.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{


namespace Wrapper
{

class VectorDataReprojection : public Application
{
public:
  /** Standard class typedefs. */
  typedef VectorDataReprojection Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** filter typedef  **/
  typedef otb::GeometriesProjectionFilter ProjectionFilterType;

  typedef otb::GeometriesSet InputGeometriesType;
  typedef otb::GeometriesSet OutputGeometriesType;

  /** Standard macro */
  itkNewMacro(Self)
;

  itkTypeMacro(VectorDataReprojection, otb::Application)
;

private:
  void DoInit()
  {
    SetName("VectorDataReprojection");
    std::ostringstream oss;
    oss << "Reproject a vector data using support image projection reference"
        ", or a user specified map projection" << std::endl;
    SetDescription(oss.str());
    // Documentation
    SetDocName("Vector Data reprojection");
    oss.str("");
    oss <<" This application allows reprojecting a vector data using support image projection reference"
        ", or a user given map projection." << std::endl;
    oss <<" If given, image keywordlist can be added to reprojected vectordata.";
    SetDocLongDescription(oss.str());
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
    AddDocTag(Tags::Vector);
    AddDocTag(Tags::Coordinates);

    // Set the parameters

    AddParameter(ParameterType_Group, "in", "Input data");
    AddParameter(ParameterType_InputFilename, "in.vd", "Input vector data");
    SetParameterDescription("in.vd", "The input vector data to reproject");
    AddParameter(ParameterType_InputImage, "in.kwl", "Use image keywords list");
    SetParameterDescription("in.kwl", "Optional input image to fill vector data with image kwl.");
    MandatoryOff("in.kwl");
    DisableParameter("in.kwl");

    AddParameter(ParameterType_Group, "out", "Output data");
    AddParameter(ParameterType_OutputFilename, "out.vd", "Output vector data");
    SetParameterDescription("out.vd", "The reprojected vector data");

    // output projection choice
    AddParameter(ParameterType_Choice, "out.proj", "Output Projection choice");
    AddChoice("out.proj.image", "Use image projection ref");
    SetParameterDescription("out.proj.image", "Vector data will be reprojected in image projection ref.");
    AddParameter(ParameterType_InputImage, "out.proj.image.in", "Image used to get projection map");
    SetParameterDescription("out.proj.image.in", "Projection map will be found using image metadata");
    AddChoice("out.proj.user", "User defined projection");
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "out.proj.user.map");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    // Doc example parameter settings
    SetDocExampleParameterValue("in.vd", "VectorData_QB1.shp");
    SetDocExampleParameterValue("out.proj", "image");
    SetDocExampleParameterValue("out.proj.image.in", "ROI_QB_MUL_1.tif");
    SetDocExampleParameterValue("out.vd","reprojected_vd.shp");
  }

  void DoUpdateParameters()
  {

  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute\n");

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // Get the input image

    otb::ogr::DataSource::Pointer OGRDSin = otb::ogr::DataSource::New(GetParameterString("in.vd"),
                                                                      otb::ogr::DataSource::Modes::Read);

    m_InputGeomSet = InputGeometriesType::New(OGRDSin);

    // Filter instanciation
    m_GeometriesProjFilter = ProjectionFilterType::New();
    m_GeometriesProjFilter->SetInput(m_InputGeomSet);

    if (HasValue("in.kwl"))
      {
      FloatVectorImageType::Pointer inImage = GetParameterFloatVectorImage("in.kwl");
      m_GeometriesProjFilter->SetInputOrigin(inImage->GetOrigin()); // nec qd capteur
      m_GeometriesProjFilter->SetInputSpacing(inImage->GetSpacing()); // nec qd capteur
      m_GeometriesProjFilter->SetInputKeywordList(inImage->GetImageKeywordlist());
      //otbAppLogINFO(<<"kwl."<<std::endl);
      }

    if (GetParameterInt("out.proj") == 0)
      {
      FloatVectorImageType::Pointer outImage = GetParameterFloatVectorImage("out.proj.image.in");

      if (outImage)
        {
        m_GeometriesProjFilter->SetOutputOrigin(outImage->GetOrigin()); // nec qd capteur
        m_GeometriesProjFilter->SetOutputSpacing(outImage->GetSpacing()); // nec qd capteur
        m_OutputProjectionRef = outImage->GetProjectionRef(); // ~ wkt
        if (m_OutputProjectionRef.empty())
          {
            m_GeometriesProjFilter->SetOutputKeywordList(outImage->GetImageKeywordlist()); // nec qd capteur
          }
        }
      else
        {
        // LogFATAl
        }
      }
    else
      {

      // Get the output projection Ref
      m_OutputProjectionRef = MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "out.proj.user.map");

      }
    otbAppLogINFO(<<"Geometries reprojection with following projection reference :"
        <<std::endl<<m_OutputProjectionRef<<std::endl);

    m_GeometriesProjFilter->SetOutputProjectionRef(m_OutputProjectionRef);

    OGRDSout = otb::ogr::DataSource::New(GetParameterString("out.vd"),
                                         otb::ogr::DataSource::Modes::Update_LayerOverwrite);

    m_OutputGeomSet = OutputGeometriesType::New(OGRDSout);

    m_GeometriesProjFilter->SetOutput(m_OutputGeomSet);
    m_GeometriesProjFilter->Update();
    OGRDSout->SyncToDisk();
  }

  otb::ogr::DataSource::Pointer OGRDSout;
  std::string m_OutputProjectionRef;
  ImageKeywordlist m_KeywordList;
  InputGeometriesType::Pointer m_InputGeomSet;
  OutputGeometriesType::Pointer m_OutputGeomSet;
  //FloatVectorImageType::Pointer m_inImage;
 // FloatVectorImageType::Pointer m_outImage;
  ProjectionFilterType::Pointer m_GeometriesProjFilter;
};

  } // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataReprojection)
