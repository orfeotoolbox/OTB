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

#include "otbBCOInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

//Transform
#include "otbCompositeTransform.h"
#include "itkScalableAffineTransform.h"
#include "itkTranslationTransform.h"
#include "itkIdentityTransform.h"
#include "itkScaleTransform.h"
#include "itkCenteredRigid2DTransform.h"

#include "otbStreamingResampleImageFilter.h"

namespace otb
{
enum
{
  Interpolator_NNeighbor,
  Interpolator_Linear,
  Interpolator_BCO
};

enum
{
  Transform_Identity,
  Transform_Translation,
  Transform_Rotation,
};

namespace Wrapper
{

class RigidTransformResample : public Application
{
public:
  /** Standard class typedefs. */
  typedef RigidTransformResample        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  typedef itk::TranslationTransform<double, FloatVectorImageType::ImageDimension> TransformType;
  typedef otb::StreamingResampleImageFilter<FloatVectorImageType, FloatVectorImageType, double>    ResampleFilterType;

  typedef itk::IdentityTransform<double, FloatVectorImageType::ImageDimension>      IdentityTransformType;

  typedef itk::ScalableAffineTransform<double, FloatVectorImageType::ImageDimension> ScalableTransformType;
  typedef ScalableTransformType::OutputVectorType                         OutputVectorType;

  /** Rotation transform */
  typedef itk::CenteredRigid2DTransform< double > RotationTransformType;
  typedef RotationTransformType::ScalarType              ScalarType;

  itkTypeMacro(RigidTransformResample, otb::Application);

private:

  void DoInit()
  {
    SetName("RigidTransformResample");
    SetDescription("Resample an image with a rigid transform");
    // Documentation
    SetDocName("Image resampling with a rigid transform");
    SetDocLongDescription("This application performs a parametric transform on the input image. Scaling, translation and rotation with scaling factor are handled."
        " Parameters of the transform is expressed in physical units, thus particular attention must be paid on pixel size (value, and sign). Moreover transform is expressed from input space to output space (on the contrary ITK Transforms are expressed form output space to input space). ");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Translation");

    AddDocTag("Conversion");
    AddDocTag(Tags::Geometry);

    AddParameter(ParameterType_InputImage,   "in",   "Input image");
    SetParameterDescription("in","The input image to translate.");
    AddParameter(ParameterType_OutputImage,  "out",  "Output image");
    SetParameterDescription("out","The transformed output image.");

    //Transform
    AddParameter(ParameterType_Group,"transform","Transform parameters");
    SetParameterDescription("transform","This group of parameters allows setting the transformation to apply.");

    AddParameter(ParameterType_Choice, "transform.type", "Type of transformation");
    SetParameterDescription("transform.type","Type of transformation. Available transformations are spatial scaling, translation and rotation with scaling factor");

    AddChoice("transform.type.id", "id");
    SetParameterDescription("transform.type.id","Spatial scaling");

    AddParameter(ParameterType_Float,"transform.type.id.scalex",   "X scaling");
    SetParameterDescription("transform.type.id.scalex","Scaling factor between the output X spacing and the input X spacing");
    SetDefaultParameterFloat("transform.type.id.scalex",1.);
    AddParameter(ParameterType_Float,"transform.type.id.scaley",   "Y scaling");
    SetParameterDescription("transform.type.id.scaley","Scaling factor between the output Y spacing and the input Y spacing");
    SetDefaultParameterFloat("transform.type.id.scaley",1.);

    AddChoice("transform.type.translation", "translation");
    SetParameterDescription("transform.type.translation","translation");

    AddParameter(ParameterType_Float,"transform.type.translation.tx",   "The X translation (in physical units)");
    SetParameterDescription("transform.type.translation.tx","The translation value along X axis (in physical units).");
    SetDefaultParameterFloat("transform.type.translation.tx",0.);
    AddParameter(ParameterType_Float,"transform.type.translation.ty",   "The Y translation (in physical units)");
    SetParameterDescription("transform.type.translation.ty","The translation value along Y axis (in physical units)");
    SetDefaultParameterFloat("transform.type.translation.ty",0.);
    AddParameter(ParameterType_Float,"transform.type.translation.scalex",   "X scaling");
    SetParameterDescription("transform.type.translation.scalex","Scaling factor between the output X spacing and the input X spacing");
    SetDefaultParameterFloat("transform.type.translation.scalex",1.);
    AddParameter(ParameterType_Float,"transform.type.translation.scaley",   "Y scaling");
    SetParameterDescription("transform.type.translation.scaley","Scaling factor between the output Y spacing and the input Y spacing");
    SetDefaultParameterFloat("transform.type.translation.scaley",1.);

    AddChoice("transform.type.rotation", "rotation");
    SetParameterDescription("transform.type.rotation","rotation");

    AddParameter(ParameterType_Float, "transform.type.rotation.angle", "Rotation angle");
    SetParameterDescription("transform.type.rotation.angle","The rotation angle in degree (values between -180 and 180)");
    SetDefaultParameterFloat("transform.type.rotation.angle",0.);

    AddParameter(ParameterType_Float, "transform.type.rotation.scalex", "X scaling");
    SetParameterDescription("transform.type.rotation.scalex","Scale factor between the X spacing of the rotated output image and the X spacing of the unrotated image");
    SetDefaultParameterFloat("transform.type.rotation.scalex",1.);

    AddParameter(ParameterType_Float, "transform.type.rotation.scaley", "Y scaling");
    SetParameterDescription("transform.type.rotation.scaley","Scale factor between the Y spacing of the rotated output image and the Y spacing of the unrotated image");
    SetDefaultParameterFloat("transform.type.rotation.scaley",1.);

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

    // RAM available
    AddRAMParameter("ram");
    SetParameterDescription("ram","This allows setting the maximum amount of RAM available for processing. As the writing task is time consuming, it is better to write large pieces of data, which can be achieved by increasing this parameter (pay attention to your system capabilities)");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_toulouse_sub.tif");
    SetDocExampleParameterValue("out", "rigitTransformImage.tif");
    SetDocExampleParameterValue("transform.type", "rotation");
    SetDocExampleParameterValue("transform.type.rotation.angle", "20");
    SetDocExampleParameterValue("transform.type.rotation.scalex", "2.");
    SetDocExampleParameterValue("transform.type.rotation.scaley", "2.");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    FloatVectorImageType* inputImage = GetParameterImage("in");

    m_Resampler = ResampleFilterType::New();
    m_Resampler->SetInput(inputImage);

    // Get Interpolator
    switch ( GetParameterInt("interpolator") )
      {
      case Interpolator_Linear:
      {
      typedef itk::LinearInterpolateImageFunction<FloatVectorImageType,
                                                  double>          LinearInterpolationType;
      LinearInterpolationType::Pointer interpolator = LinearInterpolationType::New();
      m_Resampler->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_NNeighbor:
      {
      typedef itk::NearestNeighborInterpolateImageFunction<FloatVectorImageType,
                                                           double> NearestNeighborInterpolationType;
      NearestNeighborInterpolationType::Pointer interpolator = NearestNeighborInterpolationType::New();
      m_Resampler->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_BCO:
      {
      typedef otb::BCOInterpolateImageFunction<FloatVectorImageType> BCOInterpolationType;
      BCOInterpolationType::Pointer interpolator = BCOInterpolationType::New();
      interpolator->SetRadius(GetParameterInt("interpolator.bco.radius"));
      m_Resampler->SetInterpolator(interpolator);
      }
      break;
      }

    // Get Transform
    switch ( GetParameterInt("transform.type") )
      {
      case Transform_Identity:
      {
      IdentityTransformType::Pointer transform = IdentityTransformType::New();

      m_Resampler->SetOutputParametersFromImage( inputImage );
      // Scale Transform
      OutputVectorType scale;
      scale[0] = 1.0 / GetParameterFloat("transform.type.id.scalex");
      scale[1] = 1.0 / GetParameterFloat("transform.type.id.scaley");

      // Evaluate spacing
      FloatVectorImageType::SpacingType spacing = inputImage->GetSpacing();
      FloatVectorImageType::SpacingType OutputSpacing;
      OutputSpacing[0] = spacing[0] * scale[0];
      OutputSpacing[1] = spacing[1] * scale[1];

      m_Resampler->SetOutputSpacing(OutputSpacing);

      FloatVectorImageType::PointType origin = inputImage->GetOrigin();
      FloatVectorImageType::PointType outputOrigin;
      outputOrigin[0] = origin[0] + 0.5 * spacing[0] * (scale[0] - 1.0);
      outputOrigin[1] = origin[1] + 0.5 * spacing[1] * (scale[1] - 1.0);

      m_Resampler->SetOutputOrigin(outputOrigin);

      m_Resampler->SetTransform(transform);

      // Evaluate size
      ResampleFilterType::SizeType recomputedSize;
      recomputedSize[0] = inputImage->GetLargestPossibleRegion().GetSize()[0] / scale[0];
      recomputedSize[1] = inputImage->GetLargestPossibleRegion().GetSize()[1] / scale[1];

      m_Resampler->SetOutputSize(recomputedSize);
      otbAppLogINFO( << "Output image size : " << recomputedSize );
      }
      break;

      case Transform_Translation:
      {
      m_Resampler->SetOutputParametersFromImage(inputImage);

      TransformType::Pointer transform = TransformType::New();
      TransformType::OutputVectorType offset;
      offset[0] = -1.0 * GetParameterFloat("transform.type.translation.tx"); // Offset is inverted to make transform from input cs. to output cs.
      offset[1] = -1.0 * GetParameterFloat("transform.type.translation.ty");
      otbAppLogINFO( << "Offset (inverted to respect ITK transform convention e.g. from output space to input space) : " << offset );
      transform->SetOffset(offset);

      // Scale Transform
      OutputVectorType scale;
      scale[0] = 1.0 / GetParameterFloat("transform.type.translation.scalex");
      scale[1] = 1.0 / GetParameterFloat("transform.type.translation.scaley");

      // Evaluate spacing
      FloatVectorImageType::SpacingType spacing = inputImage->GetSpacing();
      FloatVectorImageType::SpacingType OutputSpacing;
      OutputSpacing[0] = spacing[0] * scale[0];
      OutputSpacing[1] = spacing[1] * scale[1];

      m_Resampler->SetOutputSpacing(OutputSpacing);

      FloatVectorImageType::PointType origin = inputImage->GetOrigin();
      FloatVectorImageType::PointType outputOrigin;
      outputOrigin[0] = origin[0] + 0.5 * spacing[0] * (scale[0] - 1.0);
      outputOrigin[1] = origin[1] + 0.5 * spacing[1] * (scale[1] - 1.0);

      m_Resampler->SetOutputOrigin(outputOrigin);

      ResampleFilterType::SizeType recomputedSize;
      recomputedSize[0] = inputImage->GetLargestPossibleRegion().GetSize()[0] / scale[0];
      recomputedSize[1] = inputImage->GetLargestPossibleRegion().GetSize()[1] / scale[1];

      m_Resampler->SetOutputSize(recomputedSize);

      otbAppLogINFO( << "Output image size : " << recomputedSize );
      m_Resampler->SetTransform(transform);

      }
      break;

      case Transform_Rotation:
      {
      ScalableTransformType::Pointer transform = ScalableTransformType::New();

      FloatVectorImageType::SizeType inSize = inputImage->GetLargestPossibleRegion().GetSize();
      FloatVectorImageType::SpacingType spacing = inputImage->GetSpacing();

      itk::ContinuousIndex<double,2> ULindex(inputImage->GetLargestPossibleRegion().GetIndex());
      ULindex[0] += -0.5;
      ULindex[1] += -0.5;
      itk::ContinuousIndex<double,2> center, URindex, LRindex, LLindex;
      center[0] = ULindex[0] + static_cast<double>(inSize[0]) / 2.0;
      center[1] = ULindex[1] + static_cast<double>(inSize[1]) / 2.0;

      URindex = ULindex;
      LRindex = ULindex;
      LLindex = ULindex;
      URindex[0] += inSize[0];
      LRindex[0] += inSize[0];
      LRindex[1] += inSize[1];
      LLindex[1] += inSize[1];

      FloatVectorImageType::PointType centerPoint;
      inputImage->TransformContinuousIndexToPhysicalPoint(center, centerPoint);

      //image boundary
      FloatVectorImageType::PointType ULpoint, URpoint, LRpoint, LLpoint;
      inputImage->TransformContinuousIndexToPhysicalPoint(ULindex, ULpoint);
      inputImage->TransformContinuousIndexToPhysicalPoint(URindex, URpoint);
      inputImage->TransformContinuousIndexToPhysicalPoint(LRindex, LRpoint);
      inputImage->TransformContinuousIndexToPhysicalPoint(LLindex, LLpoint);

      // Scale Transform
      OutputVectorType scale;
      scale[0] = 1.0 / GetParameterFloat("transform.type.rotation.scalex");
      scale[1] = 1.0 / GetParameterFloat("transform.type.rotation.scaley");

      //angle of rotation
      ScalarType rot_angle = GetParameterFloat("transform.type.rotation.angle");
      if (rot_angle < -180 || rot_angle > 180)
        {
        itkExceptionMacro(<<"The rotation angle must value  be between -180 and 180.");
        }

      transform->SetIdentity();
      if(spacing[0] > 0 && spacing[1] > 0) transform->Rotate2D( rot_angle * CONST_PI_180 );
      else transform->Rotate2D( - rot_angle * CONST_PI_180 );

      transform->SetCenter( centerPoint );
      // transform->Scale( scale ); // Scaling is done by modification of the output spacing

      //inverse transform
      ScalableTransformType::Pointer inverseTransform = ScalableTransformType::New();
      transform->GetInverse(inverseTransform);
      m_Resampler->SetTransform(transform);


      FloatVectorImageType::PointType ULpointTrans, URpointTrans, LRpointTrans, LLpointTrans;
      ULpointTrans=inverseTransform->TransformPoint(ULpoint);
      URpointTrans=inverseTransform->TransformPoint(URpoint);
      LRpointTrans=inverseTransform->TransformPoint(LRpoint);
      LLpointTrans=inverseTransform->TransformPoint(LLpoint);

      //compute min and max
      std::vector<FloatVectorImageType::PointType>   voutput;
      voutput.push_back(ULpointTrans);
      voutput.push_back(URpointTrans);
      voutput.push_back(LRpointTrans);
      voutput.push_back(LLpointTrans);

      double minX = voutput[0][0];
      double maxX = voutput[0][0];
      double minY = voutput[0][1];
      double maxY = voutput[0][1];

      for(unsigned int i = 1; i<voutput.size(); i++)
        {
        // Origins
        if ( minX > voutput[i][0] )
          {
          minX = voutput[i][0];
          }
        if ( minY > voutput[i][1] )
          {
          minY = voutput[i][1];
          }

        // Sizes
        if ( maxX < voutput[i][0] )
          {
          maxX = voutput[i][0];
          }
        if ( maxY < voutput[i][1] )
          {
          maxY = voutput[i][1];
          }
        }

      FloatVectorImageType::PointType outputOrig;
      if( spacing[0] > 0 ) outputOrig[0] = minX;
      else outputOrig[0] = maxX;

      if( spacing[1] > 0 ) outputOrig[1] = minY;
      else outputOrig[1] = maxY;

      //size of output image
      FloatVectorImageType::PointType size;
      size[0]=vcl_abs(maxX-minX);
      size[1]=vcl_abs(maxY-minY);

      // Evaluate spacing
      FloatVectorImageType::SpacingType OutputSpacing;
      OutputSpacing[0] = scale[0] * spacing[0];
      OutputSpacing[1] = scale[1] * spacing[1];

      m_Resampler->SetOutputSpacing(OutputSpacing);

      outputOrig[0] += 0.5 * OutputSpacing[0];
      outputOrig[1] += 0.5 * OutputSpacing[1];
      m_Resampler->SetOutputOrigin(outputOrig);

      // Evaluate size
      ResampleFilterType::SizeType recomputedSize;
      recomputedSize[0] = static_cast<unsigned int>(vcl_floor(vcl_abs(size[0]/OutputSpacing[0])));
      recomputedSize[1] = static_cast<unsigned int>(vcl_floor(vcl_abs(size[1]/OutputSpacing[1])));
      m_Resampler->SetOutputSize( recomputedSize );
      otbAppLogINFO( << "Output image size : " << recomputedSize );
      }
      break;
      }

    FloatVectorImageType::PixelType defaultValue;
    itk::NumericTraits<FloatVectorImageType::PixelType>::SetLength(defaultValue, inputImage->GetNumberOfComponentsPerPixel());
    m_Resampler->SetEdgePaddingValue(defaultValue);

    m_Resampler->UpdateOutputInformation();
    // Output Image
    SetParameterOutputImage("out", m_Resampler->GetOutput());
  }

  ResampleFilterType::Pointer m_Resampler;
}; //class


} //namespace wrapper
} //namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::RigidTransformResample)
