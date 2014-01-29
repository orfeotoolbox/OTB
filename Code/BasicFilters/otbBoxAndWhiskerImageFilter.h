/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbBoxAndWhiskerImageFilter_h
#define __otbBoxAndWhiskerImageFilter_h

#include "itkMacro.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb {

/** \class BoxAndWhiskerImageFilter
 * \brief This class performs the detection of outlier with the Box and Whisker technique
 *
 * This is appropriated to perform simple outlier detection over vector data.
 * The input image has to contain vector pixel through a VectorImage type.
 * When an outlier is detected, it is treated to as an missing value through the function
 * \code otb::EuclideanDistanceMetricWithMissingValue::SetToMissingValue() \endcode.
 *
 * A component is considered to as a missing value when
 * \f$ \| x_i - \beta \left( x_{3/4} - x_{1/4} \right) \| > \| x_{1/2} \| \f$
 * where \f$ x_{1/4}, x_{1/2}, x_{3/4} \f$ stand for the first, second (median) and
 * third quantile values.
 *
 * \ingroup Streamed
 * \sa EuclideanDistanceMetricWithMissingValue
 */
template <class TInputImage>
class ITK_EXPORT BoxAndWhiskerImageFilter
  : public itk::InPlaceImageFilter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BoxAndWhiskerImageFilter                      Self;
  typedef typename itk::InPlaceImageFilter<TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BoxAndWhiskerImageFilter, InPlaceImageFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  typedef typename Superclass::InputImageType         InputImageType;
  typedef typename Superclass::InputImagePointer      InputImagePointer;
  typedef typename Superclass::InputImageConstPointer InputImageConstPointer;
  typedef typename Superclass::InputImageRegionType   InputImageRegionType;
  typedef typename Superclass::InputImagePixelType    InputImagePixelType;

  typedef typename InputImageType::PixelType  PixelType;
  typedef typename InputImageType::InternalPixelType   ValueType;
  typedef typename InputImageType::SizeType   SizeType;
  typedef typename InputImageType::RegionType RegionType;

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);

  /** Get the Radius (all to 1) */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Fix the whisker */
  itkGetConstMacro(Beta, double);
  itkSetMacro(Beta, double);
  itkGetConstMacro(NumberFound, unsigned int);

protected:
  BoxAndWhiskerImageFilter ();
  virtual ~BoxAndWhiskerImageFilter () {}

  /** Main computation method implemented as a multithreaded filter */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  virtual void GenerateOutputInformation();
  virtual void AllocateOutputs();

  /** Perform the outlier detection */
  PixelType PerformBoxAndWhiskerDetection(const PixelType& pixel);

private:
  BoxAndWhiskerImageFilter (const Self &);
  void operator =(const Self&);    // not implemented

  SizeType m_Radius;
  double   m_Beta;
  long int m_NumberFound;

}; // end of class BoxAndWhiskerImageFilter

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBoxAndWhiskerImageFilter.txx"
#endif

#endif
