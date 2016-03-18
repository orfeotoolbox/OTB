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

#include "otbDisparityMapToDEMFilter.h"
#include "otbWrapperElevationParametersHandler.h"

#include "otbImageList.h"
#include "otbVectorImageToImageListFilter.h"

namespace otb
{
namespace Wrapper
{

class DisparityMapToElevationMap : public Application
{
public:
  /** Standard class typedefs. */
  typedef DisparityMapToElevationMap    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::ImageList<FloatImageType>                  ImageListType;
  typedef otb::VectorImageToImageListFilter
    <FloatVectorImageType, ImageListType>                 VectorImageToListFilterType;

  typedef otb::DisparityMapToDEMFilter
    <FloatImageType, FloatVectorImageType>                DisparityToElevationFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(DisparityMapToElevationMap, otb::Application);

  /** Filters typedef */

private:
  DisparityMapToElevationMap()
  {
    // Initialize filters
    m_VectorToList = VectorImageToListFilterType::New();
    m_DispToElev = DisparityToElevationFilterType::New();
  }

  void DoInit()
  {
    SetName("DisparityMapToElevationMap");
    SetDescription("Projects a disparity map into a regular elevation map");

    SetDocName("Disparity map to elevation map");
    SetDocLongDescription("This application uses a disparity map computed from a stereo image pair to produce an "
      "elevation map on the ground area covered by the stereo pair. The needed inputs are : the disparity map, "
      "the stereo pair (in original geometry) and the epipolar deformation grids. These grids have to link the "
      "original geometry (stereo pair) and the epipolar geometry (disparity map). ");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbStereoRectificationGridGenerator otbBlockMatching");

    AddDocTag(Tags::Stereo);

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows one to set input images, output images and grids.");

    AddParameter(ParameterType_InputImage,"io.in","Input disparity map");
    SetParameterDescription("io.in","The input disparity map (horizontal disparity in first band, vertical in second)");

    AddParameter(ParameterType_InputImage,"io.left","Left sensor image");
    SetParameterDescription("io.left","Left image in original (sensor) geometry");

    AddParameter(ParameterType_InputImage,"io.right","Right sensor image");
    SetParameterDescription("io.right","Right image in original (sensor) geometry");

    AddParameter(ParameterType_InputImage,"io.lgrid","Left Grid");
    SetParameterDescription("io.lgrid","Left epipolar grid (deformation grid between sensor et disparity spaces)");

    AddParameter(ParameterType_InputImage,"io.rgrid","Right Grid");
    SetParameterDescription("io.rgrid","Right epipolar grid (deformation grid between sensor et disparity spaces)");

    AddParameter(ParameterType_OutputImage,"io.out","Output elevation map");
    SetParameterDescription("io.out", "Output elevation map in ground projection");

    AddParameter(ParameterType_Float,"step","DEM step");
    SetParameterDescription("step","Spacing of the output elevation map (in meters)");
    SetDefaultParameterFloat("step",5.0);

    AddParameter(ParameterType_Float,"hmin","Minimum elevation expected");
    SetParameterDescription("hmin","Minimum elevation expected (in meters)");
    SetDefaultParameterFloat("hmin",0.0);

    AddParameter(ParameterType_Float,"hmax","Maximum elevation expected");
    SetParameterDescription("hmax","Maximum elevation expected (in meters)");
    SetDefaultParameterFloat("hmax",100.0);

    AddParameter(ParameterType_InputImage,"io.mask","Disparity mask");
    SetParameterDescription("io.mask","Masked disparity cells won't be projected");
    MandatoryOff("io.mask");

    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in","disparity.tif");
    SetDocExampleParameterValue("io.left","sensor_left.tif");
    SetDocExampleParameterValue("io.right","sensor_right.tif");
    SetDocExampleParameterValue("io.lgrid","grid_epi_left.tif");
    SetDocExampleParameterValue("io.rgrid","grid_epi_right.tif");
    SetDocExampleParameterValue("io.out","dem.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do
  }

  void DoExecute()
  {
    FloatVectorImageType::Pointer inputDisp     = this->GetParameterImage("io.in");
    FloatVectorImageType::Pointer sensorLeft    = this->GetParameterImage("io.left");
    FloatVectorImageType::Pointer sensorRight   = this->GetParameterImage("io.right");
    FloatVectorImageType::Pointer gridLeft      = this->GetParameterImage("io.lgrid");
    FloatVectorImageType::Pointer gridRight     = this->GetParameterImage("io.rgrid");

    m_VectorToList->SetInput(inputDisp);
    m_VectorToList->UpdateOutputInformation();

    m_DispToElev->SetHorizontalDisparityMapInput(m_VectorToList->GetOutput()->GetNthElement(0));
    m_DispToElev->SetVerticalDisparityMapInput(m_VectorToList->GetOutput()->GetNthElement(1));
    m_DispToElev->SetLeftInput(sensorLeft);
    m_DispToElev->SetRightInput(sensorRight);
    m_DispToElev->SetLeftEpipolarGridInput(gridLeft);
    m_DispToElev->SetRightEpipolarGridInput(gridRight);
    m_DispToElev->SetElevationMin(this->GetParameterFloat("hmin"));
    m_DispToElev->SetElevationMax(this->GetParameterFloat("hmax"));
    m_DispToElev->SetDEMGridStep(this->GetParameterFloat("step"));

    if (IsParameterEnabled("io.mask"))
      {
      m_DispToElev->SetDisparityMaskInput(this->GetParameterUInt8Image("io.mask"));
      }

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    m_DispToElev->UpdateOutputInformation();
    FloatVectorImageType::RegionType outputRegion = m_DispToElev->GetOutput()->GetLargestPossibleRegion();

    otbAppLogINFO(<<"Elevation map origin : [" << m_DispToElev->GetOutput()->GetOrigin()[0] << "," << m_DispToElev->GetOutput()->GetOrigin()[1] <<  "]" << std::endl);
    otbAppLogINFO(<<"Elevation map size : [" << outputRegion.GetSize()[0] << "," << outputRegion.GetSize()[1] << "]" << std::endl);

    this->SetParameterOutputImage("io.out",m_DispToElev->GetOutput());
  }

  VectorImageToListFilterType::Pointer m_VectorToList;

  DisparityToElevationFilterType::Pointer m_DispToElev;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::DisparityMapToElevationMap)
