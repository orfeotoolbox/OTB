/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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



#include <itkComposeImageFilter.h>
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryCrossStructuringElement.h"

#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "otbConvexOrConcaveClassificationFilter.h"
#include "otbMorphologicalProfilesSegmentationFilter.h"
#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.h"

namespace otb
{
namespace Wrapper
{

class MorphologicalProfilesAnalysis : public Application
{
public:
/** Standard class typedefs. */
  typedef MorphologicalProfilesAnalysis Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef FloatVectorImageType::InternalPixelType InputPixelType;

  typedef unsigned short LabeledPixelType;
  typedef otb::Image<LabeledPixelType, 2> LabeledImageType;

  typedef otb::MultiToMonoChannelExtractROI<InputPixelType, InputPixelType> ExtractorFilterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, 2> BallStructuringElementType;
  typedef itk::BinaryCrossStructuringElement<InputPixelType, 2> CrossStructuringElementType;

/** Standard macro */
  itkNewMacro( Self );

  itkTypeMacro( MorphologicalProfilesAnalysis, otb::Application );

private:

  void DoInit() override
  {
    SetName( "MorphologicalProfilesAnalysis" );
    SetDescription( "Performs morphological profiles analysis on an input image channel." );

    // Documentation
    SetDocName( "Morphological Profiles Analysis" );
    SetDocLongDescription( "This algorithm is derived from the following publication:\n\n"

                                   "Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE: A new approach\n"
                                   "for the morphological segmentation of high resolution satellite imagery.\n"
                                   "IEEE Transactions on geoscience and remote sensing, vol. 39, NO. 2,\n"
                                   "February 2001, p. 309-320.\n\n"

                                   "Depending of the profile selection, the application provides:\n\n"

                                   "* The multi scale geodesic morphological opening or closing profile of the input image.\n"
                                   "* The multi scale derivative of the opening or closing profile.\n"
                                   "* The parameter (called characteristic) of the maximum derivative value of the multi scale closing or opening profile for which this maxima occurs.\n"
                                   "* The labeled classification of the input image.\n\n"

                                   "The behavior of the classification is:\n\n"

                                   "Given :math:`x_1` and :math:`x_2` two membership values,\n"
                                   ":math:`L_1, L_2` two labels associated, and :math:`\\sigma` a tolerance\n"
                                   "value, the following decision rule is applied:\n"
                                   "\n"
                                   ":math:`L = \\begin{cases}  L_{1} & : x_{1}>x_{2} \\quad and \\quad x_{1}>\\sigma \\\\ L_{2} & : x_{2}>x_{1} \\quad and \\quad x_{2}>\\sigma \\\\ 0   & : otherwise. \\end{cases}` \n"
                                   "\n"
                                   "The output image can be:"
                                   "- A :math:`N` multi band image for the opening/closing normal or derivative profiles.\n"
                                   "- A mono band image for the opening/closing characteristics.\n"
                                   "- A labeled image for the classification." );
    SetDocLimitations( "Generation of the morphological profile is not streamable, pay attention to this fact when setting the radius initial size and step of the structuring element." );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso( "otbMorphologicalOpeningProfileFilter, otbMorphologicalClosingProfileFilter, otbProfileToProfileDerivativeFilter, otbProfileDerivativeToMultiScaleCharacteristicsFilter, otbMultiScaleConvexOrConcaveClassificationFilter, classes" );

    AddDocTag(Tags::FeatureExtraction);
    AddDocTag("Morphology");

    AddParameter( ParameterType_InputImage, "in", "Input Image" );
    SetParameterDescription( "in", "The input image." );

    AddParameter( ParameterType_OutputImage, "out", "Output Image" );
    SetParameterDescription( "out", "The output image." );

    AddParameter( ParameterType_Int, "channel", "Selected Channel" );
    SetParameterDescription( "channel", "The selected channel index for input image" );
    SetDefaultParameterInt( "channel", 1 );
    SetMinimumParameterIntValue( "channel", 1 );

    // Structuring Element (Ball | Cross)
    AddParameter( ParameterType_Choice, "structype", "Structuring Element Type" );
    SetParameterDescription( "structype", "Choice of the structuring element type" );
    AddChoice( "structype.ball", "Ball" );
    AddChoice( "structype.cross", "Cross" );

    AddParameter( ParameterType_Int, "size", "Profile Size" );
    SetParameterDescription( "size", "Size of the profiles" );
    SetDefaultParameterInt( "size", 5 );
    SetMinimumParameterIntValue( "size", 2 );

    AddParameter( ParameterType_Int, "radius", "Initial radius" );
    SetParameterDescription( "radius", "Initial radius of the structuring element (in pixels)" );
    SetDefaultParameterInt( "radius", 5 );
    SetMinimumParameterIntValue( "radius", 1 );

    AddParameter( ParameterType_Int, "step", "Radius step" );
    SetParameterDescription( "step", "Radius step along the profile (in pixels)" );
    SetDefaultParameterInt( "step", 1 );
    SetMinimumParameterIntValue( "step", 1 );


    AddParameter( ParameterType_Choice, "profile", "Profile" );
    SetParameterDescription( "profile", "" );
    AddChoice( "profile.opening", "opening" );
    AddChoice( "profile.closing", "closing" );
    AddChoice( "profile.derivativeopening", "derivativeopening" );
    AddChoice( "profile.derivativeclosing", "derivativeclosing" );
    AddChoice( "profile.openingcharacteristics", "openingcharacteristics" );
    AddChoice( "profile.closingcharacteristics", "closingcharacteristics" );
    AddChoice( "profile.classification", "classification" );

    AddParameter( ParameterType_Float, "profile.classification.sigma", "Sigma value for leveling tolerance" );
    SetParameterDescription( "profile.classification.sigma", "Sigma value for leveling tolerance" );
    SetDefaultParameterFloat( "profile.classification.sigma", 1 );
    SetMinimumParameterFloatValue( "profile.classification.sigma", 0 );

    AddRAMParameter();

    SetDocExampleParameterValue( "in", "ROI_IKO_PAN_LesHalles.tif" );
    SetDocExampleParameterValue( "channel", "1" );
    SetDocExampleParameterValue( "structype", "ball" );
    SetDocExampleParameterValue( "profile", "classification" );
    SetDocExampleParameterValue( "size", "5" );
    SetDocExampleParameterValue( "radius", "1" );
    SetDocExampleParameterValue( "step", "1" );
    SetDocExampleParameterValue( "profile.classification.sigma", "1" );
    SetDocExampleParameterValue( "out", "output.tif" );

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here: all parameters are independent
  }

  void DoExecute() override
  {

    FloatVectorImageType::Pointer inImage = GetParameterImage( "in" );

    int nBComp = inImage->GetNumberOfComponentsPerPixel();
    int selectedChannel = GetParameterInt( "channel" );

    if( selectedChannel > nBComp )
      {
      itkExceptionMacro( << "The specified channel index for input image is invalid." );
      }

    m_ExtractorFilter = ExtractorFilterType::New();
    m_ExtractorFilter->SetInput( inImage );
    m_ExtractorFilter->SetStartX( static_cast<unsigned int>(inImage->GetLargestPossibleRegion().GetIndex( 0 )) );
    m_ExtractorFilter->SetStartY( static_cast<unsigned int>(inImage->GetLargestPossibleRegion().GetIndex( 1 )) );
    m_ExtractorFilter->SetSizeX( inImage->GetLargestPossibleRegion().GetSize( 0 ) );
    m_ExtractorFilter->SetSizeY( inImage->GetLargestPossibleRegion().GetSize( 1 ) );
    m_ExtractorFilter->SetChannel( static_cast<unsigned int>(GetParameterInt( "channel" )) );

    unsigned int profileSize = static_cast<unsigned int>(GetParameterInt( "size" ));
    unsigned short initValue = static_cast<unsigned short>(GetParameterInt( "radius" ));
    unsigned short step = static_cast<unsigned short>(GetParameterInt( "step" ));
    float sigma = GetParameterFloat( "profile.classification.sigma" );
    std::string profile = GetParameterString( "profile" );


    if ( GetParameterString( "structype" ) == "ball" )
      {
      performProfileAnalysis<BallStructuringElementType>( profile, profileSize, initValue, step, sigma );
      }
    else // Cross
      {
      performProfileAnalysis<CrossStructuringElementType>( profile, profileSize, initValue, step, sigma );
      }
  }

  template<typename StructuringElementType>
  void
  performProfileAnalysis(std::string profile, unsigned int profileSize, unsigned short initValue,
                         unsigned short step, float sigma) {

    typedef otb::MorphologicalOpeningProfileFilter<FloatImageType, FloatImageType, StructuringElementType> OpeningProfileFilterType;
    typedef otb::MorphologicalClosingProfileFilter<FloatImageType, FloatImageType, StructuringElementType> ClosingProfileFilterType;
    typedef otb::ProfileToProfileDerivativeFilter<FloatImageType, FloatImageType> DerivativeFilterType;

    typedef otb::MultiScaleConvexOrConcaveClassificationFilter<FloatImageType, LabeledImageType> MultiScaleClassificationFilterType;
    typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<FloatImageType, FloatImageType, LabeledImageType> MultiScaleCharacteristicsFilterType;

    // Instantiation
    typename OpeningProfileFilterType::Pointer oprofileFilter = OpeningProfileFilterType::New();
    typename ClosingProfileFilterType::Pointer cprofileFilter = ClosingProfileFilterType::New();
    typename DerivativeFilterType::Pointer oderivativeFilter = DerivativeFilterType::New();
    typename DerivativeFilterType::Pointer cderivativeFilter = DerivativeFilterType::New();
    typename MultiScaleCharacteristicsFilterType::Pointer omsCharFilter = MultiScaleCharacteristicsFilterType::New();
    typename MultiScaleCharacteristicsFilterType::Pointer cmsCharFilter = MultiScaleCharacteristicsFilterType::New();
    typename MultiScaleClassificationFilterType::Pointer classificationFilter = MultiScaleClassificationFilterType::New();

    bool classify = profile == "classification";
    bool opening = profile == "opening";
    bool closing = profile == "closing";
    bool characOpening = profile == "openingcharacteristics";
    bool characClosing = profile == "closingcharacteristics";
    bool derivativeOpening = profile == "derivativeopening";
    bool derivativeClosing = profile == "derivativeclosing";

    bool doOpening = classify || opening || derivativeOpening || characOpening;
    bool doClosing = classify || closing || derivativeClosing || characClosing;

    if ( doOpening )
      {
      performOperations<OpeningProfileFilterType, DerivativeFilterType, MultiScaleCharacteristicsFilterType>(
              oprofileFilter, oderivativeFilter, omsCharFilter,
              opening, derivativeOpening, characOpening,
              profileSize, step, initValue );
      if ( !classify )
        return;
      }

    if ( doClosing )
      {
      performOperations<ClosingProfileFilterType, DerivativeFilterType, MultiScaleCharacteristicsFilterType>(
              cprofileFilter, cderivativeFilter, cmsCharFilter,
              closing, derivativeClosing, characClosing,
              profileSize, step, initValue );
      if ( !classify )
        return;
      }

    classificationFilter = MultiScaleClassificationFilterType::New();
    using namespace Functor::MultiScaleConvexOrConcaveDecisionRule_tags;
    classificationFilter->SetInput<max_opening_profile_derivative>( omsCharFilter->GetOutput() );
    classificationFilter->SetInput<opening_profile_characteristics>( omsCharFilter->GetOutputCharacteristics() );
    classificationFilter->SetInput<max_closing_profile_derivative>( cmsCharFilter->GetOutput() );
    classificationFilter->SetInput<closing_profile_characteristics>( cmsCharFilter->GetOutputCharacteristics() );
    classificationFilter->GetModifiableFunctor().SetSigma( sigma );
    classificationFilter->GetModifiableFunctor().SetLabelSeparator( static_cast<unsigned short>(initValue + profileSize * step) );
    AddProcess(classificationFilter, "Classification");
    classificationFilter->Update();
    SetParameterOutputImage( "out", classificationFilter->GetOutput() );
  }


  template<typename TProfileFilter, typename TDerivativeFilter, typename TCharacteristicsFilter>
  void
  performOperations(typename TProfileFilter::Pointer &profileFilter,
                    typename TDerivativeFilter::Pointer &derivativeFilter,
                    typename TCharacteristicsFilter::Pointer &msCharFilter,
                    bool profile, bool derivative, bool characteristics,
                    unsigned int profileSize, unsigned short initValue, unsigned short step) {

    typedef ImageList<FloatImageType> TImageList;
    typedef otb::ImageListToVectorImageFilter<TImageList, FloatVectorImageType> TListToVectorImageFilter;

    profileFilter->SetInput( m_ExtractorFilter->GetOutput() );
    profileFilter->SetProfileSize( profileSize );
    profileFilter->SetInitialValue( initValue );
    profileFilter->SetStep( step );

    if ( profile )
      {
      TListToVectorImageFilter::Pointer listToVectorImageFilter = TListToVectorImageFilter::New();
      listToVectorImageFilter->SetInput( profileFilter->GetOutput() );
      AddProcess(listToVectorImageFilter, "Profile");
      listToVectorImageFilter->Update();
      SetParameterOutputImage( "out", listToVectorImageFilter->GetOutput() );
      return;
      }

    derivativeFilter->SetInput( profileFilter->GetOutput() );

    if ( derivative )
      {
      TListToVectorImageFilter::Pointer listToVectorImageFilter = TListToVectorImageFilter::New();
      listToVectorImageFilter->SetInput( derivativeFilter->GetOutput() );
      AddProcess(listToVectorImageFilter, "Derivative");
      listToVectorImageFilter->Update();
      SetParameterOutputImage( "out", listToVectorImageFilter->GetOutput() );
      return;
      }

    msCharFilter->SetInput( derivativeFilter->GetOutput() );
    msCharFilter->SetInitialValue( initValue );
    msCharFilter->SetStep( step );

    if ( characteristics )
      {
      AddProcess(msCharFilter, "Characteristics");
      msCharFilter->Update();
      SetParameterOutputImage( "out", msCharFilter->GetOutputCharacteristics() );
      }
  }

  ExtractorFilterType::Pointer m_ExtractorFilter;

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::MorphologicalProfilesAnalysis )

