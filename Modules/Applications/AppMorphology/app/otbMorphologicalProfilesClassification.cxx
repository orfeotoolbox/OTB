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
#include "otbGeodesicMorphologyDecompositionImageFilter.h"
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

namespace otb {
namespace Wrapper {

class MorphologicalProfilesClassification : public Application {
public:
/** Standard class typedefs. */
  typedef MorphologicalProfilesClassification Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef FloatVectorImageType::InternalPixelType InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType, 2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;

  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, InputPixelType>
          ExtractorFilterType;

  typedef otb::ConvexOrConcaveClassificationFilter<InputImageType, OutputImageType> ClassificationFilterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, 2> BallStructuringElementType;
  typedef itk::BinaryCrossStructuringElement<InputPixelType, 2> CrossStructuringElementType;

/** Standard macro */
  itkNewMacro( Self );

  itkTypeMacro( MorphologicalProfilesClassification, otb::Application );

private:

  void DoInit() ITK_OVERRIDE
  {
    SetName( "MorphologicalProfilesClassification" );
    SetDescription( "Performs morphological concave or convex classification on an input image channel" );

    // Documentation
    SetDocName( "Morphological Multi Scale Decomposition" );
    SetDocLongDescription( "This application performs on a mono band image" );
    SetDocLimitations( "None" );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso( "otbConvexOrConcaveClassificationFilter class" );

    AddDocTag( "MorphologicalProfilesClassification" );

    AddParameter( ParameterType_InputImage, "in", "Input Image" );
    SetParameterDescription( "in", "The input image to be classified." );

    AddParameter( ParameterType_OutputImage, "out", "Output Image" );
    SetParameterDescription( "out",
                             "The output classified image with 3 different values (0 : Flat, 1 : Convex, 2 : Concave)" );

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

    AddParameter( ParameterType_Int, "radius", "Radius of the kernel" );
    SetParameterDescription( "radius", "Radius of the kernel" );
    SetDefaultParameterInt( "radius", 5 );
    SetMinimumParameterIntValue( "radius", 1 );

    AddParameter( ParameterType_Float, "sigma", "Sigma value for leveling tolerance" );
    SetParameterDescription( "sigma", "Sigma value for leveling tolerance" );
    SetDefaultParameterFloat( "sigma", 0.5 );
    SetMinimumParameterFloatValue( "sigma", 0 );


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
    m_ExtractorFilter->SetChannel( static_cast<unsigned int>(selectedChannel) );
    m_ExtractorFilter->UpdateOutputInformation();

    unsigned int sigma = static_cast<unsigned int>(GetParameterInt( "sigma" ));
    unsigned int radius = static_cast<unsigned int>(GetParameterInt( "radius" ));

    m_ClassificationFilter = ClassificationFilterType::New();
    m_ClassificationFilter->SetInput( m_ExtractorFilter->GetOutput() );
    m_ClassificationFilter->SetSigma( sigma );
    m_ClassificationFilter->SetFlatLabel( 0 );
    m_ClassificationFilter->SetConvexLabel( 1 );
    m_ClassificationFilter->SetConcaveLabel( 2 );


    if ( GetParameterString( "structype" ) == "ball" )
      {
      performClassification<BallStructuringElementType>( radius );
      }
    else // Cross
      {
      performClassification<CrossStructuringElementType>( radius );
      }


  }

  template<typename TStructuringElement>
  void performClassification(unsigned int radius_size) {

    typedef otb::GeodesicMorphologyDecompositionImageFilter<InputImageType, InputImageType, TStructuringElement> TDecompositionImageFilter;

    typename TDecompositionImageFilter::Pointer decompositionImageFilter;
    decompositionImageFilter = TDecompositionImageFilter::New();
    decompositionImageFilter->SetInput( m_ExtractorFilter->GetOutput() );

    typename TStructuringElement::RadiusType radius;
    radius.Fill(radius_size);
    decompositionImageFilter->SetRadius( radius );
    decompositionImageFilter->Update();

    m_ClassificationFilter->SetInputLeveling( decompositionImageFilter->GetOutput() );
    SetParameterOutputImage( "out", m_ClassificationFilter->GetOutput() );
  }

  ExtractorFilterType::Pointer m_ExtractorFilter;
  ClassificationFilterType::Pointer m_ClassificationFilter;

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::MorphologicalProfilesClassification )

