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
#ifndef __otbTerraSarIntraCalibrationImageFilter_h
#define __otbTerraSarIntraCalibrationImageFilter_h


#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "itkMetaDataDictionary.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/**
   * \class TerraSarIntraCalibrationImageFunctor
   *  \brief Compute the surface reflectance pixel from a TOA reflectance.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template <class TInputIt, class TOutput>
class TerraSarIntraCalibrationImageFunctor
{
public:
  TerraSarIntraCalibrationImageFunctor()
  {
    m_CalFactor = 1.;
    m_NoiseRangeValidityMin = 0.;
    m_NoiseRangeValidityMax = 0.;
    m_NoiseRangeValidityRef = 0.;
    m_LocalIncidentAngle = 0.;
    m_NoisePolynomialCoefficientsList.clear();
    m_ImageSize.Fill(0);
  };
  virtual ~TerraSarIntraCalibrationImageFunctor() {};

  typedef std::vector<double>           DoubleVectorType;
  typedef std::vector<DoubleVectorType> DoubleVectorVectorType;
  typedef itk::Size<2>                  SizeType;

  /** Accessors */
  void SetCalFactor( double val ) { m_CalFactor = val; };
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


  double ComputeCurrentNoise( unsigned int colId )
  {
    double curRange = 0.;
    double width_2 = static_cast<double>(m_ImageSize[0])/2.;

    if( colId < static_cast<unsigned int>(width_2) )
      {
	curRange = m_NoiseRangeValidityMin + ( m_NoiseRangeValidityRef-m_NoiseRangeValidityMin )/width_2 * static_cast<double>(colId);
      }
    else
      {
	curRange = m_NoiseRangeValidityRef + ( m_NoiseRangeValidityMax-m_NoiseRangeValidityRef )/width_2 * static_cast<double>(colId);
      }
    return curRange;
  }

  DoubleVectorType ComputeCurrentCoeffs( unsigned int lineId )
  {
    DoubleVectorType curCoeffs;
    curCoeffs = m_NoisePolynomialCoefficientsList[0];
    return curCoeffs;
  }

  inline TOutput operator() (const TInputIt & inIt)
  {
    //std::cout<<inIt.GetIndex()<<" -> "<<inIt.GetCenterPixel()<<std::endl;
    double squareInPix = vcl_pow( static_cast<double>( inIt.GetCenterPixel() ), 2);
    double diffCurRange = ComputeCurrentNoise( static_cast<unsigned int>(inIt.GetIndex()[0]) ) - m_NoiseRangeValidityRef;
    DoubleVectorType curCoeff = ComputeCurrentCoeffs( static_cast<unsigned int>(inIt.GetIndex()[1]) );

    double NEBN = 0.;
    for(int i=0; i<curCoeff.size(); i++)
      {
	NEBN += curCoeff[i]*vcl_pow( diffCurRange, i);
      }
    double sigma = m_CalFactor*(squareInPix-NEBN)*m_SinLocalIncidentAngle;
    
    std::cout<<sigma<<std::endl;

    return static_cast<TOutput>(sigma);
  }

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
};
}

/** \class TerraSarIntraCalibrationImageFilter
 *  \brief Calculates the intra calibration of TerraSar sensor images.
 *
 * \ingroup Radiometry
 */

template <class TInputImage, class TOutputImage >
class ITK_EXPORT TerraSarIntraCalibrationImageFilter :
  public UnaryFunctorNeighborhoodImageFilter< 
    TInputImage,
    TOutputImage,
    ITK_TYPENAME Functor::TerraSarIntraCalibrationImageFunctor< ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>, ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::TerraSarIntraCalibrationImageFunctor<ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>,
  ITK_TYPENAME OutputImageType::InternalPixelType> FunctorType;
  /** "typedef" for standard classes. */
  typedef TerraSarIntraCalibrationImageFilter                                          Self;
  typedef UnaryFunctorNeighborhoodImageFilter< InputImageType, OutputImageType, FunctorType > Superclass;
  typedef itk::SmartPointer<Self>                                                      Pointer;
  typedef itk::SmartPointer<const Self>                                                ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  // Use a with neighborhood to have access to the pixel coordinates
  itkTypeMacro(TerraSarIntraCalibrationImageFilter, UnaryFunctorNeighborhoodImageFilter);

  /** Supported images definition. */
  //typedef typename InputImageType::PixelType                            InputPixelType;
  //typedef typename InputImageType::InternalPixelType                    InputInternalPixelType;
  //typedef typename InputImageType::RegionType                           InputImageRegionType;
  //typedef typename OutputImageType::PixelType                           OutputPixelType;
  //typedef typename OutputImageType::InternalPixelType                   OutputInternalPixelType;
  //typedef typename OutputImageType::RegionType                          OutputImageRegionType;

  typedef itk::MetaDataDictionary                                       MetaDataDictionaryType;
  typedef std::vector<double>                                           DoubleVectorType;
  typedef std::vector<DoubleVectorType>                                 DoubleVectorVectorType;

  /** Accessors */
  itkGetMacro(CalFactor, double);
  itkSetMacro(CalFactor, double);
  itkGetMacro(NoiseRangeValidityMin, double);
  itkSetMacro(NoiseRangeValidityMin, double);
  itkGetMacro(NoiseRangeValidityMax, double);
  itkSetMacro(NoiseRangeValidityMax, double);
  itkGetMacro(NoiseRangeValidityRef, double);
  itkSetMacro(NoiseRangeValidityRef, double);
  itkGetMacro(LocalIncidentAngle, double);
  itkSetMacro(LocalIncidentAngle, double);
  void SetNoisePolynomialCoefficientsList( DoubleVectorVectorType vect )
  {
    m_NoisePolynomialCoefficientsList = vect;
    this->Modified();
  };
  DoubleVectorVectorType GetNoisePolynomialCoefficientsList()
  {
    return m_NoisePolynomialCoefficientsList;
  };


protected:
  /** Constructor */
  TerraSarIntraCalibrationImageFilter();
  /** Destructor */
  virtual ~TerraSarIntraCalibrationImageFilter() {};

  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();
  
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
  /** vector of vector that contain noise polinomial coefficient */
  DoubleVectorVectorType m_NoisePolynomialCoefficientsList;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarIntraCalibrationImageFilter.txx"
#endif

#endif
