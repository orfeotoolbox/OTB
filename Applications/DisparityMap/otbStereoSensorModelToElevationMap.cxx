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

#include "otbImage.h"
#include "otbStereoSensorModelToElevationMapFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbVectorImageToIntensityImageFilter.h"

#include "itkDiscreteGaussianImageFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class StereoSensorModelToElevationMap : public Application
{
public:
  /** Standard class typedefs. */
  typedef StereoSensorModelToElevationMap Self;
  typedef Application                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(StereoSensorModelToElevationMap, otb::Application);

  /** Filters typedef */
  typedef otb::Image<short, 2> ImageType;
  typedef otb::Image<float, 2> HeightImageType;
  
  typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType> GaussianFilterType;
  typedef otb::StereoSensorModelToElevationFilter<ImageType, HeightImageType> StereoFilterType;
  typedef otb::VectorImage<float, 2> VectorImageType;
  typedef otb::ImageList<HeightImageType> ImageListType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType> IL2VIFilterType;
  
  /* conversion from VectorImage to Intensity Image */
  typedef otb::VectorImageToIntensityImageFilter<FloatVectorImageType, ImageType> IntensityFilterType;
  

private:
  void DoInit()
  {
    SetName("StereoSensorModelToElevationMap");
    SetDescription("Convert a stereo sensor model to elevation map.");

    // Documentation
    SetDocName("Stereo sensor model to elevation map");
    SetDocLongDescription("Produce an elevation map from a pair of stereo images along-track by implicit exploration of their epipolar lines.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
 
    AddDocTag(Tags::Stereo);

    // Mandatory parameters
    AddParameter(ParameterType_InputImage,  "ref",   "Reference");
    SetParameterDescription("ref", "The reference image.");
    AddParameter(ParameterType_InputImage,  "sec",   "Secondary");
    SetParameterDescription("sec", "The secondary image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "The output image.");
    
    // Optional parameters
    AddRAMParameter();
    
    AddParameter(ParameterType_Int, "r",  "Radius");
    SetParameterDescription("r", "Radius (in pixels) of the metric computation window (default is 3).");
    SetDefaultParameterInt("r", 3);
    SetMinimumParameterIntValue("r", 0);
    MandatoryOff("r");
    
    AddParameter(ParameterType_Float, "ct",  "Correlation Threshold");
    SetParameterDescription("ct", "Threshold bellow which correlation is considered invalid (default is 0.7).");
    SetDefaultParameterFloat("ct", 0.7);
    SetMinimumParameterFloatValue("ct", 0.0);
    MandatoryOff("ct");
    
    AddParameter(ParameterType_Float, "vt",  "Variance Threshold");
    SetParameterDescription("vt", "Threshold on the master patch variance bellow which no height exploration is performed (default is 4).");
    SetDefaultParameterFloat("vt", 4.0);
    SetMinimumParameterFloatValue("vt", 0.0);
    MandatoryOff("vt");
    
    AddParameter(ParameterType_Float, "minh",  "MinHeightOffset");
    SetParameterDescription("minh", "Minimum height offset with respect to local initial height for height exploration (default is -20 meters).");
    SetDefaultParameterFloat("minh", -20.0);
    MandatoryOff("minh");
    
    AddParameter(ParameterType_Float, "maxh",  "MaxHeightOffset");
    SetParameterDescription("maxh", "Maximum height offset with respect to local initial height for height exploration (default is 20 meters).");
    SetDefaultParameterFloat("maxh", 20.0);
    MandatoryOff("maxh");
    
    AddParameter(ParameterType_Float, "step",  "HeightStep");
    SetParameterDescription("step", "Step of height exploration (default is 1 meter).");
    SetDefaultParameterFloat("step", 1.0);
    SetMinimumParameterFloatValue("step", 0.0);
    MandatoryOff("step");
    
    AddParameter(ParameterType_Float, "ae",  "AverageElevation");
    SetParameterDescription("ae", "If no DEM is used, provide the initial height value (default is 0 meters)");
    SetDefaultParameterFloat("ae", 0.0);
    MandatoryOff("ae");
    
    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");
    
    AddParameter(ParameterType_Float, "rgs",  "ReferenceGaussianSmoothing");
    SetParameterDescription("rgs", "(optional) Perform a gaussian smoothing of the reference image. Parameter is gaussian sigma (in pixels). Default is no smoothing.");
    SetDefaultParameterFloat("rgs", 1.0);
    SetMinimumParameterFloatValue("rgs", 0.0);
    MandatoryOff("rgs");
    DisableParameter("rgs");
    
    AddParameter(ParameterType_Float, "sgs",  "SecondaryGaussianSmoothing");
    SetParameterDescription("sgs", "(optional) Perform a gaussian smoothing of the secondary image. Parameter is gaussian sigma (in pixels). Default is no smoothing.");
    SetDefaultParameterFloat("sgs", 1.0);
    SetMinimumParameterFloatValue("sgs", 0.0);
    MandatoryOff("sgs");
    DisableParameter("sgs");
    
    AddParameter(ParameterType_Empty, "s",  "SubtractInitialHeight");
    SetParameterDescription("s", "If activated, the elevation map will contain only the estimated height offset from the initial height (either average elevation or DEM)");
    MandatoryOff("s"); // Not necessary, already optional by default
    

   // Doc example parameter settings
   SetDocExampleParameterValue("ref", "sensor_stereo_left.tif");
   SetDocExampleParameterValue("sec", "sensor_stereo_right.tif");
   SetDocExampleParameterValue("out", "StereoSensorModelToElevationMap.tif");
   SetDocExampleParameterValue("elev", "dem");
   SetDocExampleParameterValue("elev.dem.path", "DEM_srtm");
   SetDocExampleParameterValue("elev.dem.geoid", "DEM_srtm/egm96.grd");
  }

  void DoUpdateParameters()
  {
    // The algorithm needs at least 1 samples in the height exploration
    if (GetParameterFloat("minh") > GetParameterFloat("maxh"))
    {
      SetParameterFloat("maxh", GetParameterFloat("minh") + GetParameterFloat("step"));
    }
  }

  void DoExecute()
  {
    bool referenceSmoothing = IsParameterEnabled("rgs");
    
    bool secondarySmoothing = IsParameterEnabled("sgs");
    
    bool subtractInitialHeight = IsParameterEnabled("s");
     
    FloatVectorImageType::Pointer inputRef = GetParameterImage("ref");
    FloatVectorImageType::Pointer inputSec = GetParameterImage("sec");
    
    m_Intensity1 = IntensityFilterType::New();
    m_Intensity1->SetInput(inputRef);
    
    m_Intensity2 = IntensityFilterType::New();
    m_Intensity2->SetInput(inputSec);
    
    m_Sigma1.Fill(GetParameterFloat("rgs"));
    
    m_Sigma2.Fill(GetParameterFloat("sgs"));
    
    m_Gaussian1 = GaussianFilterType::New();
    m_Gaussian1->SetInput(m_Intensity1->GetOutput());
    m_Gaussian1->SetVariance(m_Sigma1);
    m_Gaussian1->SetUseImageSpacingOff();
  
    m_Gaussian2 = GaussianFilterType::New();
    m_Gaussian2->SetInput(m_Intensity2->GetOutput());
    m_Gaussian2->SetVariance(m_Sigma2);
    m_Gaussian2->SetUseImageSpacingOff();
    
    StereoFilterType::Pointer m_StereoFilter = StereoFilterType::New();
    
    if(referenceSmoothing)
      {
      m_StereoFilter->SetMasterInput(m_Gaussian1->GetOutput());
      }
    else
      {
      m_StereoFilter->SetMasterInput(m_Intensity1->GetOutput());
      }
    if(secondarySmoothing)
      {
      m_StereoFilter->SetSlaveInput(m_Gaussian2->GetOutput());
      }
    else
      {
      m_StereoFilter->SetSlaveInput(m_Intensity2->GetOutput());
      }
  
    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    m_StereoFilter->SetSubtractInitialElevation(subtractInitialHeight);
  
    m_StereoFilter->SetLowerElevation(GetParameterFloat("minh"));
    m_StereoFilter->SetHigherElevation(GetParameterFloat("maxh"));
    m_StereoFilter->SetCorrelationThreshold(GetParameterFloat("ct"));
    m_StereoFilter->SetVarianceThreshold(GetParameterFloat("vt"));
    m_StereoFilter->SetElevationStep(GetParameterFloat("step"));
    m_StereoFilter->SetRadius(GetParameterInt("r"));
    
    AddProcess(m_StereoFilter,"Processing elevation");
    m_StereoFilter->Update();
    
    ImageListType::Pointer il = ImageListType::New();
    
    il->PushBack(m_StereoFilter->GetOutput());
    il->PushBack(m_StereoFilter->GetCorrelationOutput());
    
    IL2VIFilterType::Pointer m_Il2vi = IL2VIFilterType::New();
    m_Il2vi->SetInput(il);
    
    m_Il2vi->Update();
    
    SetParameterOutputImage("out", m_Il2vi->GetOutput());
    
    }

  /* Members */
  IntensityFilterType::Pointer m_Intensity1;
  IntensityFilterType::Pointer m_Intensity2;
  
  GaussianFilterType::ArrayType m_Sigma1;
  GaussianFilterType::ArrayType m_Sigma2;
  
  GaussianFilterType::Pointer m_Gaussian1;
  GaussianFilterType::Pointer m_Gaussian2;
  
  StereoFilterType::Pointer m_StereoFilter;
  
  IL2VIFilterType::Pointer m_Il2vi;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::StereoSensorModelToElevationMap)
