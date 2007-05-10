/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbJoinHistogramMIImageFilter_h
#define __otbJoinHistogramMIImageFilter_h

#include "otbBinaryFunctorNeighborhoodJoinHistogramImageFilter.h"
#include "itkHistogram.h"

namespace otb
{
  
/** \class JoinHistogramMIImageFilter
 * \brief Implements neighborhood-wise the computation of mutual
 * information using a join histogram.
 *
 * This filter is parametrized over the types of the two 
 * input images and the type of the output image. 
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following: 
 *
 * - cast the input 1 pixel value to \c double 
 * - cast the input 2 pixel value to \c double 
 * - compute the difference of the two pixel values
 * - compute the value of the mutual information
 * - cast the \c double value resulting to the pixel type of the output image 
 * - store the casted value into the output image.
 * 
 * The filter expect all images to have the same dimension 
 * (e.g. all 2D, or all 3D, or all ND)
 * 
 * \ingroup IntensityImageFilters Multithreaded
 */
namespace Functor {  
  
template< class TInput1, class TInput2, class TOutput>
class JoinHistogramMI
{
public:
  typedef double HistogramFrequencyType;
  typedef typename itk::Statistics::Histogram<HistogramFrequencyType, 2> HistogramType;
  JoinHistogramMI() {};
  ~JoinHistogramMI() {};
  inline TOutput operator()( const TInput1 & itA, 
                             const TInput2 & itB, const HistogramType* histogram)
  {
    TOutput jointEntropy = itk::NumericTraits<TOutput>::Zero;
    HistogramFrequencyType totalFreq = histogram->GetTotalFrequency();

/*    for(unsigned long pos = 0; pos< itA.Size(); ++pos)
      {
      double value = static_cast<double>(itA.GetPixel(pos));

      unsigned int bin = 
      HistogramFrequencyType freq = histogram.GetFrequency(, 0);
      if (freq > 0)
      {
        entropyX += freq*vcl_log(freq);
      }
    }

    entropyX = -entropyX/static_cast<TOutput>(totalFreq) + vcl_log(totalFreq);

    for (unsigned int i = 0; i < this->GetHistogramSize()[1]; i++)
    {
      HistogramFrequencyType freq = histogram.GetFrequency(i, 1);
      if (freq > 0) 
      {
        entropyY += freq*vcl_log(freq);
      }
    }
    
    entropyY = -entropyY/static_cast<TOutput>(totalFreq) + vcl_log(totalFreq);

    HistogramIteratorType it = histogram.Begin();
    HistogramIteratorType end = histogram.End();
    while (it != end) 
    {
      HistogramFrequencyType freq = it.GetFrequency();
      if (freq > 0)
      {
        jointEntropy += freq*vcl_log(freq);
      }
      ++it;
    }

    jointEntropy = -jointEntropy/static_cast<TOutput>(totalFreq) +
      vcl_log(totalFreq);

    return entropyX + entropyY - jointEntropy;*/


    typename HistogramType::MeasurementVectorType sample;
    for(unsigned long pos = 0; pos< itA.Size(); ++pos)
      {
      double valueA = static_cast<double>(itA.GetPixel(pos));
      double valueB = static_cast<double>(itB.GetPixel(pos));

        sample[0] = valueA;
        sample[1] = valueB;


	HistogramFrequencyType freq = histogram->GetFrequency(
	                               histogram->GetIndex(sample));
	if (freq > 0)
	  {
	  jointEntropy += freq*vcl_log(freq);
	  }

      }
    
    jointEntropy = -jointEntropy/static_cast<TOutput>(totalFreq) +
      vcl_log(totalFreq);
    
    return jointEntropy;
    
/*    TOutput meanA = 0.0;
    TOutput meanB = 0.0;

    for(unsigned long pos = 0; pos< itA.Size(); ++pos)
      {

      meanA += static_cast<TOutput>(itA.GetPixel(pos));
      meanB += static_cast<TOutput>(itB.GetPixel(pos));
      

      }*/
    return static_cast<TOutput>( 0 );
  }

/*  void SetHistogram(HistogramType* histo)
  {
    m_Histogram = histo;
  }

protected:
  HistogramType::Pointer m_Histogram;*/
}; 
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT JoinHistogramMIImageFilter :
    public otb::BinaryFunctorNeighborhoodJoinHistogramImageFilter<
            TInputImage1,TInputImage2,TOutputImage,
            Functor::JoinHistogramMI<
                   typename itk::ConstNeighborhoodIterator<TInputImage1>, 
                   typename itk::ConstNeighborhoodIterator<TInputImage2>,
		   typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef JoinHistogramMIImageFilter  Self;
  typedef typename otb::BinaryFunctorNeighborhoodJoinHistogramImageFilter<
      TInputImage1,TInputImage2,TOutputImage, 
          Functor::JoinHistogramMI< 
               typename itk::ConstNeighborhoodIterator<TInputImage1>, 
               typename itk::ConstNeighborhoodIterator<TInputImage2>,
               typename TOutputImage::PixelType>   
  >  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  JoinHistogramMIImageFilter() {}
  virtual ~JoinHistogramMIImageFilter() {}

private:
  JoinHistogramMIImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
