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

#include "otbStereorectificationDeformationFieldSource.h"

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

  typedef otb::StereorectificationDeformationFieldSource
  <FloatVectorImageType,FloatVectorImageType> DeformationFieldSourceType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(StereoRectificationGridGenerator, otb::Application);

private:

  StereoRectificationGridGenerator()
  {
    // Instanciate deformation field source
    m_DeformationFieldSource = DeformationFieldSourceType::New();
  }

 void DoInit()
  {
    SetName("StereoRectificationGridGenerator");
    SetDescription("Generates two deformation fields to stereo-rectify (i.e. resample in epipolar geometry) a pair of stereo images up to the sensor model precision");

    SetDocName("Stereo-rectification deformation grid generator");
    SetDocLongDescription("This application generates a pair of deformation grid to stereo-rectify a pair of stereo images according to sensor modelling and a mean elevation hypothesis. The deformation grids can be passed to the StereoRectificationGridGenerator application for actual resampling in epipolar geometry.");
    SetDocLimitations("Generation of the deformation grid is not streamable, pay attention to this fact when setting the grid step.");
    SetDocAuthors("OTB-Team");

    AddDocTag(Tags::Geometry);

    SetDocSeeAlso("otbGridBasedImageResampling");

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows to set the input and output images.");
    AddParameter(ParameterType_InputImage,"io.inleft","Left input image");
    SetParameterDescription("io.inleft","The left input image to resample");

    AddParameter(ParameterType_InputImage,"io.inright","Right input image");
    SetParameterDescription("io.inright","The right input image to resample");

    AddParameter(ParameterType_OutputImage, "io.outleft", "Left output deformation grid");
    SetParameterDescription("io.outleft","The output deformation grid to be used to resample the left input image");

    AddParameter(ParameterType_OutputImage, "io.outright", "Right output deformation grid");
    SetParameterDescription("io.outright","The output deformation grid to be used to resample the right input image");

    AddParameter(ParameterType_Group,"epi","Epipolar  geometry and grid parameters");
    SetParameterDescription("epi","Parameters of the epipolar geometry and output grids");
    
    AddParameter(ParameterType_Float,"epi.elevation","Mean elevation (in meters)");
    SetParameterDescription("epi.elevation","In the output rectified images, corresponding pixels whose elevation is equal to the mean elevation will have a null disparity");
    SetDefaultParameterFloat("epi.elevation",0.);

    AddParameter(ParameterType_Float,"epi.scale","Scale of epipolar images");
    SetParameterDescription("epi.scale","The scale parameter allows to generated zoomed-in (scale < 1) or zoomed-out (scale > 1) epipolar images.");
    SetDefaultParameterFloat("epi.scale",1.);
    
    AddParameter(ParameterType_Int,"epi.step","Step of the deformation grid (in nb. of pixels)");
    SetParameterDescription("epi.step","Stereo-rectification deformation grid only varies slowly. Therefore, it is recommanded to use a coarser grid (higher step value) in case of large images");
    SetDefaultParameterInt("epi.step",1);

    AddParameter(ParameterType_Int,"epi.rectsizex","Rectified image size X");
    SetParameterDescription("epi.rectsizex","The application computes the optimal rectified image size so that the whole left input image fits into the rectified area. However, due to the scale and step parameter, this size may not match the size of the deformation field output. In this case, one can use these output values.");
    SetParameterRole("epi.rectsizex", Role_Output);

    AddParameter(ParameterType_Int,"epi.rectsizey","Rectified image size Y");
    SetParameterDescription("epi.rectsizey","The application computes the optimal rectified image size so that the whole left input image fits into the rectified area. However, due to the scale and step parameter, this size may not match the size of the deformation field output. In this case, one can use these output values.");
    SetParameterRole("epi.rectsizey", Role_Output);

    AddParameter(ParameterType_Float,"epi.baseline","Mean baseline ratio");
    SetParameterDescription("epi.baseline","This parameter is the mean value, in meters, of the baseline to sensor altitude ratio. It can be used to convert disparities to physical elevation, since a disparity of one pixel will corresspond to an elevation offset of this value with respect to the mean elevation.");
    SetParameterRole("epi.baseline", Role_Output);

    // Doc example
    // SetDocExampleParameterValue("io.in","ROI_IKO_PAN_LesHalles_sub.tif");
    // SetDocExampleParameterValue("io.out","ROI_IKO_PAN_LesHalles_sub_resampled.tif uint8");
    // SetDocExampleParameterValue("grid.in","ROI_IKO_PAN_LesHalles_sub_deformation_field.tif");
    // SetDocExampleParameterValue("out.sizex","256");
    // SetDocExampleParameterValue("out.sizey","256");
    // SetDocExampleParameterValue("grid.type","def");
  }

 void DoUpdateParameters()
  {
    // Nothing to do here
  }

void DoExecute()
    {
      m_DeformationFieldSource->SetLeftImage(GetParameterImage("io.inleft"));
      m_DeformationFieldSource->SetRightImage(GetParameterImage("io.inright"));
      m_DeformationFieldSource->SetGridStep(GetParameterInt("epi.step"));
      m_DeformationFieldSource->SetScale(GetParameterFloat("epi.scale"));
      m_DeformationFieldSource->SetAverageElevation(GetParameterFloat("epi.elevation"));
                                                    
      AddProcess(m_DeformationFieldSource, "Computing epipolar grids ...");

      m_DeformationFieldSource->Update();

      SetParameterInt("epi.rectsizex",m_DeformationFieldSource->GetRectifiedImageSize()[0]);
      SetParameterInt("epi.rectsizey",m_DeformationFieldSource->GetRectifiedImageSize()[1]);
      SetParameterFloat("epi.baseline",m_DeformationFieldSource->GetMeanBaselineRatio());

      SetParameterOutputImage("io.outleft",m_DeformationFieldSource->GetLeftDeformationFieldOutput());

      SetParameterOutputImage("io.outright",m_DeformationFieldSource->GetRightDeformationFieldOutput());
    }

  DeformationFieldSourceType::Pointer m_DeformationFieldSource;
};
} // End namespace Wrapper
} // End namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::StereoRectificationGridGenerator)
