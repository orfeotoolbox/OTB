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

  typedef otb::Image<InputPixelType, 2>         InputImageType;
  typedef otb::VectorImage<OutputPixelType, 2>  TOutputVectorImage;

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
    SetDescription( "Performs morphological operations on an input image channel" );

    // Documentation
    SetDocName( "Morphological Multi Scale Decomposition" );
    SetDocLongDescription("This application performs on a mono band image" );
    SetDocLimitations( "None" );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso( "otbGeodesicMorphologyDecompositionImageFilter class" );


    AddDocTag( "MorphologicalMultiScaleDecomposition" );

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

    AddParameter( ParameterType_Int, "radius", "Radius of the kernel" );
    SetParameterDescription( "radius", "Radius of the kernel" );
    SetDefaultParameterInt( "radius", 5 );
    SetMinimumParameterIntValue( "radius", 1 );

    AddParameter( ParameterType_Int, "channel", "Selected Channel" );
    SetParameterDescription( "channel", "The selected channel index for input image" );
    SetDefaultParameterInt( "channel", 1 );
    SetMinimumParameterIntValue( "channel", 1 );

    AddParameter( ParameterType_Int, "step", "Selected Step" );
    SetParameterDescription( "step", "Increment radius by this step for each iteration" );
    SetDefaultParameterInt( "step", 1 );
    SetMinimumParameterIntValue( "step", 1 );

    AddParameter( ParameterType_Int, "levels", "Number of levels use for multiscale" );
    SetParameterDescription( "levels", "Number of levels use for multiscale" );
    SetDefaultParameterInt( "levels", 1 );
    SetMinimumParameterIntValue( "levels", 1 );


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


    unsigned int numberOfLevels = static_cast<unsigned int>(GetParameterInt( "levels" ));
    unsigned int initValue = static_cast<unsigned int>(GetParameterInt( "radius" ));
    unsigned int step = static_cast<unsigned int>(GetParameterInt( "step" ));


    if(GetParameterString("structype") == "ball")
      {
      performDecomposition<BallStructuringElementType>(numberOfLevels, step, initValue);
      }
    else // Cross
      {
      performDecomposition<CrossStructuringElementType>(numberOfLevels, step, initValue);
      }
  }

  template<typename StructuringElement>
  void performDecomposition(unsigned int numberOfLevels, unsigned int step, unsigned int initValue)
  {

    typedef otb::GeodesicMorphologyIterativeDecompositionImageFilter<InputImageType, StructuringElement> TDecompositionImageFilter;
    typedef typename TDecompositionImageFilter::OutputImageListType TImageList;
    typedef typename TImageList::Iterator TImageListIterator;
    typedef otb::ImageListToVectorImageFilter<TImageList, TOutputVectorImage> TListToVectorImageFilter;

    typename TDecompositionImageFilter::Pointer decompositionImageFilter;
    decompositionImageFilter = TDecompositionImageFilter::New();
    decompositionImageFilter->SetInput( m_ExtractorFilter->GetOutput() );
    decompositionImageFilter->SetNumberOfIterations( numberOfLevels );
    decompositionImageFilter->SetInitialValue( initValue );
    decompositionImageFilter->SetStep( step );
    decompositionImageFilter->Update();

    // Retrieving iterators on the results images
    TImageListIterator itAnalyse = decompositionImageFilter->GetOutput()->Begin();
    TImageListIterator itConvexMap = decompositionImageFilter->GetConvexOutput()->Begin();
    TImageListIterator itConcaveMap = decompositionImageFilter->GetConcaveOutput()->Begin();
    typename TImageList::Pointer imageList = TImageList::New();

    // Compose the results images
    while ( (itAnalyse != decompositionImageFilter->GetOutput()->End())
            && (itConvexMap != decompositionImageFilter->GetConvexOutput()->End())
            && (itConcaveMap != decompositionImageFilter->GetConcaveOutput()->End())
            )
      {
      imageList->PushBack(itAnalyse.Get());
      imageList->PushBack(itConvexMap.Get());
      imageList->PushBack(itConcaveMap.Get());

      ++itAnalyse;
      ++itConvexMap;
      ++itConcaveMap;
      }

    typename TListToVectorImageFilter::Pointer listToVectorImageFilter = TListToVectorImageFilter::New();
    listToVectorImageFilter->SetInput(imageList);
    listToVectorImageFilter->Update();
    SetParameterOutputImage("out", listToVectorImageFilter->GetOutput());
  }

  ExtractorFilterType::Pointer m_ExtractorFilter;

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::MorphologicalMultiScaleDecomposition )

