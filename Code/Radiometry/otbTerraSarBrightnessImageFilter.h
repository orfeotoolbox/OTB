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
#ifndef __otbTerraSarRadarBrightnessImageFilter_h
#define __otbTerraSarRadarBrightnessImageFilter_h


#include "itkUnaryFunctorImageFilter.h"
#include "itkMetaDataDictionary.h"
#include "otbMath.h"
#include "otbTerraSarBrightnessFunctor.h"

namespace otb
{


/** \class TerraSarRadarBrightnessImageFilter
 *  \brief Calculates the intra calibration of TerraSar sensor images.
 *
 * \ingroup Radiometry
 */

template <class TInputImage, class TOutputImage >
class ITK_EXPORT TerraSarBrightnessImageFilter :
  public itk::UnaryFunctorImageFilter<
  TInputImage,
  TOutputImage,
  ITK_TYPENAME Functor::TerraSarBrightnessFunctor< ITK_TYPENAME itk::NumericTraits<ITK_TYPENAME TInputImage::InternalPixelType>::ValueType,
                                                        ITK_TYPENAME itk::NumericTraits<ITK_TYPENAME TOutputImage::InternalPixelType>::ValueType > >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename InputImageType::InternalPixelType                                          InputInternalPixelType;
  typedef typename OutputImageType::InternalPixelType                                         OutputInternalPixelType;
  typedef typename  itk::NumericTraits<InputInternalPixelType>::ValueType                     InputValueType;
  typedef typename  itk::NumericTraits<OutputInternalPixelType>::ValueType                    OutputValueType;
  typedef typename Functor::TerraSarBrightnessFunctor< InputValueType, OutputValueType> FunctorType;

  /** "typedef" for standard classes. */
  typedef TerraSarBrightnessImageFilter    Self;
  typedef itk::UnaryFunctorImageFilter< InputImageType, OutputImageType, FunctorType >
                                                Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(TerraSarBrightnessImageFilter, itk::UnaryFunctorImageFilter);


  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<DoubleVectorType>         DoubleVectorVectorType;

  /** Accessors */
  void SetCalibrationFactor(double pCalibrationFactor)
    {
      this->GetFunctor().SetCalibrationFactor( pCalibrationFactor );
      this->Modified();
    }
  double GetCalibrationFactor(){
    return this->GetFunctor().GetCalibrationFactor();
  }

  void SetResultsIndecibels(bool db)
  {
    this->GetFunctor().SetResultsInDecibels(db);
    this->Modified();
  }

  bool GetResultsInDecibels()
  {
    return this->GetFunctor().GetResultsInDecibels();
  }

protected:
  /** Constructor */
  TerraSarBrightnessImageFilter(){};
  /** Destructor */
  virtual ~TerraSarBrightnessImageFilter() {};

  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();

private:

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarBrightnessImageFilter.txx"
#endif

#endif
