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
    ITK_TYPENAME Functor::TerraSarCalibrationImageFunctor< ITK_TYPENAME itk::NumericTraits<ITK_TYPENAME TInputImage::InternalPixelType>::ValueType,
                                                           ITK_TYPENAME itk::NumericTraits<ITK_TYPENAME TOutputImage::InternalPixelType>::ValueType > >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage                                                                         InputImageType;
  typedef TOutputImage                                                                        OutputImageType;
  typedef typename InputImageType::InternalPixelType                                          InputInternalPixelType;
  typedef typename OutputImageType::InternalPixelType                                         OutputInternalPixelType;
  typedef typename  itk::NumericTraits<InputInternalPixelType>::ValueType                     InputValueType;
  typedef typename  itk::NumericTraits<OutputInternalPixelType>::ValueType                    OutputValueType;
  typedef typename Functor::TerraSarCalibrationImageFunctor< InputValueType, OutputValueType> FunctorType;  

  /** typedef to access metadata */
  typedef itk::MetaDataDictionary                                                             MetaDataDictionaryType;
  typedef typename InputImageType::SizeType                                                   SizeType;

  /** "typedef" for standard classes. */
  typedef TerraSarCalibrationImageFilter                                                      Self;
  typedef UnaryFunctorWithIndexImageFilter< InputImageType, OutputImageType, FunctorType >    Superclass;
  typedef itk::SmartPointer<Self>                                                             Pointer;
  typedef itk::SmartPointer<const Self>                                                       ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  // Use a with neighborhood to have access to the pixel coordinates
  itkTypeMacro(TerraSarCalibrationImageFilter, UnaryFunctorWithIndexImageFilter);

  /** Accessors */
  /** Calibration Factor */
  void SetCalibrationFactor( double val );
  double GetCalibrationFactor() const;

  /** Sensor local incident angle in degree */
  void SetLocalIncidentAngle( double val );
  double GetLocalIncidentAngle() const;
  
  /** Fast Calibration Method. If set to trus, will consider only the first noise coefficient else,
   *  will use all of them and applied it according to its acquisition UTC time and the coordinates
   *  of the pixel in the image. */
  void SetUseFastCalibrationMethod( bool b );
  bool GetUseFastCalibrationMethod() const;

  /** Pulse Repetition Frequency */
  void SetPRF( double val );
  double GetPRF() const;

  /** Image Size setter. This is provided in case of calbrating an
* extract of a larger scene. It does not interfer with output image
* regions */
  void SetImageSize( SizeType size );

  /** Get the image size */
  const SizeType& GetImageSize() const;
  
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
  // TODO: add private copy and reference construtor
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarCalibrationImageFilter.txx"
#endif

#endif
