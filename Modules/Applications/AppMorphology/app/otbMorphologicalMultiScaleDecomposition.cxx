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

class MorphologicalMultiScaleDecomposition : public Application {
public:
/** Standard class typedefs. */
  typedef MorphologicalMultiScaleDecomposition Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef FloatVectorImageType::InternalPixelType InputPixelType;
  typedef float OutputPixelType;

  typedef otb::Image<InputPixelType, 2> InputImageType;
  typedef otb::VectorImage<OutputPixelType, 2> TOutputVectorImage;

  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, InputPixelType>
          ExtractorFilterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, 2> BallStructuringElementType;
  typedef itk::BinaryCrossStructuringElement<InputPixelType, 2> CrossStructuringElementType;

/** Standard macro */
  itkNewMacro( Self );

  itkTypeMacro( MorphologicalMultiScaleDecomposition, otb::Application );

private:

  void DoInit() ITK_OVERRIDE
  {
    SetName( "MorphologicalMultiScaleDecomposition" );
    SetDescription( "Perform a geodesic morphology based image analysis on an input image channel" );

    // Documentation
    SetDocName( "Morphological Multi Scale Decomposition" );
    SetDocLongDescription(
            "This application recursively apply geodesic decomposition. \n"
                    "\n"
                    "This algorithm is derived from the following publication:\n"
                    "\n"
                    "Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE: A new approach "
                    "for the morphological segmentation of high resolution satellite imagery.\n"
                    "IEEE Transactions on geoscience and remote sensing, vol. 39, NO. 2, "
                    "February 2001, p. 309-320.\n"
                    "\n"
                    "It provides a geodesic decomposition of the input image, with the "
                    "following scheme. Let :math:`f_0` denote the input image, :math:`\\stackrel{\\smile}{\\mu}_{N}(f)` denote the convex membership function, :math:`\\stackrel{\\frown}{\\mu}_{N}(f)` denote the concave membership function and :math:`\\psi_{N}(f)` denote the leveling function, for a given radius :math:`N` as defined in the documentation\n"
                    "of the GeodesicMorphologyDecompositionImageFilter. Let :math:`[N_{1},\\ldots, N_{n}]` denote a range of increasing radius (or scales). The iterative decomposition is defined as follows:\n"
                    "\n"
                    "[ f\\_n = `\\psi`\\_N\\_n(f\\_n-1) ]\n"
                    "\n"
                    "[ :math:`\\stackrel{\\frown}{f}_n` = :math:`\\stackrel{\\frown}{\\mu}_N_N(f_n)` ]\n"
                    "\n"
                    "[ :math:`\\stackrel{\\smile}{f}_n` = :math:`\\stackrel{\\smile}{\\mu}_N_N(f_n)` ]\n"
                    "\n"
                    "The :math:`\\stackrel{\\frown}{f}_{n}` and :math:`\\stackrel{\\frown}{f}_{n}` are membership function for the convex\n"
                    "(resp. concave) objects whose size is comprised between :math:`N_{n-1}` and :math:`N_n`\n"
                    "\n"
                    "Output convex, concave and leveling images with N bands, where N is the number of levels." );

    SetDocLimitations( "None" );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso( "otbGeodesicMorphologyDecompositionImageFilter class" );

    AddDocTag( "MorphologicalMultiScaleDecomposition" );

    AddParameter( ParameterType_InputImage, "in", "Input Image" );
    SetParameterDescription( "in", "The input image to be classified." );

    AddParameter( ParameterType_OutputImage, "outconvex", "Output Convex Image" );
    SetParameterDescription( "outconvex", "The output convex image with N bands" );
    AddParameter( ParameterType_OutputImage, "outconcave", "Output Concave Image" );
    SetParameterDescription( "outconcave", "The output concave concave with N bands" );
    AddParameter( ParameterType_OutputImage, "outleveling", "Output Image" );
    SetParameterDescription( "outleveling", "The output leveling image with N bands" );

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

    AddParameter( ParameterType_Int, "radius", "Initial radius" );
    SetParameterDescription( "radius", "Initial radius of the structuring element (in pixels)" );
    SetDefaultParameterInt( "radius", 5 );
    SetMinimumParameterIntValue( "radius", 1 );

    AddParameter( ParameterType_Int, "step", "Radius step." );
    SetParameterDescription( "step", "Radius step along the profile (in pixels)" );
    SetDefaultParameterInt( "step", 1 );
    SetMinimumParameterIntValue( "step", 1 );

    AddParameter( ParameterType_Int, "levels", "Number of levels use for multi scale" );
    SetParameterDescription( "levels", "Number of levels use for multi scale" );
    SetDefaultParameterInt( "levels", 1 );
    SetMinimumParameterIntValue( "levels", 1 );

    SetDocExampleParameterValue("in", "ROI_IKO_PAN_LesHalles.tif");
    SetDocExampleParameterValue("structype", "ball");
    SetDocExampleParameterValue("channel", "1");
    SetDocExampleParameterValue("radius", "2");
    SetDocExampleParameterValue("levels", "2");
    SetDocExampleParameterValue("step", "3");
    SetDocExampleParameterValue("outconvex", "convex.tif");
    SetDocExampleParameterValue("outconcave", "concave.tif");
    SetDocExampleParameterValue("outleveling", "leveling.tif");


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


    unsigned int numberOfLevels = static_cast<unsigned int>(GetParameterInt( "levels" ));
    unsigned int initValue = static_cast<unsigned int>(GetParameterInt( "radius" ));
    unsigned int step = static_cast<unsigned int>(GetParameterInt( "step" ));


    if ( GetParameterString( "structype" ) == "ball" )
      {
      performDecomposition<BallStructuringElementType>( numberOfLevels, step, initValue );
      } else // Cross
      {
      performDecomposition<CrossStructuringElementType>( numberOfLevels, step, initValue );
      }
  }

  template<typename StructuringElement>
  void performDecomposition(unsigned int numberOfLevels, unsigned int step, unsigned int initValue) {

    typedef otb::GeodesicMorphologyIterativeDecompositionImageFilter<InputImageType, StructuringElement> TDecompositionImageFilter;
    typedef typename TDecompositionImageFilter::OutputImageListType TImageList;
    typedef otb::ImageListToVectorImageFilter<TImageList, TOutputVectorImage> TListToVectorImageFilter;

    typename TDecompositionImageFilter::Pointer decompositionImageFilter;
    decompositionImageFilter = TDecompositionImageFilter::New();
    decompositionImageFilter->SetInput( m_ExtractorFilter->GetOutput() );
    decompositionImageFilter->SetNumberOfIterations( numberOfLevels );
    decompositionImageFilter->SetInitialValue( initValue );
    decompositionImageFilter->SetStep( step );
    decompositionImageFilter->Update();

    typename TListToVectorImageFilter::Pointer levelingListToVectorImageFilter = TListToVectorImageFilter::New();
    typename TListToVectorImageFilter::Pointer concaveListToVectorImageFilter = TListToVectorImageFilter::New();
    typename TListToVectorImageFilter::Pointer convexListToVectorImageFilter = TListToVectorImageFilter::New();

    levelingListToVectorImageFilter->SetInput( decompositionImageFilter->GetOutput() );
    levelingListToVectorImageFilter->Update();
    SetParameterOutputImage( "outleveling", levelingListToVectorImageFilter->GetOutput() );

    concaveListToVectorImageFilter->SetInput( decompositionImageFilter->GetConcaveOutput() );
    concaveListToVectorImageFilter->Update();
    SetParameterOutputImage( "outconcave", concaveListToVectorImageFilter->GetOutput() );

    convexListToVectorImageFilter->SetInput( decompositionImageFilter->GetConvexOutput() );
    convexListToVectorImageFilter->Update();
    SetParameterOutputImage( "outconvex", convexListToVectorImageFilter->GetOutput() );
  }

  ExtractorFilterType::Pointer m_ExtractorFilter;

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::MorphologicalMultiScaleDecomposition )

