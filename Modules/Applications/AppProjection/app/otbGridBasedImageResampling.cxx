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

#include "otbStreamingWarpImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbBandMathImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageToVectorImageCastFilter.h"
#include "itkVectorCastImageFilter.h"


namespace otb
{
enum
{
  Interpolator_NNeighbor,
  Interpolator_Linear,
  Interpolator_BCO
};

namespace Wrapper
{

class GridBasedImageResampling : public Application
{
public:
  /** Standard class typedefs. */
  typedef GridBasedImageResampling      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::Vector<double,2>         DisplacementType;
  typedef otb::Image<DisplacementType>   DisplacementFieldType;

  typedef itk::VectorCastImageFilter
  <FloatVectorImageType,
   DisplacementFieldType>                DisplacementFieldCastFilterType;


  typedef otb::StreamingWarpImageFilter
  <FloatVectorImageType,
   FloatVectorImageType,
   DisplacementFieldType>                WarpFilterType;
  typedef otb::MultiToMonoChannelExtractROI
  <FloatVectorImageType::InternalPixelType,
   FloatVectorImageType::InternalPixelType>
                                        ExtractFilterType;

  typedef otb::BandMathImageFilter
  <ExtractFilterType::OutputImageType>  BandMathFilterType;

  typedef otb::ImageToVectorImageCastFilter
  <ExtractFilterType::OutputImageType,
   FloatVectorImageType>                VectorCastFilterType;

  typedef otb::ConcatenateVectorImageFilter
  <FloatVectorImageType,
   FloatVectorImageType,
   FloatVectorImageType>                ConcatenateFilterType;


  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(GridBasedImageResampling, otb::Application);

private:

  GridBasedImageResampling()
  {
    // Instanciate warp filter
    m_WarpImageFilter = WarpFilterType::New();
    m_BandMathX = BandMathFilterType::New();
    m_BandMathY = BandMathFilterType::New();
    m_ExtractX = ExtractFilterType::New();
    m_ExtractY = ExtractFilterType::New();
    m_VectorCastX = VectorCastFilterType::New();
    m_VectorCastY = VectorCastFilterType::New();
    m_Concatenate = ConcatenateFilterType::New();
    m_DisplacementFieldCaster = DisplacementFieldCastFilterType::New();
  }

 void DoInit()
  {
    SetName("GridBasedImageResampling");
    SetDescription("Resamples an image according to a resampling grid");

    SetDocName("Grid Based Image Resampling");
    SetDocLongDescription("This application allows performing image resampling from an input resampling grid.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");

    AddDocTag(Tags::Geometry);

    SetDocSeeAlso("otbStereorecificationGridGeneration");

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows setting the input and output images.");
    AddParameter(ParameterType_InputImage,"io.in","Input image");
    SetParameterDescription("io.in","The input image to resample");
    AddParameter(ParameterType_OutputImage, "io.out", "Output Image");
    SetParameterDescription("io.out","The resampled output image");

    AddParameter(ParameterType_Group,"grid","Resampling grid parameters");
    AddParameter(ParameterType_InputImage,"grid.in","Input resampling grid");
    SetParameterDescription("grid.in","The resampling grid");
    AddParameter(ParameterType_Choice,   "grid.type", "Grid Type");
    SetParameterDescription("grid.type","allows one to choose between two grid types");
    AddChoice("grid.type.def","Displacement  grid: $G(x_out,y_out) = (x_in-x_out, y_in-y_out)$");
    SetParameterDescription("grid.type.def","A deformation grid contains at each grid position the offset to apply to this position in order to get to the corresponding point in the input image to resample");
    AddChoice("grid.type.loc","Localisation grid: $G(x_out,y_out) = (x_in, y_in)$");
    SetParameterDescription("grid.type.loc","A localisation grid contains at each grid position the corresponding position in the input image to resample");


    AddParameter(ParameterType_Group, "out", "Output Image parameters");
    SetParameterDescription("out","Parameters of the output image");

    AddParameter(ParameterType_Float, "out.ulx", "Upper Left X");
    SetParameterDescription("out.ulx","X Coordinate of the upper-left pixel of the output resampled image");
    SetDefaultParameterFloat("out.ulx",0);
    AddParameter(ParameterType_Float, "out.uly", "Upper Left Y");
    SetParameterDescription("out.uly","Y Coordinate of the upper-left pixel of the output resampled image");
    SetDefaultParameterFloat("out.uly",0);

    AddParameter(ParameterType_Int, "out.sizex", "Size X");
    SetParameterDescription("out.sizex","Size of the output resampled image along X (in pixels)");
    AddParameter(ParameterType_Int, "out.sizey", "Size Y");
    SetParameterDescription("out.sizey","Size of the output resampled image along Y (in pixels)");
    AddParameter(ParameterType_Float, "out.spacingx", "Pixel Size X");
    SetParameterDescription("out.spacingx","Size of each pixel along X axis");
    SetDefaultParameterFloat("out.spacingx",1.);
    AddParameter(ParameterType_Float, "out.spacingy", "Pixel Size Y");
    SetParameterDescription("out.spacingy","Size of each pixel along Y axis");
    SetDefaultParameterFloat("out.spacingy",1.);

    AddParameter(ParameterType_Float,"out.default","Default value");
    SetParameterDescription("out.default","The default value to give to pixel that falls outside of the input image.");
    SetDefaultParameterFloat("out.default",0);


    // Interpolators
    AddParameter(ParameterType_Choice,   "interpolator", "Interpolation");
    SetParameterDescription("interpolator","This group of parameters allows one to define how the input image will be interpolated during resampling.");
    AddChoice("interpolator.nn",     "Nearest Neighbor interpolation");
    SetParameterDescription("interpolator.nn","Nearest neighbor interpolation leads to poor image quality, but it is very fast.");
    AddChoice("interpolator.linear", "Linear interpolation");
    SetParameterDescription("interpolator.linear","Linear interpolation leads to average image quality but is quite fast");
    AddChoice("interpolator.bco",    "Bicubic interpolation");
    AddParameter(ParameterType_Radius, "interpolator.bco.radius", "Radius for bicubic interpolation");
    SetParameterDescription("interpolator.bco.radius","This parameter allows controlling the size of the bicubic interpolation filter. If the target pixel size is higher than the input pixel size, increasing this parameter will reduce aliasing artefacts.");
    SetDefaultParameterInt("interpolator.bco.radius", 2);
    SetParameterString("interpolator","bco");

    AddRAMParameter();

    // Doc example
    SetDocExampleParameterValue("io.in","ROI_IKO_PAN_LesHalles_sub.tif");
    SetDocExampleParameterValue("io.out","ROI_IKO_PAN_LesHalles_sub_resampled.tif uint8");
    SetDocExampleParameterValue("grid.in","ROI_IKO_PAN_LesHalles_sub_deformation_field.tif");
    SetDocExampleParameterValue("out.sizex","256");
    SetDocExampleParameterValue("out.sizey","256");
    SetDocExampleParameterValue("grid.type","def");
  }

 void DoUpdateParameters()
  {
    // Nothing to do here
  }

void DoExecute()
    {
      // Get the input image
      FloatVectorImageType* inImage = GetParameterImage("io.in");

      // Get the resampling grid
      FloatVectorImageType * inGrid = GetParameterImage("grid.in");

      if(inGrid->GetNumberOfComponentsPerPixel() != 2)
        {
        itkExceptionMacro(<<"Number of components of the grid is not 2, this is probably not an image of 2D resampling grid.");
        }

      // In case of localisation grid, we must internally convert to
      // deformation grid, which is the only type handled by StreamingWarpImageFilter
      if(GetParameterString("grid.type") == "loc")
        {
        GetLogger()->Info("Grid intepreted as a location grid.");
        m_ExtractX->SetInput(inGrid);
        m_ExtractX->SetChannel(1);
        m_BandMathX->SetNthInput(0,m_ExtractX->GetOutput(),"locX");
        m_BandMathX->SetExpression("locX-idxPhyX");
        m_ExtractY->SetInput(inGrid);
        m_ExtractY->SetChannel(2);
        m_BandMathY->SetNthInput(0,m_ExtractY->GetOutput(),"locY");
        m_BandMathY->SetExpression("locY-idxPhyY");
        m_VectorCastX->SetInput(m_BandMathX->GetOutput());
        m_Concatenate->SetInput1(m_VectorCastX->GetOutput());
        m_VectorCastY->SetInput(m_BandMathY->GetOutput());
        m_Concatenate->SetInput2(m_VectorCastY->GetOutput());
        m_DisplacementFieldCaster->SetInput(m_Concatenate->GetOutput());
        }
      else
        {
        GetLogger()->Info("Grid intepreted as a deformation grid.");
        m_DisplacementFieldCaster->SetInput(inGrid);
        }

      m_DisplacementFieldCaster->GetOutput()->UpdateOutputInformation();

      m_WarpImageFilter->SetDisplacementField(m_DisplacementFieldCaster->GetOutput());

      // Set inputs
      m_WarpImageFilter->SetInput(inImage);

    // Get Interpolator
    switch ( GetParameterInt("interpolator") )
      {
      case Interpolator_Linear:
      {
      typedef itk::LinearInterpolateImageFunction<FloatVectorImageType,
        double>          LinearInterpolationType;
      LinearInterpolationType::Pointer interpolator = LinearInterpolationType::New();
      m_WarpImageFilter->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_NNeighbor:
      {
      typedef itk::NearestNeighborInterpolateImageFunction<FloatVectorImageType,
        double> NearestNeighborInterpolationType;
      NearestNeighborInterpolationType::Pointer interpolator = NearestNeighborInterpolationType::New();
      m_WarpImageFilter->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_BCO:
      {
      typedef otb::BCOInterpolateImageFunction<FloatVectorImageType> BCOInterpolationType;
      BCOInterpolationType::Pointer interpolator = BCOInterpolationType::New();
      interpolator->SetRadius(GetParameterInt("interpolator.bco.radius"));
      m_WarpImageFilter->SetInterpolator(interpolator);
      }
      break;
      }


    // Set Output information
    WarpFilterType::SizeType size;
    size[0] = GetParameterInt("out.sizex");
    size[1] = GetParameterInt("out.sizey");
    m_WarpImageFilter->SetOutputSize(size);

    WarpFilterType::SpacingType spacing;
    spacing[0] = GetParameterFloat("out.spacingx");
    spacing[1] = GetParameterFloat("out.spacingy");
    m_WarpImageFilter->SetOutputSpacing(spacing);

    WarpFilterType::PointType ul;
    ul[0] = GetParameterFloat("out.ulx");
    ul[1] = GetParameterFloat("out.uly");
    m_WarpImageFilter->SetOutputOrigin(ul);

    // Build the default pixel
    FloatVectorImageType::PixelType defaultValue;
    defaultValue.SetSize(inImage->GetNumberOfComponentsPerPixel());
    defaultValue.Fill(GetParameterFloat("out.default"));

    m_WarpImageFilter->SetEdgePaddingValue(defaultValue);

    // Output Image
    SetParameterOutputImage("io.out", m_WarpImageFilter->GetOutput());

    }

  WarpFilterType::Pointer        m_WarpImageFilter;
  ExtractFilterType::Pointer     m_ExtractX;
  ExtractFilterType::Pointer     m_ExtractY;
  BandMathFilterType::Pointer    m_BandMathX;
  BandMathFilterType::Pointer    m_BandMathY;
  VectorCastFilterType::Pointer  m_VectorCastX;
  VectorCastFilterType::Pointer  m_VectorCastY;
  ConcatenateFilterType::Pointer m_Concatenate;
  DisplacementFieldCastFilterType::Pointer m_DisplacementFieldCaster;
};
} // End namespace Wrapper
} // End namepsace otb


OTB_APPLICATION_EXPORT(otb::Wrapper::GridBasedImageResampling)
