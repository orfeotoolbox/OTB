/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbKullbackLeiblerSupervizedDistanceImageFilter_h
#define __otbKullbackLeiblerSupervizedDistanceImageFilter_h

#include "otbKullbackLeiblerDistanceImageFilter.h"

#include "otbROIdataConversion.h"

namespace otb
{

/** \class KullbackLeiblerSupervizedDistanceImageFilter
 * \brief Implements KullbackLeibler distance over Edgeworth approximation,
 * between a Neighborhood and a predefined Region of Interest.
 *
 * This filter is parametrized over the types of the two
 * input images and the type of the output image.
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input
 * images, and for each one of them it will do the following:
 *
 * - cast the input 1 pixel value to \c double
 * - compute the first four cumulants of the ROI
 * - cast the input 2 pixel value to \c double
 * - compute the first four cumulants of the pixel values
 * - compute the value of the Edgeorth approximation of the KL distance
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *
 * The filter expect all images to have the same dimension
 * (e.g. all 2D, or all 3D, or all ND)
 *
 * See article of  Lin Saito et Levine
 * "Edgeworth Approximation of the Kullback-Leibler Distance Towards Problems in Image Analysis"
 * and
 * "Edgeworth Expansions of the Kullback-Leibler Information" (submitted to JASA, nov 25, 1999)
 * http://www.math.ucdavis.edu/~saito/publications
 *
 * \sa CumulantsForEdgeworth
 */
namespace Functor
{
/** \class KullbackLeiblerSupervizedDistance
 * \brief Functor for KullbackLeiblerSupervizedDistanceImageFilter. Please refer to KullbackLeiblerSupervizedDistanceImageFilter.
 *
 */
template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
class KullbackLeiblerSupervizedDistance
{
public:
  KullbackLeiblerSupervizedDistance ();
  virtual ~KullbackLeiblerSupervizedDistance ();

  /** performs the preprocess calculation on the training area */
  void Evaluate(const typename TInput1::ImageType * img1,
                const typename TInput2::ImageType * img2,
                const TInputROIImage * imgROI);

  /** The functor by itself */
  TOutput operator ()(const TInput1& it1, const TInput2& it2);

protected:
  typedef ROIdataConversion<
      typename TInput1::ImageType, TInputROIImage> ROIConversionType1;

  typedef itk::ConstNeighborhoodIterator<
      typename ROIConversionType1::OutputImageType> ROIInputType1;

  typedef ROIdataConversion<
      typename TInput2::ImageType, TInputROIImage> ROIConversionType2;

  typedef itk::ConstNeighborhoodIterator<
      typename ROIConversionType2::OutputImageType> ROIInputType2;

  CumulantsForEdgeworth<ROIInputType1> * m_CumROI1;
  CumulantsForEdgeworth<ROIInputType2> * m_CumROI2;
private:
  KullbackLeiblerSupervizedDistance (const KullbackLeiblerSupervizedDistance&);
};

} // Functor

template <class TInputImage1, class TInputImage2, class TInputROIImage, class TOutputImage>
class ITK_EXPORT KullbackLeiblerSupervizedDistanceImageFilter :
  public otb::BinaryFunctorNeighborhoodImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      Functor::KullbackLeiblerSupervizedDistance<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          TInputROIImage,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef KullbackLeiblerSupervizedDistanceImageFilter Self;
  typedef typename otb::BinaryFunctorNeighborhoodImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      Functor::KullbackLeiblerSupervizedDistance<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          TInputROIImage,
          typename TOutputImage::PixelType>
      >  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Vectors that hold the training area */
  typedef typename TInputImage1::PixelType MeasurementType1;
  typedef typename TInputImage2::PixelType MeasurementType2;

  typedef std::vector<MeasurementType1> TrainingMeasureType1;
  typedef std::vector<MeasurementType2> TrainingMeasureType2;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Method for creation of the training area and the computation
   * of some reference cumulants */
  void  SetTrainingArea(const TInputROIImage * trainingImage);

protected:
  virtual void BeforeThreadedGenerateData(void);

  KullbackLeiblerSupervizedDistanceImageFilter()
    {
    this->SetNumberOfRequiredInputs(3);
    }
  virtual ~KullbackLeiblerSupervizedDistanceImageFilter() {}

private:
  KullbackLeiblerSupervizedDistanceImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKullbackLeiblerSupervizedDistanceImageFilter.txx"
#endif

#endif
