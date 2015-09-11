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
#ifndef __otbSarRadiometricCalibrationToImageFilter_txx
#define __otbSarRadiometricCalibrationToImageFilter_txx

#include "otbSarRadiometricCalibrationToImageFilter.h"
#include "otbSarImageMetadataInterfaceFactory.h"
#include "otbSarCalibrationLookupData.h"

namespace otb
{

/**
 * Constructor
 */
template<class TInputImage, class TOutputImage>
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::SarRadiometricCalibrationToImageFilter()
: m_Initialized(false)
{
  this->SetNumberOfThreads( 1 );
}

/*This method is called once from BeforeThreadedGenerateData(). Usage of this
* outside or without a call to BeforeThreadedGenerateData() is supposed to
* fail. */
template<class TInputImage, class TOutputImage>
void
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::SetupCalibrator()
{

  /** cretate a SarImageMetadataInterface instance from
   * GetMetaDataDictionary(). This will return the appropriate IMI depending on
   * the Sensor information & co available in GetMetaDataDictionary()  */
  SarImageMetadataInterface::Pointer imageMetadataInterface = SarImageMetadataInterfaceFactory::CreateIMI(
      this->GetInput()->GetMetaDataDictionary());

  /** Get the SarRadiometricCalibrationFunction function instance.  */
  FunctionPointer function = this->GetFunction();

  /** check if there is a calibration lookupdata is available with the
* product. eg. Sentinel1. This means
* A. The computation of the backscatter is based on this lookup value( whose computation vary from sensor to sensor).*
* B. The other value such as antenna pattern gain, rangespread loss, incidence
* angle has no effect in calibration  */



  bool apply = imageMetadataInterface->HasCalibrationLookupData();
  /* Below lines will toggle the necessary flags which can help skip some
* computation. For example, if there is lookup value and ofcourse antenna
* pattern gain is not required. Even if we try to compute the value with
* SarParametricFuntion we  get 1. This is the safe side. But as we are so sure
* we skip all those calls to EvaluateParametricCoefficient and also the
* Evalute(). For the function the value is 1 by default.
 */
  function->SetApplyAntennaPatternGain(!apply);
  function->SetApplyIncidenceAngleCorrection(!apply);
  function->SetApplyRangeSpreadingLossCorrection(!apply);
  function->SetApplyRescalingFactor(!apply);
  function->SetApplyLookupDataCorrection(apply);

  function->SetScale(imageMetadataInterface->GetRadiometricCalibrationScale());

  /* Compute noise if enabled */
  if( function->GetEnableNoise())
    {
    ParametricFunctionPointer   noise;
    noise = function->GetNoise();
    noise->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationNoise());
    noise->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationNoisePolynomialDegree());
    noise->EvaluateParametricCoefficient();
    }

  /* Compute old and new antenna pattern gain */
  if(function->GetApplyAntennaPatternGain())
    {
    ParametricFunctionPointer   antennaPatternNewGain;
    antennaPatternNewGain = function->GetAntennaPatternNewGain();
    antennaPatternNewGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGain());
    antennaPatternNewGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree());
    antennaPatternNewGain->EvaluateParametricCoefficient();

    ParametricFunctionPointer   antennaPatternOldGain;
    antennaPatternOldGain = function->GetAntennaPatternOldGain();
    antennaPatternOldGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGain());
    antennaPatternOldGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree());
    antennaPatternOldGain->EvaluateParametricCoefficient();
    }

  /* Compute incidence angle */
  if (function->GetApplyIncidenceAngleCorrection())
    {
    ParametricFunctionPointer   incidenceAngle;
    incidenceAngle = function->GetIncidenceAngle();
    incidenceAngle->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationIncidenceAngle());
    incidenceAngle->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationIncidenceAnglePolynomialDegree());
    incidenceAngle->EvaluateParametricCoefficient();
    }

    /* Compute Range spread Loss */
  if (function->GetApplyRangeSpreadingLossCorrection())
    {
    ParametricFunctionPointer   rangeSpreadLoss;
    rangeSpreadLoss = function->GetRangeSpreadLoss();
    rangeSpreadLoss->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLoss());
    rangeSpreadLoss->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree());
    rangeSpreadLoss->EvaluateParametricCoefficient();
    }

  /** Get the lookupdata instance. unlike the all the above this is not a
* parametricFunction instance. But rather an internal class in IMI called
* SarCalibrationLookupData.
*
*NOTE: As the computation of lookup data for sensors is not universal. One must
*provide a sub-class.
See Also: otbSentinel1ImageMetadataInterface, otbTerraSarImageMetadataInterface,
*otbRadarsat2ImageMetadataInterface  */
  if (function->GetApplyLookupDataCorrection())
    {
    function->SetCalibrationLookupData(imageMetadataInterface->GetCalibrationLookupData("sigma"));
    }


  /** This was introduced for cosmoskymed which required a rescaling factor */
  if (function->GetApplyRescalingFactor())
    {
    function->SetRescalingFactor(imageMetadataInterface->GetRescalingFactor());
    }

  /** m_Initialized to true to prevent further calls    */
  m_Initialized = true;

}

template<class TInputImage, class TOutputImage>
void
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // will SetInputImage on the function
  Superclass::BeforeThreadedGenerateData();

  /** Call SetupCalibation on first call to BeforeThreadedGenerateData( ) */
  if( !m_Initialized)
    this->SetupCalibrator();

}

} // end namespace otb

#endif
