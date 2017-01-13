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


#include <geos_c.h>
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

#include "itkTimeProbe.h"
#include "otbConvexOrConcaveClassificationFilter.h"
#include "otbMorphologicalProfilesSegmentationFilter.h"
#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.h"

namespace otb {
namespace Wrapper {

class MorphologicalProfilesAnalysis : public Application {
public:
/** Standard class typedefs. */
  typedef MorphologicalProfilesAnalysis Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef FloatVectorImageType::InternalPixelType InputPixelType;
  typedef float OutputPixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::Image<InputPixelType, 2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;
  typedef otb::Image<LabeledPixelType, 2> LabeledImageType;
  typedef otb::VectorImage<OutputPixelType, 2> TOutputVectorImage;

  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, InputPixelType>
          ExtractorFilterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, 2> BallStructuringElementType;
  typedef itk::BinaryCrossStructuringElement<InputPixelType, 2> CrossStructuringElementType;

/** Standard macro */
  itkNewMacro( Self );

  itkTypeMacro( MorphologicalProfilesAnalysis, otb::Application );

private:

  void DoInit() ITK_OVERRIDE
  {
    SetName( "MorphologicalProfilesAnalysis" );
    SetDescription( "Performs morphological profiles analysis on an input image channel" );

    // Documentation
    SetDocName( "Morphological Profiles Analysis" );
    SetDocLongDescription( "This application performs on a mono band image" );
    SetDocLimitations( "None" );
    SetDocAuthors( "OTB-Team" );

    AddDocTag( "MorphologicalProfilesAnalysis" );

    AddParameter( ParameterType_InputImage, "in", "Input Image" );
    SetParameterDescription( "in", "The input image to be classified." );

    // TODO comment
    AddParameter( ParameterType_OutputImage, "out", "Output Image" );
    SetParameterDescription( "out", "TODO" );


    AddParameter( ParameterType_Int, "channel", "Selected Channel" );
    SetParameterDescription( "channel", "The selected channel index for input image" );
    SetDefaultParameterInt( "channel", 1 );
    SetMinimumParameterIntValue( "channel", 1 );

    AddRAMParameter();

    // Strucring Element (Ball | Cross)
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

    AddParameter( ParameterType_Int, "step", "Radius step." );
    SetParameterDescription( "step", "Radius step along the profile (in pixels)" );
    SetDefaultParameterInt( "step", 1 );
    SetMinimumParameterIntValue( "step", 1 );



    // TODO Documentation
    AddParameter( ParameterType_Choice, "profile", "Classification" );
    SetParameterDescription( "profile", "" );
    AddChoice( "profile.opening", "opening" );
    AddChoice( "profile.closing", "closing" );
    AddChoice( "profile.derivatedopening", "derivatedopening" );
    AddChoice( "profile.derivatedclosing", "derivatedclosing" );
    AddChoice( "profile.openingcharacteristics", "openingcharacteristics" );
    AddChoice( "profile.closingcharacteristics", "closingcharacteristics" );
    AddChoice( "profile.classification", "classification" );

    AddParameter( ParameterType_Float, "profile.classification.sigma", "Sigma" );
    SetParameterDescription( "profile.classification.sigma", "Sigma" );
    SetDefaultParameterFloat( "profile.classification.sigma", 1 );
    SetMinimumParameterFloatValue( "profile.classification.sigma", 0 );


    // TODO Doc example parameter settings
    /*
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("out", "opened.tif");
    SetDocExampleParameterValue("channel", "1");
     SetDocExampleParameterValue("channel.leveling", "1");
    SetDocExampleParameterValue("sigma", "5");
     */
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {


    FloatVectorImageType::Pointer inImage = GetParameterImage( "in" );
    inImage->UpdateOutputInformation();
    int nBComp = inImage->GetNumberOfComponentsPerPixel();
    int selectedChannel = GetParameterInt( "channel" );

    if ( selectedChannel > nBComp )
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
    m_ExtractorFilter->UpdateOutputInformation();

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

    typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InputImageType, StructuringElementType>
            OpeningProfileFilterType;
    typedef otb::MorphologicalClosingProfileFilter<InputImageType, InputImageType, StructuringElementType>
            ClosingProfileFilterType;
    typedef otb::ProfileToProfileDerivativeFilter<InputImageType, InputImageType> DerivativeFilterType;

    typedef otb::MultiScaleConvexOrConcaveClassificationFilter<InputImageType, LabeledImageType>
            MultiScaleClassificationFilterType;
    typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<InputImageType, OutputImageType, LabeledImageType>
            MultiScaleCharacteristicsFilterType;

    // Instantiation
    typename OpeningProfileFilterType::Pointer oprofileFilter;
    typename ClosingProfileFilterType::Pointer cprofileFilter;
    typename DerivativeFilterType::Pointer oderivativeFilter;
    typename DerivativeFilterType::Pointer cderivativeFilter;
    typename MultiScaleCharacteristicsFilterType::Pointer omsCharFilter;
    typename MultiScaleCharacteristicsFilterType::Pointer cmsCharFilter;
    typename MultiScaleClassificationFilterType::Pointer classificationFilter;

    bool classify = profile == "classification";
    bool opening = profile == "opening";
    bool closing = profile == "closing";
    bool characOpening = profile == "openingcharacteristics";
    bool characClosing = profile == "closingcharacteristics";
    bool derivatedOpening = profile == "derivatedopening";
    bool derivatedClosing = profile == "derivatedclosing";

    bool doOpening = classify || opening || derivatedOpening || characOpening;
    bool doClosing = classify || closing || derivatedClosing || characClosing;

    if ( doOpening )
      {
      performOperations<OpeningProfileFilterType, DerivativeFilterType, MultiScaleCharacteristicsFilterType>(
              oprofileFilter, oderivativeFilter, omsCharFilter,
              opening, derivatedOpening, characOpening,
              profileSize, step, initValue );
      if ( !classify )
        return;
      }

    if ( doClosing )
      {
      performOperations<ClosingProfileFilterType, DerivativeFilterType, MultiScaleCharacteristicsFilterType>(
              cprofileFilter, cderivativeFilter, cmsCharFilter,
              closing, derivatedClosing, characClosing,
              profileSize, step, initValue );
      if ( !classify )
        return;
      }

    classificationFilter = MultiScaleClassificationFilterType::New();
    classificationFilter->SetOpeningProfileDerivativeMaxima( omsCharFilter->GetOutput() );
    classificationFilter->SetOpeningProfileCharacteristics( omsCharFilter->GetOutputCharacteristics() );
    classificationFilter->SetClosingProfileDerivativeMaxima( cmsCharFilter->GetOutput() );
    classificationFilter->SetClosingProfileCharacteristics( cmsCharFilter->GetOutputCharacteristics() );
    classificationFilter->SetSigma( sigma );
    classificationFilter->SetLabelSeparator( static_cast<unsigned short>(initValue + profileSize * step) );
    classificationFilter->Update();
    SetParameterOutputImage( "out", classificationFilter->GetOutput() );
  }


  template<typename TProfileFilter, typename TDerivativeFilter, typename TCharacteristicsFilter>
  void
  performOperations(typename TProfileFilter::Pointer &profileFilter,
                    typename TDerivativeFilter::Pointer &derivativeFilter,
                    typename TCharacteristicsFilter::Pointer &msCharFilter,
                    bool profile, bool derivated, bool characteristics,
                    unsigned int profileSize, unsigned short initValue, unsigned short step) {

    typedef ImageList<InputImageType> TImageList;
    typedef otb::ImageListToVectorImageFilter<TImageList, TOutputVectorImage> TListToVectorImageFilter;

    profileFilter = TProfileFilter::New();
    profileFilter->SetInput( m_ExtractorFilter->GetOutput() );
    profileFilter->SetProfileSize( profileSize );
    profileFilter->SetInitialValue( initValue );
    profileFilter->SetStep( step );

    if ( profile )
      {
      TListToVectorImageFilter::Pointer listToVectorImageFilter = TListToVectorImageFilter::New();
      listToVectorImageFilter->SetInput( profileFilter->GetOutput() );
      listToVectorImageFilter->Update();
      SetParameterOutputImage( "out", listToVectorImageFilter->GetOutput() );
      return;
      }

    derivativeFilter = TDerivativeFilter::New();
    derivativeFilter->SetInput( profileFilter->GetOutput() );

    if ( derivated )
      {
      TListToVectorImageFilter::Pointer listToVectorImageFilter = TListToVectorImageFilter::New();
      listToVectorImageFilter->SetInput( derivativeFilter->GetOutput() );
      listToVectorImageFilter->Update();
      SetParameterOutputImage( "out", listToVectorImageFilter->GetOutput() );
      return;
      }

    msCharFilter = TCharacteristicsFilter::New();
    msCharFilter->SetInput( derivativeFilter->GetOutput() );
    msCharFilter->SetInitialValue( initValue );
    msCharFilter->SetStep( step );

    if ( characteristics )
      {
      msCharFilter->Update();
      SetParameterOutputImage( "out", msCharFilter->GetOutput() );
      }
  }

  ExtractorFilterType::Pointer m_ExtractorFilter;

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::MorphologicalProfilesAnalysis )

