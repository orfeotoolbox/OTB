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
#include "otbRadarFunctors.h"

namespace otb
{


/** \class TerraSarRadarBrightnessImageFilter
 *  \brief Calculates the intra calibration of TerraSar sensor images.
 *
 * \ingroup Radiometry
 */

template <class TInputImage, class TOutputImage >
class ITK_EXPORT TerraSarRadarBrightnessImageFilter :
  public itk::UnaryFunctorImageFilter< 
    TInputImage,
    TOutputImage,
    ITK_TYPENAME Functor::TerraSarRadarBrightnessImageFunctor< ITK_TYPENAME TInputImage::InternalPixelType, ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::TerraSarRadarBrightnessImageFunctor<ITK_TYPENAME TInputImage::InternalPixelType,
  ITK_TYPENAME OutputImageType::InternalPixelType> FunctorType;
  /** "typedef" for standard classes. */
  typedef TerraSarRadarBrightnessImageFilter    Self;
  typedef itk::UnaryFunctorImageFilter< InputImageType, OutputImageType, FunctorType > 
                                                Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(TerraSarRadarBrightnessImageFilter, itk::UnaryFunctorImageFilter);


  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<DoubleVectorType>         DoubleVectorVectorType;

  /** Accessors */
  void SetCalFactor(double pCalFactor){
    this->GetFunctor().SetCalFactor( pCalFactor );
    this->Modified();
  }
  double GetCalFactor(){
    return this->GetFunctor().GetCalFactor();
  }

protected:
  /** Constructor */
  TerraSarRadarBrightnessImageFilter();
  /** Destructor */
  virtual ~TerraSarRadarBrightnessImageFilter() {};

  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();

private:

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarRadarBrightnessImageFilter.txx"
#endif

#endif
