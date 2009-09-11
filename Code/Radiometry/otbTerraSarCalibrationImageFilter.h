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
#ifndef __otbTerraSarCalibrationImageFilter_h
#define __otbTerraSarCalibrationImageFilter_h


#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "otbTerraSarRadarBrightnessImageFilter.h"
#include "itkMetaDataDictionary.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/**
   * \class TerraSarCalibrationImageFunctor
   *  \brief Compute the surface reflectance pixel from a TOA reflectance.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template<class TInputIt, class TOutput>
class TerraSarCalibrationImageFunctor
{
public:
  TerraSarCalibrationImageFunctor();
  virtual ~TerraSarCalibrationImageFunctor() {};

  typedef std::vector<double>           DoubleVectorType;
  typedef std::vector<DoubleVectorType> DoubleVectorVectorType;
  typedef std::vector<long int>         LIntVectorType;
  typedef itk::Size<2>                  SizeType;
  typedef typename TInputIt::PixelType           InputPixelType;
  typedef TerraSarRadarBrightnessImageFunctor<InputPixelType, TOutput> BrightnessFunctorType;

  /** Accessors */
  void SetCalFactor( double val ) { m_CalFactor = val; m_RadarBrightness.SetCalFactor(val); };
  double GetCalFactor() { return m_CalFactor; };
  void SetNoiseRangeValidityMin( double val ) { m_NoiseRangeValidityMin = val; };
  double GetNoiseRangeValidityMin() { return m_NoiseRangeValidityMin; };
  void SetNoiseRangeValidityMax( double val ) { m_NoiseRangeValidityMax = val; };
  double GetNoiseRangeValidityMax() { return m_NoiseRangeValidityMax; };
  void SetNoiseRangeValidityRef( double val ) { m_NoiseRangeValidityRef = val; };
  double GetNoiseRangeValidityRef() { return m_NoiseRangeValidityRef; };
  void SetLocalIncidentAngle( double val )
  { 
    m_LocalIncidentAngle = val; 
    m_SinLocalIncidentAngle = vcl_sin(m_LocalIncidentAngle*CONST_PI_180);
  };
  double GetLocalIncidentAngle() { return m_LocalIncidentAngle; };
  double GetSinLocalIncidentAngle() const { return m_SinLocalIncidentAngle; };
  void SetNoisePolynomialCoefficientsList( DoubleVectorVectorType vect ) { m_NoisePolynomialCoefficientsList = vect; };
  DoubleVectorVectorType GetNoisePolynomialCoefficientsList() { return m_NoisePolynomialCoefficientsList; };
  void SetImageSize( SizeType size ) { m_ImageSize = size; };
  SizeType GetImageSize() { return m_ImageSize; };
  void SetUseFastCalibrationMethod( bool b ) { m_UseFastCalibrationMethod = b; };
  bool GetUseFastCalibrationMethod() { return m_UseFastCalibrationMethod; };
  void SetTimeUTC( LIntVectorType vect ) { m_TimeUTC = vect; };
  LIntVectorType GetTimeUTC() { return m_TimeUTC; };
  void SetPRF( double val ) { m_PRF = val; m_InvPRF = 1./m_PRF; };
  double GetPRF() { return m_PRF; };
  double GetInvPRF() const { return m_InvPRF; };

  double ComputeCurrentNoise( unsigned int colId );
  DoubleVectorType ComputeCurrentCoeffs( unsigned int lineId );
  inline TOutput operator() (const TInputIt & inIt);

private:
  /** Calibration Factor */
  double m_CalFactor;
  /** Noise minimal range validity */
  double m_NoiseRangeValidityMin;
  /** Noise maxinimal range validity */
  double m_NoiseRangeValidityMax;
  /** Noise reference range */
  double m_NoiseRangeValidityRef;
  /** Sensor local incident angle in degree */
  double m_LocalIncidentAngle;
  /** sin of the LocalIncidentAngle */
  double m_SinLocalIncidentAngle;
  /** Vector of vector that contain noise polinomial coefficient */
  DoubleVectorVectorType m_NoisePolynomialCoefficientsList;
  /** Image Size */
  SizeType m_ImageSize;
  /** Fast Calibration Method. If set to trus, will consider only the first noise coefficient else, 
   *  will use all of them and applied it according to its acquisition UTC time and the coordinates 
   *  of the pixel in the image. */
  bool m_UseFastCalibrationMethod;
  /** TimeUTC for each noise coefficient acquisition (in second). */
  LIntVectorType m_TimeUTC;
  /** Pulse Repetition Frequency */
  double m_PRF;
  /** Inverse Pulse Repetition Frequency */
  double m_InvPRF;
  /** Radar Brightness functor */
  BrightnessFunctorType m_RadarBrightness;
};
}

/** \class TerraSarCalibrationImageFilter
 *  \brief Calculates the calibration of TerraSar sensor images.
 *
 *  For details, please refer to Infoterra documentation : "Radiometric Calibration of TerraSAR-X Data".
 *  Using FastCalibration boolean allows to compute the result considering the first noise polygone 
 *  coeffcients for each line or to use every polygones coefficient given. 
 *  The code was made considering that the first acquisition point is the lower left one. 
 *
 * \ingroup Radiometry
 */

template<class TInputImage, class TOutputImage >
class ITK_EXPORT TerraSarCalibrationImageFilter :
  public UnaryFunctorNeighborhoodImageFilter< 
    TInputImage,
    TOutputImage,
    ITK_TYPENAME Functor::TerraSarCalibrationImageFunctor< ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>, ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::TerraSarCalibrationImageFunctor<ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>,
  ITK_TYPENAME OutputImageType::InternalPixelType>                                            FunctorType;
  /** "typedef" for standard classes. */
  typedef TerraSarCalibrationImageFilter                                                 Self;
  typedef UnaryFunctorNeighborhoodImageFilter< InputImageType, OutputImageType, FunctorType > Superclass;
  typedef itk::SmartPointer<Self>                                                             Pointer;
  typedef itk::SmartPointer<const Self>                                                       ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  // Use a with neighborhood to have access to the pixel coordinates
  itkTypeMacro(TerraSarCalibrationImageFilter, UnaryFunctorNeighborhoodImageFilter);

  typedef itk::MetaDataDictionary                       MetaDataDictionaryType;
  typedef typename FunctorType::DoubleVectorType        DoubleVectorType;
  typedef typename FunctorType::DoubleVectorVectorType  DoubleVectorVectorType;
  typedef typename FunctorType::LIntVectorType          LIntVectorType;

  /** Accessors */
  /** Calibration Factor */
  void SetCalFactor( double val );
  double GetCalFactor();
  /** Noise minimal range validity */
  void SetNoiseRangeValidityMin( double val );
  double GetNoiseRangeValidityMin() ;
  /** Noise maximal range validity */
  void SetNoiseRangeValidityMax( double val );
  double GetNoiseRangeValidityMax();
  /** Noise reference range validity */
  void SetNoiseRangeValidityRef( double val );
  double GetNoiseRangeValidityRef();
  /** Sensor local incident angle in degree */
  void SetLocalIncidentAngle( double val );
  double GetLocalIncidentAngle();
  /** Sinus of the sensor local incident angle in degree */
  double GetSinLocalIncidentAngle() const;
  /** Vector of vector that contain noise polinomial coefficient */
  void SetNoisePolynomialCoefficientsList( DoubleVectorVectorType vect );
  DoubleVectorVectorType GetNoisePolynomialCoefficientsList();
  /** Fast Calibration Method. If set to trus, will consider only the first noise coefficient else, 
   *  will use all of them and applied it according to its acquisition UTC time and the coordinates 
   *  of the pixel in the image. */
  void SetUseFastCalibrationMethod( bool b );
  bool GetUseFastCalibrationMethod();
  /** TimeUTC for each noise coefficient acquisition (in second). */
  void SetTimeUTC( LIntVectorType vect );
  LIntVectorType GetTimeUTC();
  /** Pulse Repetition Frequency */
  void SetPRF( double val );
  double GetPRF();
  /** Inverse Pulse Repetition Frequency */
  double GetInvPRF() const;

protected:
  /** Constructor */
  TerraSarCalibrationImageFilter();
  /** Destructor */
  virtual ~TerraSarCalibrationImageFilter() {};
  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();
  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarCalibrationImageFilter.txx"
#endif

#endif
