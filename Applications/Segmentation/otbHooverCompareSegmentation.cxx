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

#include "otbHooverMatrixFilter.h"
#include "otbHooverInstanceFilter.h"
#include "otbLabelMapToAttributeImageFilter.h"

#include "itkLabelImageToLabelMapFilter.h"
#include "otbUnaryFunctorImageFilter.h"

namespace otb
{

namespace Functor
{
// Functor to color Hoover instances
template<class TInput, class TOutput>
class HooverColorMapping
{
public:
  HooverColorMapping() {}
  virtual ~HooverColorMapping() {}

  typedef std::vector<TOutput> ColorListType;

  unsigned int GetOutputSize()
  {
    return 3;
  }
  void AddColor(const TOutput& color)
  {
    m_ScoreColors.push_back(color);
  }

  void SetBackground(const TOutput& bg)
  {
    m_Background = bg;
  }

  inline TOutput operator ()(const TInput& A)
  {
    TOutput out;
    out.SetSize(3);

    typename TInput::ValueType max = 0.0;
    unsigned int index=0;
    for (unsigned int i=0; i<m_ScoreColors.size(); i++)
      {
      if (A[i] > max)
        {
        index = i;
        max = A[i];
        }
      }
    if (max > 0.01)
      {
      out = m_ScoreColors[index];
      }
    else
      {
      out = m_Background;
      }
    return out;
  }

private:
  ColorListType m_ScoreColors;
  TOutput       m_Background;
};

} // end namespace Functor

namespace Wrapper
{


class HooverCompareSegmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef HooverCompareSegmentation         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(HooverCompareSegmentation, otb::Application);

  typedef otb::AttributesMapLabelObject<unsigned int, 2, float> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>            LabelMapType;
  typedef otb::HooverMatrixFilter<LabelMapType>     HooverMatrixFilterType;
  typedef UInt32ImageType                           ImageType;
  typedef FloatVectorImageType::PixelType           FloatPixelType;
  typedef Int16VectorImageType::PixelType           Int16PixelType;
  //typedef otb::VectorImage<float, 2>                VectorImageType;
  typedef itk::LabelImageToLabelMapFilter
    <ImageType, LabelMapType>                       ImageToLabelMapFilterType;
  typedef otb::ImageFileReader<ImageType>           ImageReaderType;
  typedef HooverMatrixFilterType::MatrixType        MatrixType;

  typedef otb::HooverInstanceFilter<LabelMapType>   InstanceFilterType;
  typedef otb::LabelMapToAttributeImageFilter
      <LabelMapType, FloatVectorImageType>          AttributeImageFilterType;
  typedef otb::UnaryFunctorImageFilter
      <FloatVectorImageType,
       Int16VectorImageType,
       Functor::HooverColorMapping
        <FloatPixelType, Int16PixelType> >          HooverColorFilterType;

private:
  void DoInit()
  {
    SetName("HooverCompareSegmentation");
    SetDescription("Compare two segmentations with Hoover metrics");

    // Documentation
    SetDocName("Hoover compare segmentation");
    SetDocLongDescription("This application compares a machine segmentation (MS) with a partial "
                          "ground truth segmentation (GT). The Hoover metrics are used to estimate "
                          "scores for correct detection, over-segmentation, under-segmentation and "
                          "missed detection."
                          "\n The application can output the overall Hoover scores along with colored"
                          "images of the MS and GT segmentation showing the state of each region "
                          "(correct detection, over-segmentation, under-segmentation, missed)"
                          "\n The Hoover metrics are described in : Hoover et al., \"An experimental"
                          " comparison of range image segmentation algorithms\", IEEE PAMI vol. 18, no. 7, July 1996.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbHooverMatrixFilter, otbHooverInstanceFilter, otbLabelMapToAttributeImageFilter");

    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage,   "ingt",     "Input ground truth");
    SetParameterDescription( "ingt", "A partial ground truth segmentation image." );

    AddParameter(ParameterType_InputImage,   "inms",     "Input machine segmentation");
    SetParameterDescription( "inms", "A machine segmentation image." );

    AddParameter(ParameterType_Int, "bg", "Background label");
    SetParameterDescription("bg", "Label value of the background in the input segmentations");
    SetDefaultParameterInt("bg", 0);

    AddParameter(ParameterType_Float, "th", "Overlapping threshold");
    SetParameterDescription("th", "Overlapping threshold used to find Hoover instances.");
    SetDefaultParameterFloat("th", 0.75);

    AddParameter(ParameterType_OutputImage,  "outgt",    "Colored ground truth output");
    SetParameterDescription( "outgt", "The colored ground truth output image." );
    MandatoryOff("outgt");

    AddParameter(ParameterType_OutputImage,  "outms",    "Colored machine segmentation output");
    SetParameterDescription( "outms", "The colored machine segmentation output image." );
    MandatoryOff("outms");

    // TODO : add color settings ?

    AddParameter(ParameterType_Float, "rc", "Correct detection score");
    SetParameterDescription("rc", "Overall score for correct detection (RC)");
    SetParameterRole("rc", Role_Output);

    AddParameter(ParameterType_Float, "rf", "Over-segmentation score");
    SetParameterDescription("rf", "Overall score for over segmentation (RF)");
    SetParameterRole("rf", Role_Output);

    AddParameter(ParameterType_Float, "ra", "Under-segmentation score");
    SetParameterDescription("ra", "Overall score for under segmentation (RA)");
    SetParameterRole("ra", Role_Output);

    AddParameter(ParameterType_Float, "rm", "Missed detection score");
    SetParameterDescription("rm", "Overall score for missed detection (RM)");
    SetParameterRole("rm", Role_Output);

    // Doc example parameter settings
    SetDocExampleParameterValue("ingt", "maur_GT.tif");
    SetDocExampleParameterValue("inms", "maur_labelled.tif");
    SetDocExampleParameterValue("outgt", "maur_colored_GT.tif uint8");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    UInt32ImageType::Pointer inputGT = GetParameterUInt32Image("ingt");
    UInt32ImageType::Pointer inputMS = GetParameterUInt32Image("inms");

    m_GTFilter = ImageToLabelMapFilterType::New();
    m_GTFilter->SetInput(inputGT);
    m_GTFilter->SetBackgroundValue( GetParameterInt("bg") );

    m_MSFilter = ImageToLabelMapFilterType::New();
    m_MSFilter->SetInput(inputMS);
    m_MSFilter->SetBackgroundValue( GetParameterInt("bg") );

    m_HooverFilter = HooverMatrixFilterType::New();
    m_HooverFilter->SetGroundTruthLabelMap(m_GTFilter->GetOutput());
    m_HooverFilter->SetMachineSegmentationLabelMap(m_MSFilter->GetOutput());

    m_HooverFilter->Update();

    m_InstanceFilter = InstanceFilterType::New();
    m_InstanceFilter->SetGroundTruthLabelMap(m_GTFilter->GetOutput());
    m_InstanceFilter->SetMachineSegmentationLabelMap(m_MSFilter->GetOutput());
    m_InstanceFilter->SetThreshold( GetParameterFloat("th") );
    m_InstanceFilter->SetHooverMatrix( m_HooverFilter->GetHooverConfusionMatrix() );
    m_InstanceFilter->SetUseExtendedAttributes(false);

    m_AttributeImageGT = AttributeImageFilterType::New();
    m_AttributeImageGT->SetInput(m_InstanceFilter->GetOutputGroundTruthLabelMap());
    m_AttributeImageGT->SetAttributeForNthChannel(0, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RC));
    m_AttributeImageGT->SetAttributeForNthChannel(1, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RF));
    m_AttributeImageGT->SetAttributeForNthChannel(2, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RA));
    m_AttributeImageGT->SetAttributeForNthChannel(3, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RM));

    m_AttributeImageMS = AttributeImageFilterType::New();
    m_AttributeImageMS->SetInput(m_InstanceFilter->GetOutputMachineSegmentationLabelMap());
    m_AttributeImageMS->SetAttributeForNthChannel(0, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RC));
    m_AttributeImageMS->SetAttributeForNthChannel(1, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RF));
    m_AttributeImageMS->SetAttributeForNthChannel(2, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RA));
    //m_AttributeImageMS->SetAttributeForNthChannel(3, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RN);

    m_GTColorFilter = HooverColorFilterType::New();
    m_GTColorFilter->SetInput(m_AttributeImageGT->GetOutput());

    m_MSColorFilter = HooverColorFilterType::New();
    m_MSColorFilter->SetInput(m_AttributeImageMS->GetOutput());

    Int16PixelType colorPixel;
    colorPixel.SetSize(3);
    // Background : white
    colorPixel[0] = 255;
    colorPixel[1] = 255;
    colorPixel[2] = 255;
    m_GTColorFilter->GetFunctor().SetBackground(colorPixel);
    m_MSColorFilter->GetFunctor().SetBackground(colorPixel);
    // Correct detection : green
    colorPixel[0] = 0;
    colorPixel[1] = 255;
    colorPixel[2] = 0;
    m_GTColorFilter->GetFunctor().AddColor(colorPixel);
    m_MSColorFilter->GetFunctor().AddColor(colorPixel);
    // Over-segmentation : magenta
    colorPixel[0] = 255;
    colorPixel[1] = 0;
    colorPixel[2] = 255;
    m_GTColorFilter->GetFunctor().AddColor(colorPixel);
    m_MSColorFilter->GetFunctor().AddColor(colorPixel);
    // Under-segmentation : cyan
    colorPixel[0] = 0;
    colorPixel[1] = 255;
    colorPixel[2] = 255;
    m_GTColorFilter->GetFunctor().AddColor(colorPixel);
    m_MSColorFilter->GetFunctor().AddColor(colorPixel);
    // Missed detection (only for GT) : red
    colorPixel[0] = 255;
    colorPixel[1] = 0;
    colorPixel[2] = 0;
    m_GTColorFilter->GetFunctor().AddColor(colorPixel);

    if (HasValue("outgt"))
      {
      SetParameterOutputImage("outgt", m_GTColorFilter->GetOutput());
      }

    if (HasValue("outms"))
      {
      SetParameterOutputImage("outms", m_MSColorFilter->GetOutput());
      }

    m_InstanceFilter->Update();

    SetParameterFloat("rc", m_InstanceFilter->GetMeanRC());
    SetParameterFloat("rf", m_InstanceFilter->GetMeanRF());
    SetParameterFloat("ra", m_InstanceFilter->GetMeanRA());
    SetParameterFloat("rm", m_InstanceFilter->GetMeanRM());
  }

  ImageToLabelMapFilterType::Pointer m_GTFilter;
  ImageToLabelMapFilterType::Pointer m_MSFilter;

  HooverMatrixFilterType::Pointer m_HooverFilter;
  InstanceFilterType::Pointer m_InstanceFilter;

  AttributeImageFilterType::Pointer m_AttributeImageGT;
  AttributeImageFilterType::Pointer m_AttributeImageMS;

  HooverColorFilterType::Pointer m_GTColorFilter;
  HooverColorFilterType::Pointer m_MSColorFilter;
};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::HooverCompareSegmentation)
