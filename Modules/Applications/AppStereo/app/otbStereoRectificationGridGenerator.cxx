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

#include "otbWrapperApplicationFactory.h"

#include "otbStereorectificationDisplacementFieldSource.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"
#include "itkInverseDisplacementFieldImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbDEMToImageGenerator.h"
#include "otbStreamingStatisticsImageFilter.h"

namespace otb
{
namespace Wrapper
{

class StereoRectificationGridGenerator : public Application
{
public:
  /** Standard class typedefs. */
  typedef StereoRectificationGridGenerator    Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef otb::StereorectificationDisplacementFieldSource
  <FloatVectorImageType,FloatVectorImageType> DisplacementFieldSourceType;

  typedef itk::Vector<double,2>               DisplacementType;
  typedef otb::Image<DisplacementType>         DisplacementFieldType;

  typedef itk::VectorCastImageFilter
  <FloatVectorImageType,
   DisplacementFieldType>                      DisplacementFieldCastFilterType;

  typedef itk::InverseDisplacementFieldImageFilter
  <DisplacementFieldType,DisplacementFieldType> InverseDisplacementFieldFilterType;

  typedef itk::VectorIndexSelectionCastImageFilter<DisplacementFieldType,
                                                   FloatImageType> IndexSelectionCastFilterType;

  typedef otb::ImageList<FloatImageType>      ImageListType;
  typedef otb::ImageListToVectorImageFilter<ImageListType,FloatVectorImageType> ImageListFilterType;

  typedef otb::DEMToImageGenerator<FloatImageType >           DEMToImageGeneratorType;
  typedef otb::StreamingStatisticsImageFilter<FloatImageType> StatisticsFilterType;


  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(StereoRectificationGridGenerator, otb::Application);

private:

  StereoRectificationGridGenerator()
  {
    // Instantiate deformation field source
    m_DisplacementFieldSource = DisplacementFieldSourceType::New();
    m_LeftInvertDisplacementFieldFilter = InverseDisplacementFieldFilterType::New();
    m_RightInvertDisplacementFieldFilter = InverseDisplacementFieldFilterType::New();

    m_LeftDisplacementFieldCaster = DisplacementFieldCastFilterType::New();
    m_LeftIndexSelectionFilter1  = IndexSelectionCastFilterType::New();
    m_LeftIndexSelectionFilter2  = IndexSelectionCastFilterType::New();
    m_LeftImageList              = ImageListType::New();
    m_LeftImageListFilter        = ImageListFilterType::New();

    m_RightDisplacementFieldCaster = DisplacementFieldCastFilterType::New();
    m_RightIndexSelectionFilter1  = IndexSelectionCastFilterType::New();
    m_RightIndexSelectionFilter2  = IndexSelectionCastFilterType::New();
    m_RightImageList              = ImageListType::New();
    m_RightImageListFilter        = ImageListFilterType::New();

    m_DEMToImageGenerator         = DEMToImageGeneratorType::New();
    m_StatisticsFilter            = StatisticsFilterType::New();
  }

  void DoInit() override
  {
    SetName("StereoRectificationGridGenerator");
    SetDescription("Generates two deformation fields to resample in epipolar "
      "geometry, a pair of stereo images up to the sensor model precision");

    SetDocLongDescription(
      "This application generates a pair of deformation "
      "grid to stereo-rectify a pair of stereo images according to sensor "
      "modelling and a mean elevation hypothesis.\n\n"

      "This application is the first part of the stereo reconstruction "
      "framework. The output deformation grids can be passed to the "
      "GridBasedImageResampling application for actual resampling into epipolar"
      " geometry.\n\n"

      "There are several ways to set the elevation source:\n\n"
      "* An arbitrary constant elevation\n"
      "* A DEM directory\n"
      "* Compute an average elevation from a DEM\n\n"

      "If needed, the application can compute inverse resampling grids (from "
      "epipolar to original sensor geometry). Don't forget to check the other "
      "outputs from the application. For instance, the application gives the "
      "X and Y size of the rectified images, along with an estimated baseline "
      "ratio.");
    SetDocLimitations("Generation of the deformation grid is not streamable, "
      "pay attention to this fact when setting the grid step.");
    SetDocAuthors("OTB-Team");

    AddDocTag(Tags::Stereo);

    SetDocSeeAlso("otbGridBasedImageResampling");

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows setting the input and output images.");
    AddParameter(ParameterType_InputImage,"io.inleft","Left input image");
    SetParameterDescription("io.inleft","The left image from the stereo pair,"
      " in sensor geometry.");

    AddParameter(ParameterType_InputImage,"io.inright","Right input image");
    SetParameterDescription("io.inright","The right image from the stereo pair,"
      " in sensor geometry.");

    AddParameter(ParameterType_OutputImage, "io.outleft", "Left output deformation grid");
    SetParameterDescription("io.outleft","The deformation grid to resample the"
      " left image from sensor geometry to epipolar geometry.");

    AddParameter(ParameterType_OutputImage, "io.outright", "Right output deformation grid");
    SetParameterDescription("io.outright","The deformation grid to resample the"
      " right image from sensor geometry to epipolar geometry.");

    AddParameter(ParameterType_Group,"epi","Epipolar  geometry and grid parameters");
    SetParameterDescription("epi","Parameters of the epipolar geometry and output grids");

    ElevationParametersHandler::AddElevationParameters(this, "epi.elevation");

    AddParameter(ParameterType_Group,"epi.elevation.avgdem","Average elevation computed from DEM");
    SetParameterDescription("epi.elevation.avgdem","Average elevation computed from the provided DEM");

    AddParameter(ParameterType_Int,"epi.elevation.avgdem.step","Sub-sampling step");
    SetParameterDescription("epi.elevation.avgdem.step","Step of sub-sampling for average elevation estimation");
    SetDefaultParameterInt("epi.elevation.avgdem.step",1);
    SetMinimumParameterIntValue("epi.elevation.avgdem.step",1);
    MandatoryOff("epi.elevation.avgdem.step");
    DisableParameter("epi.elevation.avgdem.step");

    AddParameter(ParameterType_Float,"epi.elevation.avgdem.value","Average elevation value");
    SetParameterDescription("epi.elevation.avgdem.value","Average elevation value estimated from DEM");
    SetParameterRole("epi.elevation.avgdem.value",Role_Output);
    DisableParameter("epi.elevation.avgdem.value");

    AddParameter(ParameterType_Float,"epi.elevation.avgdem.mindisp","Minimum disparity from DEM");
    SetParameterDescription("epi.elevation.avgdem.mindisp","Disparity "
      "corresponding to estimated minimum elevation over the left image");
    SetParameterRole("epi.elevation.avgdem.mindisp",Role_Output);
    DisableParameter("epi.elevation.avgdem.mindisp");

    AddParameter(ParameterType_Float,"epi.elevation.avgdem.maxdisp","Maximum disparity from DEM");
    SetParameterDescription("epi.elevation.avgdem.maxdisp","Disparity "
      "corresponding to estimated maximum elevation over the left image");
    SetParameterRole("epi.elevation.avgdem.maxdisp",Role_Output);
    DisableParameter("epi.elevation.avgdem.maxdisp");

    AddParameter(ParameterType_Float,"epi.scale","Scale of epipolar images");
    SetParameterDescription("epi.scale","The scale parameter allows generating"
      " zoomed-in (scale < 1) or zoomed-out (scale > 1) epipolar images.");
    SetDefaultParameterFloat("epi.scale",1.);

    AddParameter(ParameterType_Int,"epi.step","Step of the deformation grid (in number of pixels)");
    SetParameterDescription("epi.step","Stereo-rectification deformation grid "
      "only varies slowly. Therefore, it is recommended to use a coarser grid "
      "(higher step value) in case of large images");
    SetDefaultParameterInt("epi.step",1);

    AddParameter(ParameterType_Int,"epi.rectsizex","Rectified image size X");
    SetParameterDescription("epi.rectsizex","The application computes the "
      "optimal rectified image size so that the whole left input image fits "
      "into the rectified area. However, due to the scale and step parameter, "
      "this size may not match the size of the deformation field output. In "
      "this case, one can use these output values.");
    SetParameterRole("epi.rectsizex", Role_Output);

    AddParameter(ParameterType_Int,"epi.rectsizey","Rectified image size Y");
    SetParameterDescription("epi.rectsizey","The application computes the "
      "optimal rectified image size so that the whole left input image fits "
      "into the rectified area. However, due to the scale and step parameter, "
      "this size may not match the size of the deformation field output. In "
      "this case, one can use these output values.");
    SetParameterRole("epi.rectsizey", Role_Output);

    AddParameter(ParameterType_Float,"epi.baseline","Mean baseline ratio");
    SetParameterDescription("epi.baseline","This parameter is the mean value, "
      "in pixels.meters^-1, of the baseline to sensor altitude ratio. It can be"
      " used to convert disparities to physical elevation, since a disparity of"
      " one pixel will correspond to an elevation offset of the invert of this"
      " value with respect to the mean elevation.");
    SetParameterRole("epi.baseline", Role_Output);

    AddParameter(ParameterType_Group,"inverse","Write inverse fields");
    SetParameterDescription("inverse","This group of parameter allows "
      "generating the inverse fields as well");

    AddParameter(ParameterType_OutputImage, "inverse.outleft", "Left inverse deformation grid");
    SetParameterDescription("inverse.outleft","The deformation grid to resample"
      " the left image from the epipolar geometry back into its original sensor"
      " geometry.");
    MandatoryOff("inverse.outleft");

    AddParameter(ParameterType_OutputImage, "inverse.outright", "Right inverse deformation grid");
    SetParameterDescription("inverse.outright","The output deformation grid to"
      " resample the right image from the epipolar geometry back into its "
      "original sensor geometry.");
    MandatoryOff("inverse.outright");

    AddParameter(ParameterType_Int, "inverse.ssrate", "Sub-sampling rate for inversion");
    SetParameterDescription("inverse.ssrate","Grid inversion is an heavy "
      "process that implies spline regression on control points. To avoid "
      "eating to much memory, this parameter allows one to first sub-sample "
      "the field to invert.");
    SetDefaultParameterInt("inverse.ssrate",16);
    SetMinimumParameterIntValue("inverse.ssrate",1);

    // Doc example
    SetDocExampleParameterValue("io.inleft","wv2_xs_left.tif");
    SetDocExampleParameterValue("io.inright","wv2_xs_left.tif");
    SetDocExampleParameterValue("io.outleft","wv2_xs_left_epi_field.tif");
    SetDocExampleParameterValue("io.outright","wv2_xs_right_epi_field.tif");
    SetDocExampleParameterValue("epi.elevation.default","400");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here
  }

  void DoExecute() override
  {
    m_DisplacementFieldSource->SetLeftImage(GetParameterImage("io.inleft"));
    m_DisplacementFieldSource->SetRightImage(GetParameterImage("io.inright"));
    m_DisplacementFieldSource->SetGridStep(GetParameterInt("epi.step"));
    m_DisplacementFieldSource->SetScale(GetParameterFloat("epi.scale"));

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"epi.elevation");

    if (otb::Wrapper::ElevationParametersHandler::IsGeoidUsed(this,"epi.elevation") &&
        otb::Wrapper::ElevationParametersHandler::IsDEMUsed(this,"epi.elevation") &&
        !IsParameterEnabled("epi.elevation.avgdem.step"))
      {
      m_DisplacementFieldSource->SetUseDEM(true);
      }

    if(IsParameterEnabled("epi.elevation.avgdem.step"))
      {
      FloatImageType::PointType   origin  = GetParameterImage("io.inleft")->GetOrigin();
      FloatImageType::SizeType    size    = GetParameterImage("io.inleft")->GetLargestPossibleRegion().GetSize();
      FloatImageType::SpacingType spacing = GetParameterImage("io.inleft")->GetSignedSpacing();

      size[0]/=GetParameterInt("epi.elevation.avgdem.step");
      size[1]/=GetParameterInt("epi.elevation.avgdem.step");
      spacing[0]*=GetParameterInt("epi.elevation.avgdem.step");
      spacing[1]*=GetParameterInt("epi.elevation.avgdem.step");

      m_DEMToImageGenerator->SetOutputOrigin(origin);
      m_DEMToImageGenerator->SetOutputSize(size);
      m_DEMToImageGenerator->SetOutputSpacing(spacing);
      m_DEMToImageGenerator->SetOutputProjectionRef(GetParameterImage("io.inleft")->GetProjectionRef());
      m_DEMToImageGenerator->SetOutputKeywordList(GetParameterImage("io.inleft")->GetImageKeywordlist());
      m_DEMToImageGenerator->AboveEllipsoidOn();

      m_StatisticsFilter->SetInput(m_DEMToImageGenerator->GetOutput());
      m_StatisticsFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
      AddProcess(m_StatisticsFilter->GetStreamer(),"Computing DEM statistics ...");
      m_StatisticsFilter->Update();

      otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(m_StatisticsFilter->GetMean());

      EnableParameter("epi.elevation.avgdem.value");
      SetParameterFloat("epi.elevation.avgdem.value",m_StatisticsFilter->GetMean());

      }

    AddProcess(m_DisplacementFieldSource, "Computing epipolar grids ...");

    m_DisplacementFieldSource->Update();

    SetParameterInt("epi.rectsizex",m_DisplacementFieldSource->GetRectifiedImageSize()[0]);
    SetParameterInt("epi.rectsizey",m_DisplacementFieldSource->GetRectifiedImageSize()[1]);
    SetParameterFloat("epi.baseline",m_DisplacementFieldSource->GetMeanBaselineRatio());

    if(IsParameterEnabled("epi.elevation.avgdem.step"))
      {
      EnableParameter("epi.elevation.avgdem.mindisp");
      otbAppLogINFO("Minimum height of averaged DEM : "<<m_StatisticsFilter->GetMinimum());
      SetParameterFloat("epi.elevation.avgdem.mindisp",(m_StatisticsFilter->GetMinimum()-m_StatisticsFilter->GetMean())*m_DisplacementFieldSource->GetMeanBaselineRatio());
      EnableParameter("epi.elevation.avgdem.maxdisp");
      SetParameterFloat("epi.elevation.avgdem.maxdisp",(m_StatisticsFilter->GetMaximum()-m_StatisticsFilter->GetMean())*m_DisplacementFieldSource->GetMeanBaselineRatio());
      otbAppLogINFO("Maximum height of averaged DEM : "<<m_StatisticsFilter->GetMaximum());
      }


    SetParameterOutputImage("io.outleft",m_DisplacementFieldSource->GetLeftDisplacementFieldOutput());
    SetParameterOutputImage("io.outright",m_DisplacementFieldSource->GetRightDisplacementFieldOutput());

    // Inverse part
    // lots of casting here ...

    // Left field inversion
    if(IsParameterEnabled("inverse.outleft"))
      {

      m_LeftDisplacementFieldCaster->SetInput(m_DisplacementFieldSource->GetLeftDisplacementFieldOutput());

      m_LeftInvertDisplacementFieldFilter->SetInput(m_LeftDisplacementFieldCaster->GetOutput());

      FloatVectorImageType::PointType lorigin = GetParameterImage("io.inleft")->GetOrigin();
      FloatVectorImageType::SpacingType lspacing = GetParameterImage("io.inleft")->GetSignedSpacing();
      FloatVectorImageType::SizeType lsize = GetParameterImage("io.inleft")->GetLargestPossibleRegion().GetSize();

      if (lsize[0]*lsize[1]>256*256)
        otbAppLogWARNING("Left image's size (" << lsize[0] << "x" << lsize[1] << ") may result in a too high computational demand. Please consider setting inverse.ssrate parameter correctly (actual value is " << GetParameterInt("inverse.ssrate") << ").");

      lspacing[0]*=GetParameterInt("epi.step");
      lspacing[1]*=GetParameterInt("epi.step");

      lsize[0]/=GetParameterInt("epi.step");
      lsize[1]/=GetParameterInt("epi.step");

      lsize[0]+=1;
      lsize[1]+=1;

      m_LeftInvertDisplacementFieldFilter->SetOutputOrigin(lorigin);
      m_LeftInvertDisplacementFieldFilter->SetOutputSpacing(lspacing);
      m_LeftInvertDisplacementFieldFilter->SetSize(lsize);
      m_LeftInvertDisplacementFieldFilter->SetSubsamplingFactor(GetParameterInt("inverse.ssrate"));
      AddProcess(m_LeftInvertDisplacementFieldFilter, "Inverting left deformation field ...");
      m_LeftInvertDisplacementFieldFilter->Update();

      m_LeftIndexSelectionFilter1->SetInput(m_LeftInvertDisplacementFieldFilter->GetOutput());
      m_LeftIndexSelectionFilter1->SetIndex(0);
      m_LeftIndexSelectionFilter2->SetInput(m_LeftInvertDisplacementFieldFilter->GetOutput());
      m_LeftIndexSelectionFilter2->SetIndex(1);
      m_LeftImageList->Clear();
      m_LeftImageList->PushBack(m_LeftIndexSelectionFilter1->GetOutput());
      m_LeftImageList->PushBack(m_LeftIndexSelectionFilter2->GetOutput());
      m_LeftImageListFilter->SetInput(m_LeftImageList);

      SetParameterOutputImage("inverse.outleft",m_LeftImageListFilter->GetOutput());
      }

    // Right field inversion

    if(IsParameterEnabled("inverse.outright"))
      {
      m_RightDisplacementFieldCaster->SetInput(m_DisplacementFieldSource->GetRightDisplacementFieldOutput());

      m_RightInvertDisplacementFieldFilter->SetInput(m_RightDisplacementFieldCaster->GetOutput());
      FloatVectorImageType::PointType rorigin = GetParameterImage("io.inright")->GetOrigin();
      FloatVectorImageType::SpacingType rspacing = GetParameterImage("io.inright")->GetSignedSpacing();
      FloatVectorImageType::SizeType rsize = GetParameterImage("io.inright")->GetLargestPossibleRegion().GetSize();

      if (rsize[0]*rsize[1]>256*256)
        otbAppLogWARNING("Right image's size (" << rsize[0] << "x" << rsize[1] << ") may result in a too high computational demand. Please consider setting inverse.ssrate parameter correctly (actual value is " << GetParameterInt("inverse.ssrate") << ").");

      rspacing[0]*=GetParameterInt("epi.step");
      rspacing[1]*=GetParameterInt("epi.step");

      rsize[0]/=GetParameterInt("epi.step");
      rsize[1]/=GetParameterInt("epi.step");

      rsize[0]+=1;
      rsize[1]+=1;

      m_RightInvertDisplacementFieldFilter->SetOutputOrigin(rorigin);
      m_RightInvertDisplacementFieldFilter->SetOutputSpacing(rspacing);
      m_RightInvertDisplacementFieldFilter->SetSize(rsize);
      m_RightInvertDisplacementFieldFilter->SetSubsamplingFactor(GetParameterInt("inverse.ssrate"));

      AddProcess(m_RightInvertDisplacementFieldFilter, "Inverting right deformation field ...");
      m_RightInvertDisplacementFieldFilter->Update();

      m_RightIndexSelectionFilter1->SetInput(m_RightInvertDisplacementFieldFilter->GetOutput());
      m_RightIndexSelectionFilter1->SetIndex(0);
      m_RightIndexSelectionFilter2->SetInput(m_RightInvertDisplacementFieldFilter->GetOutput());
      m_RightIndexSelectionFilter2->SetIndex(1);
      m_RightImageList->Clear();
      m_RightImageList->PushBack(m_RightIndexSelectionFilter1->GetOutput());
      m_RightImageList->PushBack(m_RightIndexSelectionFilter2->GetOutput());
      m_RightImageListFilter->SetInput(m_RightImageList);

      SetParameterOutputImage("inverse.outright",m_RightImageListFilter->GetOutput());
      }
  }

  DisplacementFieldSourceType::Pointer         m_DisplacementFieldSource;
  InverseDisplacementFieldFilterType::Pointer  m_LeftInvertDisplacementFieldFilter;
  DisplacementFieldCastFilterType::Pointer     m_LeftDisplacementFieldCaster;
  IndexSelectionCastFilterType::Pointer        m_LeftIndexSelectionFilter1;
  IndexSelectionCastFilterType::Pointer        m_LeftIndexSelectionFilter2;
  ImageListType::Pointer                       m_LeftImageList;
  ImageListFilterType::Pointer                 m_LeftImageListFilter;

  InverseDisplacementFieldFilterType::Pointer  m_RightInvertDisplacementFieldFilter;
  DisplacementFieldCastFilterType::Pointer     m_RightDisplacementFieldCaster;
  IndexSelectionCastFilterType::Pointer        m_RightIndexSelectionFilter1;
  IndexSelectionCastFilterType::Pointer        m_RightIndexSelectionFilter2;
  ImageListType::Pointer                       m_RightImageList;
  ImageListFilterType::Pointer                 m_RightImageListFilter;

  DEMToImageGeneratorType::Pointer             m_DEMToImageGenerator;
  StatisticsFilterType::Pointer                m_StatisticsFilter;

};
} // End namespace Wrapper
} // End namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::StereoRectificationGridGenerator)
