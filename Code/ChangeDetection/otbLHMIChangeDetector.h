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
#ifndef __otbLHMIChangeDetector_h
#define __otbLHMIChangeDetector_h

#include "otbBinaryFunctorNeighborhoodImageFilter.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "itkHistogram.h"


namespace otb
{
  
/** \class LHMIChangeDetector
 * \brief Implements neighborhood-wise the computation of the
 * cumulant-based approximation to mutual information.
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
 * - compute the value of the LHMI
 * - cast the \c double value resulting to the pixel type of the output image 
 * - store the casted value into the output image.
 * 
 * The filter expect all images to have the same dimension 
 * (e.g. all 2D, or all 3D, or all ND)
 * 
 * \ingroup IntensityImageFilters Multithreaded
 */

#define epsilon 0.01

namespace Functor {  

template< class TInput1, class TInput2, class TOutput>
class LHMI
{

public:

  typedef typename std::vector<TOutput> VectorType;
  typedef typename VectorType::iterator IteratorType;
  typedef typename std::vector<VectorType> VectorOfVectorType;
  typedef typename VectorOfVectorType::iterator VecOfVecIteratorType;
  typedef double HistogramFrequencyType;
  typedef typename itk::Statistics::Histogram<HistogramFrequencyType, 2> HistogramType;
  typedef typename HistogramType::MeasurementVectorType
                                           MeasurementVectorType;
  typedef typename HistogramType::SizeType    HistogramSizeType;
  typedef typename HistogramType::Iterator    HistogramIteratorType;

  
  LHMI() {};
  ~LHMI() {};
  inline TOutput operator()( const TInput1 & itA, 
                             const TInput2 & itB)
  {


    HistogramType::Pointer histogram;
    
    /** The histogram size. */
    HistogramSizeType histogramSize;
    /** The lower bound for samples in the histogram. */
    MeasurementVectorType lowerBound;
    /** The upper bound for samples in the histogram. */
    MeasurementVectorType upperBound;

    double upperBoundIncreaseFactor = 0.001;
    bool usePaddingValue = false;

    histogramSize.Fill(256);

    TOutput maxA = itA.GetPixel(0);
    TOutput minA = itA.GetPixel(0);
    TOutput maxB = itB.GetPixel(0);
    TOutput minB = itB.GetPixel(0);
    
    for(unsigned long pos = 0; pos< itA.Size(); ++pos)
      {

      TOutput value = static_cast<TOutput>(itA.GetPixel(pos));

      if(value > maxA)
	maxA = value;
      else if (value < minA)
	minA = value;

      value = static_cast<TOutput>(itB.GetPixel(pos));

      if(value > maxB)
	maxB = value;
      else if (value < minB)
	minB = value;
	

      }


      // Initialize the upper and lower bounds of the histogram.
  lowerBound[0] = minA;
  lowerBound[1] = minB;
  upperBound[0] =
    maxA + (maxA - minA ) * upperBoundIncreaseFactor;
  upperBound[1] =
    maxB + (maxB - minB ) * upperBoundIncreaseFactor;


  histogram = HistogramType::New();
    
  histogram->Initialize(histogramSize, lowerBound, upperBound);

  for(unsigned long pos = 0; pos< itA.Size(); ++pos)
    {
              
        typename HistogramType::MeasurementVectorType sample;
        sample[0] = itA.GetPixel(pos);
        sample[1] = itB.GetPixel(pos);
	/*if(sample[0]!=NumericTraits<TOutput>::Zero &&
	   sample[1]!=NumericTraits<TOutput>::Zero)*/
	  histogram->IncreaseFrequency(sample, 1);

    }



  TOutput entropyX = itk::NumericTraits<TOutput>::Zero;
  TOutput entropyY = itk::NumericTraits<TOutput>::Zero;
  TOutput jointEntropy = itk::NumericTraits<TOutput>::Zero;
  HistogramFrequencyType totalFreq = histogram->GetTotalFrequency();

    for (unsigned int i = 0; i < histogram->GetSize()[0]; i++)
    {
      HistogramFrequencyType freq = histogram->GetFrequency(i, 0);
      if (freq > 0)
      {
        entropyX += freq*log(freq);
      }
    }

    entropyX = -entropyX/static_cast<TOutput>(totalFreq) + log(totalFreq);

    for (unsigned int i = 0; i < histogram->GetSize()[1]; i++)
    {
      HistogramFrequencyType freq = histogram->GetFrequency(i, 1);
      if (freq > 0) 
      {
        entropyY += freq*log(freq);
      }
    }
    
    entropyY = -entropyY/static_cast<TOutput>(totalFreq) + log(totalFreq);

    HistogramIteratorType it = histogram->Begin();
    HistogramIteratorType end = histogram->End();
    while (it != end) 
    {
      HistogramFrequencyType freq = it.GetFrequency();
      if (freq > 0)
      {
        jointEntropy += freq*log(freq);
      }
      ++it;
    }

    jointEntropy = -jointEntropy/static_cast<TOutput>(totalFreq) +
      log(totalFreq);
    
    return static_cast<TOutput>( jointEntropy/(entropyX + entropyY) );
  }



}; 
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT LHMIChangeDetector :
    public otb::BinaryFunctorNeighborhoodImageFilter<
            TInputImage1,TInputImage2,TOutputImage,
            Functor::LHMI<
                   typename itk::ConstNeighborhoodIterator<TInputImage1>, 
                   typename itk::ConstNeighborhoodIterator<TInputImage2>,
		   typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef LHMIChangeDetector  Self;
  typedef typename otb::BinaryFunctorNeighborhoodImageFilter<
      TInputImage1,TInputImage2,TOutputImage, 
          Functor::LHMI< 
               typename itk::ConstNeighborhoodIterator<TInputImage1>, 
               typename itk::ConstNeighborhoodIterator<TInputImage2>,
               typename TOutputImage::PixelType>   
  >  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  LHMIChangeDetector() {}
  virtual ~LHMIChangeDetector() {}

private:
  LHMIChangeDetector(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
