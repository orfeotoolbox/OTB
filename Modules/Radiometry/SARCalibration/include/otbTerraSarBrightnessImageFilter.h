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
#ifndef __otbTerraSarBrightnessImageFilter_h
#define __otbTerraSarBrightnessImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkMetaDataDictionary.h"

#include "otbTerraSarBrightnessFunctor.h"

namespace otb
{
/** \class TerraSarBrightnessImageFilter
 *  \brief Calculates the brightness of TerraSar sensor images.
 *
 * Implementation of the brightness algorithm has been made following
 * Infoterra documentation  "Radiometric Calibration of TerraSAR-X
 * Data".
 *
 * This filter computes the beta naught (\f$ \beta^{0} \f$) using the following formula:
 * \f[\beta^{0} =  k_{s} \cdot |DN|^{2} \f]
 *
 * Where \f$ k_{s} \f$ is the calibration factor, which is imported
 * from the input image metadata, even if one can set different values
 * using the proper accessors.
 *
 * Results can be obtained either in linear or logarithmic scale
 * (decibels), using the ResultsInDecibels flag.
 *
 * This filter works with either real or complex image. In the case of
 * the complex images (SLC products for instance), modulus and phase
 * are extracted, sigma naught is computed from the modulus and the
 * phase is set back to the result.
 *
 * For implementation details, consider reading the code
 * TerraSarBrightnessFunctor.
 *
 * \sa TerraSarBrightnessFunctor
 * \sa TerraSarBrightnessImageFilter
 *
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup Radiometry
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT TerraSarBrightnessImageFilter :
  public itk::UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      typename Functor::TerraSarBrightnessFunctor<
          typename itk::NumericTraits<typename TInputImage::InternalPixelType>::ValueType,
          typename itk::NumericTraits<typename TOutputImage::InternalPixelType>::ValueType> >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage                                                      InputImageType;
  typedef TOutputImage                                                     OutputImageType;
  typedef typename InputImageType::InternalPixelType                       InputInternalPixelType;
  typedef typename OutputImageType::InternalPixelType                      OutputInternalPixelType;
  typedef typename  itk::NumericTraits<InputInternalPixelType>::ValueType  InputValueType;
  typedef typename  itk::NumericTraits<OutputInternalPixelType>::ValueType OutputValueType;
  typedef typename Functor::TerraSarBrightnessFunctor
  <InputValueType, OutputValueType>                                       FunctorType;
  typedef itk::MetaDataDictionary MetaDataDictionaryType;

  /** "typedef" for standard classes. */
  typedef TerraSarBrightnessImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<InputImageType,
      OutputImageType, FunctorType>     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(TerraSarBrightnessImageFilter, itk::UnaryFunctorImageFilter);

  /** Set the calibration factor*/
  void SetCalibrationFactor(double pCalibrationFactor)
  {
    this->GetFunctor().SetCalibrationFactor(pCalibrationFactor);
    this->Modified();
  }

  /** Get the calibration factor */
  double GetCalibrationFactor()
  {
    return this->GetFunctor().GetCalibrationFactor();
  }

  /** Set the ResultsInDecibels flag */
  void SetResultsInDecibels(bool db)
  {
    this->GetFunctor().SetResultsInDecibels(db);
    this->Modified();
  }

  /** Get the ResultsInDecibels flag */
  bool GetResultsInDecibels()
  {
    return this->GetFunctor().GetResultsInDecibels();
  }

  /** Set the default value (replacing negative beta) */
  void SetDefaultValue(double value)
  {
    this->GetFunctor().SetResultsInDecibels(value);
    this->Modified();
  }

  double GetDefaultValue()
  {
    return this->GetFunctor().SetResultsInDecibels();
  }

protected:
  /** Constructor */
  TerraSarBrightnessImageFilter(){};
  /** Destructor */
  virtual ~TerraSarBrightnessImageFilter() {}

  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();

private:
  TerraSarBrightnessImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarBrightnessImageFilter.txx"
#endif

#endif
