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


#include "otbUnaryFunctorWithIndexImageFilter.h"
#include "otbTerraSarFunctors.h"
#include "itkMetaDataDictionary.h"
//#include "itkUnaryFunctorImageFilter.h"
//#include "itkConstNeighborhoodIterator.h"
#include "otbMath.h"

namespace otb
{


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
  public UnaryFunctorWithIndexImageFilter<
    TInputImage,
    TOutputImage,
    ITK_TYPENAME Functor::TerraSarCalibrationImageFunctor< ITK_TYPENAME TInputImage::InternalPixelType, ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::TerraSarCalibrationImageFunctor<ITK_TYPENAME InputImageType::InternalPixelType,
  ITK_TYPENAME OutputImageType::InternalPixelType>                                            FunctorType;
  /** "typedef" for standard classes. */
  typedef TerraSarCalibrationImageFilter                                                 Self;
  typedef UnaryFunctorWithIndexImageFilter< InputImageType, OutputImageType, FunctorType > Superclass;
  typedef itk::SmartPointer<Self>                                                             Pointer;
  typedef itk::SmartPointer<const Self>                                                       ConstPointer;
  
  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  // Use a with neighborhood to have access to the pixel coordinates
  itkTypeMacro(TerraSarCalibrationImageFilter, UnaryFunctorWithIndexImageFilter);

  typedef itk::MetaDataDictionary                       MetaDataDictionaryType;
  typedef typename FunctorType::DoubleVectorType        DoubleVectorType;
  typedef typename FunctorType::DoubleVectorVectorType  DoubleVectorVectorType;
  typedef typename InputImageType::SizeType                      SizeType;

//  typedef typename FunctorType::LIntVectorType          LIntVectorType;

  /** Accessors */
  /** Calibration Factor */
  void SetCalFactor( double val );
  double GetCalFactor() const;
  /** Noise minimal range validity */
  void SetNoiseRangeValidityMin( double val );
  double GetNoiseRangeValidityMin() const;

  /** Noise maximal range validity */
  void SetNoiseRangeValidityMax( double val );
  double GetNoiseRangeValidityMax() const;
  /** Noise reference range validity */
  void SetNoiseRangeValidityRef( double val );
  double GetNoiseRangeValidityRef() const;
  /** Sensor local incident angle in degree */
  void SetLocalIncidentAngle( double val );
  double GetLocalIncidentAngle() const;
  /** Sinus of the sensor local incident angle in degree */
  double GetSinLocalIncidentAngle() const;
  /** Vector of vector that contain noise polinomial coefficient */
  void SetNoisePolynomialCoefficientsList( DoubleVectorVectorType vect );
  DoubleVectorVectorType GetNoisePolynomialCoefficientsList() const;
  /** Image size (setter is protected)*/
  SizeType GetImageSize() const;
  
  /** Fast Calibration Method. If set to trus, will consider only the first noise coefficient else,
   *  will use all of them and applied it according to its acquisition UTC time and the coordinates
   *  of the pixel in the image. */
  void SetUseFastCalibrationMethod( bool b );
  bool GetUseFastCalibrationMethod() const;
  /** TimeUTC for each noise coefficient acquisition (in Julian day). */
  void SetTimeUTC( DoubleVectorType vect );
  DoubleVectorType GetTimeUTC() const;
  /** Pulse Repetition Frequency */
  void SetPRF( double val );
  double GetPRF() const;
  /** Inverse Pulse Repetition Frequency */
  double GetInvPRF() const;

protected:
  /** Constructor */
  TerraSarCalibrationImageFilter();
  /** Destructor */
  virtual ~TerraSarCalibrationImageFilter() {};
  /** Image Size setter*/
  void SetImageSize( SizeType size );

  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();
  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
bool m_ParamLoaded;
};



} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarCalibrationImageFilter.txx"
#endif

#endif
