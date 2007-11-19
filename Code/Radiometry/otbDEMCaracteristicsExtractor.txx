/*=========================================================================
	
  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbDEMCaracteristicsExtractor.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "otbMath.h"

namespace otb
{
  
  template <class TInputImage, class TOutputImage>
  DEMCaracteristicsExtractor<TInputImage, TOutputImage>
  ::DEMCaracteristicsExtractor()
  {
    this->SetNumberOfInputs(1);
    this->SetNumberOfOutputs(3);
    
    this->SetNthOutput(0,OutputImageType::New());
    this->SetNthOutput(1,OutputImageType::New());
    this->SetNthOutput(2,OutputImageType::New());
    this->SetNthOutput(3,OutputImageType::New());

    m_GradientMagnitudeFilter         = GradientMagnitudeFilterType::New();
    m_GradientRecursiveGaussianFilter = GradientRecursiveGaussianImageFilterType::New(); 
    m_AspectFilter                    = Atan2FilterType::New(); 
    m_IncidenceFilter                 = AcosImageFilterType::New();
    m_ExitanceFilter                  = AcosImageFilterType::New();

    m_SolarRadius = 0;
    m_SolarAzimut = 0;
    m_ViewRadius = 0;
    m_ViewAzimut = 0;
  }
  
  
  template <class TInputImage, class TOutputImage>
  DEMCaracteristicsExtractor<TInputImage, TOutputImage>
  ::~DEMCaracteristicsExtractor()
  {
  }
  
  
  /**
   * ThreadedGenerateData Performs the pixel-wise addition
   */
  template <class TInputImage, class TOutputImage>
  void
  DEMCaracteristicsExtractor<TInputImage,TOutputImage>
  ::GenerateData()
  {
    // Input and output pointer definition
    typename InputImageType::Pointer  inputPtr  = const_cast<InputImageType *>(this->GetInput());
    typename OutputImageType::Pointer SlopOutputPtr = this->GetSlopOutput();
    typename OutputImageType::Pointer AspectOutputPtr = this->GetAspectOutput();
    typename OutputImageType::Pointer IncidenceOutputPtr = this->GetIncidenceOutput();
    typename OutputImageType::Pointer ExitanceOutputPtr = this->GetExitanceOutput();

    // Degrees To Radian _-> Radian To Degree coefficient
    double rad2degCoef;
    rad2degCoef = 180/M_PI;

    // Slop calculation 
    m_GradientMagnitudeFilter->SetInput(inputPtr);
    // // Transform values from radian to degrees.
    typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter = MultiplyByScalarImageFilterType::New();
    rad2DegFilter->SetInput( m_GradientMagnitudeFilter->GetOutput() );
    rad2DegFilter->SetCoef( rad2degCoef );
    rad2DegFilter->GraftOutput( SlopOutputPtr );
    rad2DegFilter->Update();
    this->GraftNthOutput( 0, rad2DegFilter->GetOutput() );

    // Aspect calcultation
    m_GradientRecursiveGaussianFilter->SetInput(inputPtr);
    m_GradientRecursiveGaussianFilter->Update();
    
    // // Extract the X and the Y gradient
    typename AdaptorType::Pointer XAdaptator = AdaptorType::New();
    typename AdaptorType::Pointer YAdaptator = AdaptorType::New();
    XAdaptator->SetImage(m_GradientRecursiveGaussianFilter->GetOutput());
    YAdaptator->SetImage(m_GradientRecursiveGaussianFilter->GetOutput());
    XAdaptator->SelectNthElement(0);
    YAdaptator->SelectNthElement(1);
    // // Compute Arctan
    m_AspectFilter->SetInput1(XAdaptator);
    m_AspectFilter->SetInput2(YAdaptator);
    // // Transform values from radian to degres.
    typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter1 = MultiplyByScalarImageFilterType::New();
    rad2DegFilter1->SetInput( m_AspectFilter->GetOutput() );
    rad2DegFilter1->SetCoef( rad2degCoef );
    rad2DegFilter1->GraftOutput( AspectOutputPtr );
    rad2DegFilter1->Update();
    this->GraftNthOutput( 1, rad2DegFilter1->GetOutput() );
    

    // Angle calculation :
    // sin(slop)
    typename SinImageFilterType::Pointer sinS = SinImageFilterType::New();
    sinS->SetInput( m_GradientMagnitudeFilter->GetOutput() );
    // cos (slop)
    typename CosImageFilterType::Pointer cosS = CosImageFilterType::New();
    cosS->SetInput( m_GradientMagnitudeFilter->GetOutput() );
    // -aspect
    typename MultiplyByScalarImageFilterType::Pointer oppositeFilter = MultiplyByScalarImageFilterType::New();
    oppositeFilter->SetInput( m_AspectFilter->GetOutput() );
    oppositeFilter->SetCoef( -1 );

    // Incidence calculation
    typename ShiftScaleImageFilterType::Pointer addAzimut = ShiftScaleImageFilterType::New();
    addAzimut->SetScale(1.);
    addAzimut->SetShift( m_SolarAzimut/rad2degCoef );
    addAzimut->SetInput( oppositeFilter->GetOutput()  );

    typename CosImageFilterType::Pointer cosAAzimut = CosImageFilterType::New();
    cosAAzimut->SetInput( addAzimut->GetOutput() );

    typename MultiplyByScalarImageFilterType::Pointer sinSsinSolarRadiusFilter = MultiplyByScalarImageFilterType::New();
    sinSsinSolarRadiusFilter->SetCoef( vcl_sin(m_SolarRadius/rad2degCoef ));
    sinSsinSolarRadiusFilter->SetInput( sinS->GetOutput() );

    typename MultiplyImageFilterType::Pointer cosAAzimuthsinSsinRadius =  MultiplyImageFilterType::New();
    cosAAzimuthsinSsinRadius->SetInput1( sinSsinSolarRadiusFilter->GetOutput() );
    cosAAzimuthsinSsinRadius->SetInput2( cosAAzimut->GetOutput() );

    typename MultiplyByScalarImageFilterType::Pointer cosScosSolarRadiusFilter = MultiplyByScalarImageFilterType::New();
    cosScosSolarRadiusFilter->SetCoef( vcl_cos(m_SolarRadius/rad2degCoef) );
    cosScosSolarRadiusFilter->SetInput( cosS->GetOutput() );

    typename AddImageFilterType::Pointer cosIncidence = AddImageFilterType::New();
    cosIncidence->SetInput1( cosAAzimuthsinSsinRadius->GetOutput() );
    cosIncidence->SetInput2( cosScosSolarRadiusFilter->GetOutput() );

    m_IncidenceFilter->SetInput( cosIncidence->GetOutput() );

    // // Change radians in degres
    typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter2 = MultiplyByScalarImageFilterType::New();
    rad2DegFilter2->SetInput( m_IncidenceFilter->GetOutput() );
    rad2DegFilter2->SetCoef( rad2degCoef );
    // // Link to the output
    rad2DegFilter2->GraftOutput( IncidenceOutputPtr );
    rad2DegFilter2->Update();
    this->GraftNthOutput( 2, rad2DegFilter2->GetOutput() );


    // Exitance calculation
    typename ShiftScaleImageFilterType::Pointer addAzimut2 = ShiftScaleImageFilterType::New();
    addAzimut2->SetScale(1.);
    addAzimut2->SetShift( m_ViewAzimut/rad2degCoef );
    addAzimut2->SetInput( oppositeFilter->GetOutput()  );

    typename CosImageFilterType::Pointer cosAAzimut2 = CosImageFilterType::New();
    cosAAzimut2->SetInput( addAzimut2->GetOutput() );

    typename MultiplyByScalarImageFilterType::Pointer sinSsinSolarRadiusFilter2 = MultiplyByScalarImageFilterType::New();
    sinSsinSolarRadiusFilter2->SetCoef( vcl_sin(m_ViewRadius/rad2degCoef) );
    sinSsinSolarRadiusFilter2->SetInput( sinS->GetOutput() );

    typename MultiplyImageFilterType::Pointer cosAAzimuthsinSsinRadius2 =  MultiplyImageFilterType::New();
    cosAAzimuthsinSsinRadius2->SetInput1( sinSsinSolarRadiusFilter2->GetOutput() );
    cosAAzimuthsinSsinRadius2->SetInput2( cosAAzimut2->GetOutput() );


    typename MultiplyByScalarImageFilterType::Pointer cosScosSolarRadiusFilter2 = MultiplyByScalarImageFilterType::New();
    cosScosSolarRadiusFilter2->SetCoef( vcl_cos(m_ViewRadius/rad2degCoef) );
    cosScosSolarRadiusFilter2->SetInput( cosS->GetOutput() );

    typename AddImageFilterType::Pointer cosIncidence2 = AddImageFilterType::New();
    cosIncidence2->SetInput1( cosAAzimuthsinSsinRadius2->GetOutput() );
    cosIncidence2->SetInput2( cosScosSolarRadiusFilter2->GetOutput() );

    m_ExitanceFilter->SetInput( cosIncidence2->GetOutput() );

   // // Change radians in degres
    typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter3 = MultiplyByScalarImageFilterType::New();
    rad2DegFilter3->SetInput( m_ExitanceFilter->GetOutput() );
    rad2DegFilter3->SetCoef( rad2degCoef );
    // // Link to the output
    rad2DegFilter3->GraftOutput( ExitanceOutputPtr );
    rad2DegFilter3->Update();
    this->GraftNthOutput( 3, rad2DegFilter3->GetOutput() );
    
    
  }
  
} // end namespace otb
