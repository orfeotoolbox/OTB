/*=========================================================================
Program: ITK nSIFT Implemention (Template Source)
Module: $RCSfile: itkScaleInvariantFeatureImageFilter.txx,v $
Language: C++
Date: $Date: 2007/11/25 15:51:48 $
Version: $Revision: 1.0 $
Copyright (c) 2005,2006,2007 Warren Cheung
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* The name of the Insight Consortium, nor the names of any consortium members,
nor of any contributors, may be used to endorse or promote products derived
from this software without specific prior written permission.
* Modified source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=========================================================================*/

//#define VERBOSE
//#define DEBUG
//#define DEBUG_VERBOSE

//#define GENERATE_KEYS 
//#define REORIENT

//#define SIFT_FEATURE

//TODO:  Migrate the histogram reorientation code to the SIFT feature
// Make histogram feature special case of the sift feature with
// a single histogram.
//REORIENT NSIFT:  Keep a "global histogram" in addition to the specific ones.
// Or maybe do the global histogram/reorient phase first?

// Granularity of the Histogram causing cycles in reorientation accuracy?
// Or maybe we need to shift all the angles by half a bin width?

//Generate histograms
//compare gradient histograms
//

// Min DoG value for keypoints
// Fix Gaussian Scale
// Only iterate through the region up to 3 sigma from the point

//More advanced features for better matching?
//Simple quadrant system?


/* Based on example code from the ITK Toolkit
 * TODO:  Use resampler+identity transform+spacing change instead of scaling
 * arbitrary downscale between pyramid levels
 * may need to have a threshold
 * Gaussian filtration may fail if input image format is integer
 * Generate Pointset
 * Get Orientation
 * Generate Features

 * Test vs Noise (no need to for orientation)
 * vs Stretch 
 * vs Scale
 */
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkScaleInvariantFeatureImageFilter.h"
#include <cstdio>
#include "vnl/vnl_math.h"

#ifndef SIFTKEY_CLASS
#define SIFTKEY_CLASS


const float PI=3.14159265;

namespace itk
{  

  template <class TFixedImageType, unsigned int VDimension> 

  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::ScaleInvariantFeatureImageFilter() 
  {
#ifdef DO_DOUBLE
    m_ImageScalesTestedNumber = 4;
#else
    m_ImageScalesTestedNumber = 3;
#endif
    m_ScalingFactor = 2.0;
    m_DifferenceOfGaussianTestsNumber = 3;
#ifdef DO_DOUBLE
    m_DoubleOriginalImage = true;
#else
    m_DoubleOriginalImage = false;
#endif
    m_HistogramBinsNumber = 36;      
    m_ErrorThreshold = 0.0;
    m_MaxFeatureDistanceRatio = 0.8;
    m_GaussianSigma = 1.5;  
    m_MinKeypointValue = 0.0075;
    m_SIFTHalfWidth = 8;  // This MUST be a multiple of m_SIFTSubfeatureWidth
    m_SIFTSubfeatureWidth = 4;
    m_SIFTSubfeatureBins = 8;
    
    // Derived from above
    m_DifferenceOfGaussianImagesNumber = m_DifferenceOfGaussianTestsNumber+2;
    m_GaussianImagesNumber = m_DifferenceOfGaussianImagesNumber+1;
    m_IdentityTransform = IdentityTransformType::New();
  }

    
  template <class TFixedImageType, unsigned int VDimension> 
  double
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::GetGaussianScale( int j ) 
  {
    return (vcl_pow(2, (double) j / (double) m_DifferenceOfGaussianTestsNumber) * m_GaussianSigma);
  }

  template <class TFixedImageType, unsigned int VDimension> 
  unsigned int
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::HistFeatureSize() 
  {
    unsigned int size = 1;
    // Have m_HistogramBinsNumber for each of the (VDimension-1) Orientation dimensions
    for (unsigned int i = 1; i < VDimension; ++i)
      {
	size *= m_HistogramBinsNumber;
      }
    return size;
  }

  template <class TFixedImageType, unsigned int VDimension> 
  typename ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>::GradientImageType::Pointer
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::GetHypersphericalCoordinates(typename GradientImageType::Pointer inputImg) 
  {
    typedef itk::ImageRegionIteratorWithIndex< GradientImageType > ImageIteratorType;
    typedef itk::ImageRegionConstIteratorWithIndex< GradientImageType > ConstImageIteratorType;
    typedef itk::ImageRegionConstIteratorWithIndex< TFixedImageType > ConstFixedImageIteratorType;
    typename GradientImageType::Pointer outputImg = GradientImageType::New();
    // Copy attributes
    outputImg->SetRegions(inputImg->GetLargestPossibleRegion());
    outputImg->CopyInformation( inputImg );
    outputImg->Allocate();

    ConstImageIteratorType inputIt(inputImg, inputImg->GetLargestPossibleRegion());
    ImageIteratorType outputIt(outputImg, inputImg->GetLargestPossibleRegion());
      
    for ( inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();
	  ++inputIt, ++outputIt)
      {
	typename GradientImageType::PixelType x =  inputIt.Get();
	typename GradientImageType::PixelType p;

	// position 0 is the norm
	p[0] = x.GetNorm();

	// position 1 is arctan (x0 / x1)
	p[1] = atan2( x[0],x[1] );

	// Iterate over all the positions
	// position k  is arctan (x_k-1 / (x_k * cos p_k))	  
	for (unsigned int k = 2; k < x.Size(); ++k)
	  {
	    p[k] = atan2( x[k-1], x[k] * cos(p[k-1]));
	  }
	outputIt.Set(p);
      }

    return outputImg;
  }

  // Generates a vector with positions 1 to VDimension-1 filled with
  // histogram bin numbers from a single bin number

  template <class TFixedImageType, unsigned int VDimension> 
  typename ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>::GradientImageType::PixelType
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::BinToVector (unsigned int maxbin) 
  {
    // convert maxpos to orientation bin vector
    typename GradientImageType::PixelType maxp;
    for (unsigned int i = 1; i < VDimension; ++i) {
      maxp[i] = maxbin % m_HistogramBinsNumber;
      maxbin /= m_HistogramBinsNumber;
    }
    return maxp;
  }           

  template <class TFixedImageType, unsigned int VDimension> 
  unsigned int
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::SiftFeatureSize() 
  {
    unsigned int size = 1;
    // Have m_HistogramBinsNumber for each of the (VDimension-1) Orientation dimensions
    for (unsigned int i = 0; i < VDimension; ++i)
      {
	size *= (m_SIFTHalfWidth * 2 / m_SIFTSubfeatureWidth );
	if (i > 0)
	  size *= m_SIFTSubfeatureBins;
      }
    
    return size;
  }
  
  // Convert the delta iterator into index to the
  // start of the SIFT histogram
  template <class TFixedImageType, unsigned int VDimension> 
  unsigned int
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::DeltaToSiftIndex (int delta[]) 
  {
    unsigned int bin = 0;
    unsigned int binpos = 1;
#ifdef DEBUG_VERBOSE
    std::cerr << "Converting delta: ";
#endif
    for (unsigned int i = 0; i < VDimension; ++i) {
#ifdef DEBUG_VERBOSE
      std::cerr << delta[i];
#endif
      unsigned int tmp =  (delta[i] + m_SIFTHalfWidth) / m_SIFTSubfeatureWidth;
      
      bin += tmp * binpos;
      binpos *= (m_SIFTHalfWidth * 2 / m_SIFTSubfeatureWidth );
    }
    for (unsigned int i = 1; i < VDimension; ++i)
      bin *= m_SIFTSubfeatureBins;
    
#ifdef DEBUG_VERBOSE
    std::cerr << "\n";
#endif
    return bin;
  }


#ifdef GENERATE_KEYS
  template <class TFixedImageType, unsigned int VDimension> 
  typename ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>::FeatureType
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::GetSiftKey(typename GradientImageType::Pointer inputImg,
	       FixedImagePointer multImg,
	       IndexType pixelIndex) 
  {
#ifdef DEBUG_VERBOSE
    std::cerr << "GetSiftKey..." << std::endl; 
#endif
    FeatureType sifthistogram(this->SiftFeatureSize());
    sifthistogram.Fill(0);

    // delta iterates from  -m_SIFTHalfWidth to m_SIFTHalfWidth-1 
    // in each dimensions
    int delta[VDimension];
    for (unsigned int k = 0; k < VDimension; ++k) {
      delta[k] = -m_SIFTHalfWidth;
    }

    typename GradientImageType::SizeType regionSize = 
      inputImg->GetLargestPossibleRegion().GetSize();	
    
    while(1) {
      unsigned int siftbin = this->DeltaToSiftIndex(delta);
      
#ifdef DEBUG_VERBOSE
      std::cerr << "Siftbin:" << siftbin << std::endl; 
#endif
      
      // Get pixel index
      // Clamp to image edges
      IndexType tmpIndex;
      for (unsigned int k=0; k < VDimension; ++k) {
	if ((pixelIndex[k] + delta[k]) < 0) {
	  tmpIndex[k] = 0;
	} else {
	  tmpIndex[k] = pixelIndex[k] + delta[k];
	  if (static_cast<unsigned int>(tmpIndex[k]) >= regionSize[k] )
	    tmpIndex[k] = regionSize[k]-1;
	}
      }
      
#ifdef DEBUG_VERBOSE
      std::cerr << "Pixel:" << tmpIndex << std::endl; 
#endif
      typename GradientImageType::PixelType x = 
	inputImg->GetPixel(tmpIndex);
      
      // Get histogram bin
      // Iterate over all the positions
      unsigned int bin = 0;
      unsigned int binpos = 1;
      for (unsigned int k = 1; k < x.Size(); ++k)
	{
	  // Rescale from -PI to PI ->  0 to m_HistogramBinsNumber-1
	  float p;
	  p = (x[k] + PI)  * (float) m_SIFTSubfeatureBins / (2.0 * PI);
	  
	  
	  if (p < 0 || p >= m_SIFTSubfeatureBins) 
	    p = 0;
	  bin += (unsigned int) p * binpos;
	  
#ifdef DEBUG_VERBOSE
	  std::cout << " " << p;
#endif
	  binpos *= m_SIFTSubfeatureBins;
	}
      
      bin += siftbin;
      
      // Fill Sift Index bin
      if (bin > this->SiftFeatureSize()) {
	// VERY BAD
	std::cerr << bin << " > " << this->SiftFeatureSize() << " Warning -- Overload2\n";
      }
      sifthistogram[bin] += x[0] * multImg->GetPixel(tmpIndex);
      
#ifdef DEBUG_VERBOSE
      std::cerr << "Incrementing\n";
#endif	  
      // Increment delta
      bool resetdelta=false;
      for(unsigned int k=0; k <= VDimension; ++k) {
#ifdef DEBUG_VERBOSE
	std::cerr << delta[k];
#endif
	if (k == VDimension) {
#ifdef DEBUG_VERBOSE
	  std::cerr << "done\n";
#endif
	  resetdelta = true;
	  break; // done
	}
	// Don't want to go past m_SIFTHalfWidth-1
	if (++delta[k] < (int) m_SIFTHalfWidth) {
	  break;
	}
	delta[k] = -m_SIFTHalfWidth; // reset and increment the next pos
      }
      if(resetdelta) break;	  
    }
    
#ifdef DEBUG_VERBOSE
    std::cerr << "SIFT key: " << sifthistogram << "\n";
#endif
    return(sifthistogram);
  }
  

  // Takes a hyperspherical coordinate gradient and gaussian weights
  // returns a histogram 
  // Each orientation divides the 2PI angles into m_HistogramBinsNumber 
  // The value in each bin is the weighted magnitude
  template <class TFixedImageType, unsigned int VDimension> 
  typename ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>::FeatureType
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::GetHistogram(typename GradientImageType::Pointer inputImg,
		 FixedImagePointer multImg) 
  {
    
#ifdef ERROR_CHECK
    std::cerr << "GetHistogram ... ";
#endif
    
    FeatureType histogram(this->HistFeatureSize());

    histogram.Fill(0);
    
    typedef itk::ImageRegionConstIteratorWithIndex< GradientImageType > ConstImageIteratorType;
    typedef itk::ImageRegionConstIteratorWithIndex< TFixedImageType > ConstFixedImageIteratorType;
    
    ConstImageIteratorType inputIt(inputImg, inputImg->GetLargestPossibleRegion());
    ConstFixedImageIteratorType multIt( multImg, inputImg->GetLargestPossibleRegion());
      
    for ( inputIt.GoToBegin(), multIt.GoToBegin(); !inputIt.IsAtEnd();
	  ++inputIt, ++multIt)
      {
	typename GradientImageType::PixelType x =  inputIt.Get();
	typename TFixedImageType::PixelType m = multIt.Get();
	unsigned int bin = 0;
	unsigned int binpos = 1;
	typename GradientImageType::PixelType p;
	
	// position 0 is the norm
	p[0] = x[0];
	
	if (vnl_math_isnan(p[0]) || (p[0] == 0.0))
	  continue;
	
	// multiply by m
	p[0] *= m;
	
#ifdef DEBUG_VERBOSE
	std::cout << "Bin: ";
#endif	  
	// Iterate over all the positions
	for (unsigned int k = 1; k < x.Size(); ++k)
	  {
	    // Rescale from -PI to PI ->  0 to m_HistogramBinsNumber-1
	    p[k] = (x[k] + PI)  * (float) m_HistogramBinsNumber / (2.0 * PI);
	    
	    
	    if (p[k] < 0 || p[k] >= m_HistogramBinsNumber) 
	      p[k] = 0;
	    bin += (unsigned int) p[k] * binpos;
	    
#ifdef DEBUG_VERBOSE
	    std::cout << " " << p[k];
#endif
	    binpos *= m_HistogramBinsNumber;
	  }
#ifdef DEBUG_VERBOSE
	std::cout << " Value: " << p[0] << std::endl;
#endif
	if (bin > this->HistFeatureSize()) {
	  // VERY BAD
	  std::cerr << x << " -> " << p << "\n";
	  std::cerr << bin << " > " << this->HistFeatureSize() << " Warning -- Overload2\n";
	}
	histogram[bin] += p[0];
      }
#ifdef DEBUG
    // Print the Histogram
    std::cout << histogram << std::endl;
#endif
    
    // Since we are going to use this as a feature
    // Normalise
    float hmag = 0.0;
#ifdef REORIENT
    float maxmag = -1;
    unsigned int maxbin;
#endif
    for (unsigned int i = 0; i < this->HistFeatureSize(); ++i) {
      float mag = histogram[i]*histogram[i];
      hmag += mag;
#ifdef REORIENT
      if (maxmag < 0 || mag > maxmag) {
	maxmag = mag;
	maxbin = i;
      }
#endif
    }
    hmag = sqrt(hmag);
    
#ifdef REORIENT
    typename GradientImageType::PixelType maxp = this->BinToVector(maxbin);
    
    FeatureType histogram2(this->HistFeatureSize());
    histogram2.Fill(0);
#endif
    
    for (unsigned int i = 0; i < this->HistFeatureSize(); ++i) {
      histogram[i] /= hmag;
      
#ifdef REORIENT
      typename GradientImageType::PixelType bini = this->BinToVector(i);
      
      unsigned int bin = 0;
      unsigned int binpos = 1;
      for (unsigned int k = 1; k < VDimension; ++k) {
	bini[k] = ((int) (bini[k] - maxp[k] + m_HistogramBinsNumber)) % m_HistogramBinsNumber;
	if (bini[k] < 0 || bini[k] >= m_HistogramBinsNumber)
	  bini[k] = 0;
	bin += (int) bini[k] * binpos;
	binpos *= m_HistogramBinsNumber;
      }
      histogram2[bin] = histogram[i];
#endif
    }
    
    
#ifdef DEBUG
    std::cout << histogram << std::endl;
#ifdef REORIENT
    std::cout << "Max Bin: " << maxbin << " Max Mag: " << maxmag << std::endl;
    std::cout << "Reoriented: " << histogram2 << std::endl;
#endif
#endif
#ifdef ERROR_CHECK
    std::cerr << "OK\n ";
#endif
    
#ifdef REORIENT
    return histogram2;
#else
    return histogram;
#endif
  }
#endif

  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::CheckLocalExtrema(FixedImagePointer image, IndexType pixelIndex,
		      PixelType pixelValue,
		      bool &isMax, bool &isMin,
		      bool checkCentre)
  {
#ifdef ERROR_CHECK
    std::cerr << "CheckLocalExtrema ... ";
#endif
    int delta[VDimension];
    for (unsigned int k = 0; k < VDimension; ++k) {
      delta[k] = -1;
    }
    while(1) {
      bool isZero=true;
      if (!checkCentre) {
	for (unsigned int k=0; k < VDimension; ++k) {
	  if(delta[k] != 0) {
	    isZero = false;
	    break;
	  }
	}
      }
      
      if (checkCentre || !isZero) {
	// Check if not the centre
	IndexType tmpIndex;
	for (unsigned int k=0; k < VDimension; ++k) {
	  tmpIndex[k] = pixelIndex[k] + delta[k];
	}
	
	typename TFixedImageType::PixelType tmpValue = 
	  image->GetPixel(tmpIndex);
	
#ifdef DEBUG_VERBOSE
	std::cout << "...Comparing to ( ";
	for (unsigned int k = 0; k < VDimension; ++k)
	  std::cout << tmpIndex[k] << " ";
	std::cout << ") = " << tmpValue << "\n";
#endif  
	// Treat as equality if within the error bound
	if (((tmpValue - pixelValue) <= m_ErrorThreshold) && 
	    ((tmpValue - pixelValue) >= -m_ErrorThreshold)) {
	  isMax = false;
	  isMin = false;
	} else	
	  if (tmpValue > pixelValue) {
	    isMax = false;
	  } else
	    if (tmpValue < pixelValue) {
	      isMin = false;
	    }
	if (!isMax && !isMin) break;
      }
      // Increment delta
      bool resetdelta=false;
      for(unsigned int k=0; k <= VDimension; ++k) {
	if (k == VDimension) {
	  resetdelta = true;
	  break; // done
	}
	if (delta[k] < 1) {
	  ++delta[k];
	  break;
	}
	delta[k] = -1; // reset and increment the next pos
      }
      if(resetdelta) break;	  
    }  
#ifdef ERROR_CHECK
    std::cerr << "OK\n";
#endif
  }
  
#ifdef GENERATE_KEYS
  // Input:  Image, Gradient Image, Point
  // Output:  Vector of direction
  template <class TFixedImageType, unsigned int VDimension> 
  typename ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>::FeatureType
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::GetFeatures( FixedImagePointer fixedImage, 
		 typename GradientImageType::Pointer hgradImage,
		 PointType &point, float currScale) 
  {
#ifdef ERROR_CHECK
    std::cerr << "GetFeatures ... ";
#endif
    
    // Generate the Gaussian
    typedef GaussianImageSource<TFixedImageType> GaussianImageSourceType;
    typename GaussianImageSourceType::Pointer gaussImgSource;
    typename GaussianImageSourceType::ArrayType sigma;

    gaussImgSource = GaussianImageSourceType::New();
    gaussImgSource->SetNormalized(true);
    //gaussImgSource->SetNormalized(false);
    //gaussImgSource->SetScale(255);  
    gaussImgSource->SetSpacing(fixedImage->GetSpacing());
    gaussImgSource->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
    gaussImgSource->SetMean(point);
    
#if 0
    // If we wanted to find the orientation,  we would use this
    for (unsigned int i = 0; i < VDimension; ++i)
      sigma[i] = 3.0 * currScale;
    gaussImgSource->SetSigma(sigma);
#endif
    
    // Simulate the 16x16 Gaussian window descriptor
    // use sigma equal to half the descriptor window width
    for (unsigned int i = 0; i < VDimension; ++i)
      sigma[i] = 8.0;
    gaussImgSource->SetSigma(sigma);
    
    gaussImgSource->Update();
    
    IndexType pixelIndex;
    fixedImage->TransformPhysicalPointToIndex(point, pixelIndex);
    
#if 0
    // Only iterate through the region that is within 3 sigma of the mean
    
    IndexType regionStart;
    for (unsigned int k=0; k < VDimension; ++k)
      {
	if ((pixelIndex[k] - 3*sigma[k]) > 0)
	  regionStart[k] = (int) floor(pixelIndex[k] - 3*sigma[k]);
	else 
	  regionStart[k] = 0;
      }
      
    typename TFixedImageType::SizeType regionSize = 
      fixedImage->GetLargestPossibleRegion().GetSize();
    
    // Avoid far edge
    for (unsigned int k=0; k < VDimension; ++k) {
      if ( ceil(regionStart[k] + 6*sigma[k]) < regionSize[k])
	regionSize[k] =  (int) ceil(6*sigma[k]);
      else
	regionSize[k] -=  regionStart[k];
    }
    
    typename TFixedImageType::RegionType itregion;
    itregion.SetIndex(regionStart);
    itregion.SetSize(regionSize);
#endif
    
    // return the Gaussian weighted Histogram
#ifdef SIFT_FEATURE
    return this->GetSiftKey(hgradImage, gaussImgSource->GetOutput(),
		      pixelIndex);
#else
    return this->GetHistogram(hgradImage, gaussImgSource->GetOutput());
#endif
  }
  
#endif

  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::writeImage(FixedImagePointer fixedImage, const char *filename)
  {
    
    if (VDimension != 2) return;
    
    typedef itk::Image< unsigned char, VDimension >  OutImageType;
    typedef typename itk::ImageFileWriter< OutImageType  >  FixedWriterType;
    typedef  itk::ResampleImageFilter< TFixedImageType,  OutImageType    >    
      OutResampleFilterType;
    
    
    typename OutResampleFilterType::Pointer resampler = 
      OutResampleFilterType::New();
    
    resampler->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputSpacing( fixedImage->GetSpacing() );
    resampler->SetTransform(m_IdentityTransform);
    resampler->SetInput(fixedImage);
    
    typename FixedWriterType::Pointer fixedWriter = FixedWriterType::New();
    
    fixedWriter->SetFileName(filename);
    fixedWriter->SetInput( resampler->GetOutput() );
	
    std::cout << "[Writing file << " << filename << "]";
    
    try 
      {
	fixedWriter->Update();
      }
    catch( itk::ExceptionObject & excep )
      {
	std::cerr << "Exception caught !" << std::endl;
	std::cerr << excep << std::endl;
      }

  }
  
  // create a filter that resamples the image (scale up or down) 
  template <class TFixedImageType, unsigned int VDimension> 
  typename ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>::ResampleFilterPointerType
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::getScaleResampleFilter ( FixedImagePointer fixedImage, float scale )
  {
    ResampleFilterPointerType scaler = ResampleFilterType::New();
    
    scaler->SetInput( fixedImage );
    
	
    // Change the size of the image
    typename TFixedImageType::SizeType size = 
      fixedImage->GetLargestPossibleRegion().GetSize();
    for (unsigned int k = 0; k < VDimension; ++k)
      size[k] = (unsigned int) floor(size[k] * scale);
    scaler->SetSize( size );

    // Change the spacing
    typename TFixedImageType::SpacingType spacing = 
      fixedImage->GetSpacing();
    for (unsigned int k = 0; k < VDimension; ++k)
      spacing[k] = (spacing[k] / scale);
    scaler->SetOutputSpacing( spacing );

    scaler->SetTransform( m_IdentityTransform );
    scaler->SetDefaultPixelValue( 100 );
    scaler->Update();
    
    return scaler;
  }
  
  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::SetDoubling (bool tmp) 
  {
    m_DoubleOriginalImage = tmp;
  }

  template <class TFixedImageType, unsigned int VDimension>
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::SetNumBins( unsigned int tmp) 
  {
    m_HistogramBinsNumber = tmp;
  }

  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::SetSigma( double tmp) 
  {
    m_GaussianSigma = tmp;
  }      

  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::SetNumScales ( unsigned int tmp) 
  {
    m_ImageScalesTestedNumber = tmp;
  }
      
  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::SetMatchRatio ( float tmp) 
  {
     m_MaxFeatureDistanceRatio = tmp;
  }


  template <class TFixedImageType, unsigned int VDimension> 
  typename ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>::PointSetTypePointer
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::getSiftFeatures(FixedImagePointer fixedImage) 
  {
    unsigned int numMin = 0, numMax = 0, numReject = 0;
    
    m_KeypointSet = PointSetType::New();

    m_PointsCount = 0;      

    // Declare Gaussian 
    typedef itk::DiscreteGaussianImageFilter<TFixedImageType, TFixedImageType > 
      GaussianFilterType;

    std::vector<typename GaussianFilterType::Pointer> gaussianFilter(m_GaussianImagesNumber);
	
    std::vector<typename TFixedImageType::Pointer>
      gaussianImage(m_GaussianImagesNumber);

    // Declare DoG 
    typedef itk::SubtractImageFilter<TFixedImageType, TFixedImageType, 
      TFixedImageType> DifferenceFilterType;
    std::vector<typename DifferenceFilterType::Pointer> dogFilter(m_DifferenceOfGaussianImagesNumber);
    std::vector<typename TFixedImageType::Pointer> dogImage(m_DifferenceOfGaussianImagesNumber);

    // Resampled image filters
    std::vector<typename ResampleFilterType::Pointer> scaler(m_ImageScalesTestedNumber);
    std::vector<typename TFixedImageType::Pointer> scaleImage(m_ImageScalesTestedNumber);

#ifdef GENERATE_KEYS
    // Declare Gradient
    std::vector<typename GradientFilterType::Pointer> gradFilter(m_ImageScalesTestedNumber);
    std::vector<typename GradientImageType::Pointer> gradImage(m_ImageScalesTestedNumber);
    std::vector<typename GradientImageType::Pointer> hgradImage(m_ImageScalesTestedNumber);

    std::vector<typename GradientMagFilterType::Pointer> gradMagFilter(m_ImageScalesTestedNumber);
    std::vector<FixedImagePointer> gradMagImage(m_ImageScalesTestedNumber);
#endif


    float currScale = 0.5;

    // For each scale
    for (unsigned int i = 0; i < m_ImageScalesTestedNumber; ++i) {
      std::cout << "Computing Scale Level " << i << "... (";

      if (i == 0 && !m_DoubleOriginalImage) {
	scaleImage[0] = fixedImage;
      } else {
	if (i == 0) {
	  // Input is the fixed Image.  
	  scaler[i] = getScaleResampleFilter ( fixedImage, m_ScalingFactor );
	} else {
	  // Input is the 2*sigma smoothed image from the previous octave
	  scaler[i] = getScaleResampleFilter ( gaussianImage[m_DifferenceOfGaussianTestsNumber] , 1.0 / m_ScalingFactor );
	}
	scaleImage[i] = scaler[i]->GetOutput();
      }

      {
	typename TFixedImageType::SizeType gsize = 
	  scaleImage[i]->GetLargestPossibleRegion().GetSize();
	for (unsigned int j = 0; j < VDimension; ++j)
	  std::cout << gsize[j] << " ";
      }
    
      std::cout << ") Done\n";

#ifdef DEBUG
      char filename[256];
      sprintf(filename, "gauss-%d-0.png", i);
      this->writeImage(scaleImage[i], filename);
#endif

#ifdef GENERATE_KEYS
      // ...Compute Gradient
      std::cout << "...Computing Gradient...";
      gradFilter[i] = GradientFilterType::New();
      gradFilter[i]->SetInput(scaleImage[i]);
      // Do this in pixel space
      gradFilter[i]->SetUseImageSpacing(false);
      gradFilter[i]->Update();
      gradImage[i] = gradFilter[i]->GetOutput();
      hgradImage[i] = this->GetHypersphericalCoordinates(gradImage[i]);

      gradMagFilter[i] = GradientMagFilterType::New();
      gradMagFilter[i]->SetInput(scaleImage[i]);
      // Do this in pixel space
      gradMagFilter[i]->SetUseImageSpacing(false);
      gradMagFilter[i]->Update();
      gradMagImage[i] = gradMagFilter[i]->GetOutput();
      std::cout << "...Done\n";
#endif

      // ...Compute Gaussians
      for (unsigned int j = 0; j < m_GaussianImagesNumber; ++j) {
#ifdef VERBOSE
	std::cout << "Setting Up Gaussian Filter " << i << "-" << j << "...";
	std::cout.flush();
#endif
	gaussianFilter[j] = GaussianFilterType::New();

	/* Variance is square of the sigma
	 * sigma = (2^(j/s)*sigma)
	 */

	double variance = this->GetGaussianScale(j);
	variance *= variance;
	gaussianFilter[j]->SetVariance(variance);
	gaussianFilter[j]->SetInput( scaleImage[i] );
	// pixel-wise smoothing
	gaussianFilter[j]->SetUseImageSpacing(false); 
	try {
	  gaussianFilter[j]->Update();
	}
	catch( itk::ExceptionObject & excep ) {
	  std::cerr << "Exception caught !" << std::endl;
	  std::cerr << excep << std::endl;
	}

	gaussianImage[j] = gaussianFilter[j]->GetOutput();

#ifdef DEBUG
	char filename[256];
	sprintf(filename, "gauss-%d-%d.png", i, j);
	this->writeImage(gaussianImage[j], filename);
#endif

#ifdef VERBOSE
	std::cout << "Done\n";
	std::cout.flush();
#endif
      }
    
      // ...Compute Difference of Gaussians
      for (unsigned int j = 0; j < (m_DifferenceOfGaussianImagesNumber); ++j) {
#ifdef VERBOSE
	std::cout << "Setting Up DoG Filter " << i << "-" << j << "...";
	std::cout.flush();
#endif
	dogFilter[j] = DifferenceFilterType::New();
	dogFilter[j]->SetInput1( gaussianImage[j] );
	dogFilter[j]->SetInput2( gaussianImage[j+1] );
	dogFilter[j]->Update();
	dogImage[j] = dogFilter[j]->GetOutput();

#ifdef DEBUG
	char filename[256];
	sprintf(filename, "dog-%d-%d.png", i, j);
	this->writeImage(dogImage[j], filename);
#endif

#ifdef VERBOSE
	std::cout << "Done\n";
	std::cout.flush();
#endif 
      }

      for (unsigned int j=1; j < (m_DifferenceOfGaussianImagesNumber - 1); ++j) {
	// Search the dogImages for local maxima,  w.r.t. corresponding
	// point in the scale above and below
	// level 0 is the "doubled" image
	// Iterate over the various doG filters
	// Only use the middle dogs (ones with both neighbours above and below)
	// Iterate over each position in the dog filter
	typedef itk::ImageRegionIteratorWithIndex< TFixedImageType > 
	  ImageIteratorType;

	IndexType regionStart;
	// Avoid the edges
	for (unsigned int k=0; k < VDimension; ++k)
	  regionStart[k] = 1;

	typename TFixedImageType::SizeType regionSize = 
	  dogImage[j]->GetLargestPossibleRegion().GetSize();

#ifdef VERBOSE
	std::cout << "Searching for Extrema in DoG Image " << i << "-" << j;
	std::cout << " ( ";
	for (unsigned int k=0; k < VDimension; ++k)
	  std::cout << regionSize[k] << " ";
	std::cout << ") Scale " << currScale << "\n";
	std::cout.flush();
#endif

	// Avoid far edge
	for (unsigned int k=0; k < VDimension; ++k)
	  regionSize[k] -=  2;
	  
	typename TFixedImageType::RegionType itregion;
	itregion.SetIndex(regionStart);
	itregion.SetSize(regionSize);
      
	ImageIteratorType pixelIt(dogImage[j],
				  itregion);
      
	for ( pixelIt.GoToBegin(); !pixelIt.IsAtEnd(); ++pixelIt) {
	  // Make sure to start sufficiently into the image so that all
	  // neighbours are present
	  IndexType pixelIndex = pixelIt.GetIndex();
	  typename TFixedImageType::PixelType pixelValue = pixelIt.Get();

	  PointType point;
	  dogImage[j]->TransformIndexToPhysicalPoint (pixelIndex, point);

#ifdef ERROR_CHECK
	  std::cerr << "Checking ( ";
	  for (unsigned int k = 0; k < VDimension; ++k)
	    std::cerr << pixelIndex[k] << " ";
	  std::cerr << ") = " << pixelValue <<"\n";
#endif

	  // Compare to the 8 immediate neighbours
	  bool isMax=true;
	  bool isMin=true;

	  this->CheckLocalExtrema(dogImage[j], 
			    pixelIndex, pixelValue, isMax, isMin, false);

	  if (!isMax && !isMin) continue;
	  
	  // Compare to scale above
	  if (j < (m_GaussianImagesNumber-1)) {
#ifdef DEBUG_VERBOSE
	    std::cout << "...Checking Scale Above\n";
#endif
	    //dogImage[i+1][j]->TransformPhysicalPointToIndex (point, tmpIndex);

	    this->CheckLocalExtrema(dogImage[j+1], 
			      pixelIndex, pixelValue, isMax, isMin, true);
	  }
	  if (!isMax && !isMin) continue;

	  // Compare to scale below
	  if (j > 0) {
#ifdef DEBUG_VERBOSE
	    std::cout << "...Checking Scale Below\n";
#endif
	    //dogImage[i-1][j]->TransformPhysicalPointToIndex (point, tmpIndex);

	    this->CheckLocalExtrema(dogImage[j-1], 
			      pixelIndex, pixelValue, isMax, isMin, true);
	  }
	  if (!isMax && !isMin) continue;
	    
	  // Check if it is sufficiently large (absolute value)
	  if (fabs(pixelValue) < m_MinKeypointValue) {
	    ++numReject;
	    continue;
	  }


	  // Passed all checks:

#ifdef DEBUG
	  std::cout << point << std::endl;
#endif
	  m_KeypointSet->SetPoint( m_PointsCount, point);
#ifdef GENERATE_KEYS
	  // Generate features
	  // Space used is the (smoothed) original image)
	  m_KeypointSet->SetPointData( m_PointsCount, 
				   this->GetFeatures( gaussianImage[0], 
						      hgradImage[i], point,
						      this->GetGaussianScale(j)));
#else
	  m_KeypointSet->SetPointData( m_PointsCount, currScale);
#endif
	  ++m_PointsCount;

	  if (isMax) {
	    // Maxima detected.  
	    ++numMax;
#ifdef DEBUG
	    std::cout << "Found Maxima! ";
#endif
	  }
	  if (isMin) {
	    // Minima detected.  
	    ++numMin;
#ifdef DEBUG
	    std::cout << "Found Minima! ";
#endif
	  }
	}
#ifdef VERBOSE
	std::cout << "Acc. Num Max: " << numMax 
		  << "\nAcc. Num Min: " << numMin 
		  << "\nAcc. Num Reject: " << numReject 
		  << std::endl;
	std::cout.flush();
#endif
      }
      currScale *= m_ScalingFactor;
    }
	
#ifdef VERBOSE
    std::cout << "Total Num Max: " << numMax 
	      << "\nTotal Num Min: " << numMin 
	      << "\nTotal Num Reject: " << numReject
	      << std::endl;	
    std::cout.flush();
#endif
    return m_KeypointSet;
  }

  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::MatchKeypointsPos(PointSetTypePointer keypoints1, PointSetTypePointer keypoints2,
		      typename TransformType::Pointer inverse_transform)
  {
    // Compare Keypoints.  Check Coverage and Accuracy
    // This does the comparison based on position of the keypoints
    // Find:  
    // # of points that match which will tell us
    // # of points that did not scale
    // # of points created by the scale
    unsigned int numMatches;
    unsigned int numMatches2;
    unsigned int numMatches5;
    const double MATCH_THRESHOLD = 1.5;
    typename PointSetType::PointsContainer::Pointer keyps1, keyps2;

    unsigned long numpoints1, numpoints2;
    numpoints1 = keypoints1->GetNumberOfPoints();
    std::cout << "Keypoints1 Found: " << numpoints1 << std::endl;
    numpoints2 = keypoints2->GetNumberOfPoints();
    std::cout << "Keypoints2 Found: " << numpoints2 << std::endl;

    if (!inverse_transform)
      return;

    numMatches = 0;
    numMatches2 = 0;
    numMatches5 = 0;
    for (unsigned int i = 0; i < numpoints2; ++i) {
      PointType pp2;
      keypoints2->GetPoint(i, &pp2);	
	
      bool match = false;
      bool match2 = false;
      bool match5 = false;
      for (unsigned int j = 0; j < numpoints1; ++j) {
	PointType tmpp, pp;
	keypoints1->GetPoint(j, &tmpp);
	pp = inverse_transform->TransformPoint(tmpp);
	//	    for (int k = 0; k < VDimension; ++k)
	//	      pp[k] *= scale_test;	      
	if(!match  && pp.EuclideanDistanceTo(pp2) <= MATCH_THRESHOLD)
	  {
	    ++numMatches;
	    match = true;
	  }
	if(!match2 && pp.EuclideanDistanceTo(pp2) <= 2*MATCH_THRESHOLD)
	  {
	    ++numMatches2;
	    match2 = true;
	  }
	if(!match5 && pp.EuclideanDistanceTo(pp2) <= 5*MATCH_THRESHOLD)
	  {
	    ++numMatches5;
	    match5 = true;
	  }
	if (match && match2 && match5)
	  break;
      }      
    }

    std::cout << "Keypoints 2 Matching to Keypoints 1 (<" << MATCH_THRESHOLD << "): " << numMatches << std::endl;
    std::cout << "% of Keypoints 2 Matching (<" << MATCH_THRESHOLD << "):  " << (float) numMatches / numpoints2 << std::endl;
    std::cout << "Keypoints 2 Matching to Keypoints 1 (<" << 2*MATCH_THRESHOLD << "): " << numMatches2 << std::endl;
    std::cout << "% of Keypoints 2 Matching (<" << 2*MATCH_THRESHOLD << "):  " << (float) numMatches2 / numpoints2 << std::endl;
    std::cout << "Keypoints 2 Matching to Keypoints 1 (<" << 5*MATCH_THRESHOLD << "): " << numMatches5 << std::endl;
    std::cout << "% of Keypoints 2 Matching (<" << 5*MATCH_THRESHOLD << "):  " << (float) numMatches5 / numpoints2 << std::endl;


    numMatches = 0;
    numMatches2 = 0;
    numMatches5 = 0;
    for (unsigned int j = 0; j < numpoints1; ++j) {
      PointType tmpp, pp;
      keypoints1->GetPoint(j, &tmpp);
      pp = inverse_transform->TransformPoint(tmpp);
      //	  for (int k = 0; k < VDimension; ++k)
      //	    pp[k] *= scale_test;

      bool match = false;
      bool match2 = false;
      bool match5 = false;	
      for (unsigned int i = 0; i < numpoints2; ++i) {
	PointType pp2;
	keypoints2->GetPoint(i, &pp2);	

	if(!match  && pp.EuclideanDistanceTo(pp2) <= MATCH_THRESHOLD)
	  {
	    ++numMatches;
	    match = true;
	  }
	if(!match2 && pp.EuclideanDistanceTo(pp2) <= 2*MATCH_THRESHOLD)
	  {
	    ++numMatches2;
	    match2 = true;
	  }
	if(!match5 && pp.EuclideanDistanceTo(pp2) <= 5*MATCH_THRESHOLD)
	  {
	    ++numMatches5;
	    match5 = true;
	  }
	if (match && match2 && match5)
	  break;
      }      
    }

    std::cout << "Keypoints 1 Matching to Keypoints 2 (<" << MATCH_THRESHOLD << "): " << numMatches << std::endl;
    std::cout << "% of Keypoints 1 Matching (<" << MATCH_THRESHOLD << "):  " << (float) numMatches / numpoints1 << std::endl;
    std::cout << "Keypoints 1 Matching to Keypoints 2 (<" << 2*MATCH_THRESHOLD << "): " << numMatches2 << std::endl;
    std::cout << "% of Keypoints 1 Matching (<" << 2*MATCH_THRESHOLD << "):  " << (float) numMatches2 / numpoints1 << std::endl;
    std::cout << "Keypoints 1 Matching to Keypoints 2 (<" << 5*MATCH_THRESHOLD << "): " << numMatches5 << std::endl;
    std::cout << "% of Keypoints 1 Matching (<" << 5*MATCH_THRESHOLD << "):  " << (float) numMatches5 / numpoints1 << std::endl;
  }

#ifdef GENERATE_KEYS
  template <class TFixedImageType, unsigned int VDimension> 
  void
  ScaleInvariantFeatureImageFilter<TFixedImageType,VDimension>
  ::MatchKeypointsFeatures(PointSetTypePointer keypoints1, PointSetTypePointer keypoints2,
			   typename TransformType::Pointer inverse_transform)
  {
    // Compare Keypoints.  Check Coverage and Accuracy
    // This does the comparison based on position of the keypoints
    // Find:  
    // # of points that match which will tell us
    // # of points that did not scale
    // # of points created by the scale
    unsigned int numMatches;
    unsigned int numMatchesTried;
    unsigned int numMatches2;
    unsigned int numMatches5;
    const double MATCH_THRESHOLD = 1.5;
    typename PointSetType::PointsContainer::Pointer keyps1, keyps2;

    unsigned long numpoints1, numpoints2;
    numpoints1 = keypoints1->GetNumberOfPoints();
    std::cout << "Keypoints1 Found: " << numpoints1 << std::endl;
    numpoints2 = keypoints2->GetNumberOfPoints();
    std::cout << "Keypoints2 Found: " << numpoints2 << std::endl;

    std::cout << "***Keypoint Matches***\n";

    numMatches = 0;
    numMatches2 = 0;
    numMatches5 = 0;
    numMatchesTried = 0;
    for (unsigned int i = 0; i < numpoints2; ++i) {
      PointType pp2;
      keypoints2->GetPoint(i, &pp2);	
      FeatureType ft2;
      keypoints2->GetPointData(i, &ft2);	
	
      FeatureType bestft;
      float bestdist = -1.0;
      float nextbestdist = -1.0;
      unsigned int bestj;
      for (unsigned int j = 0; j < numpoints1; ++j) {
	PointType pp;
	keypoints1->GetPoint(j, &pp);
	FeatureType ft;
	keypoints1->GetPointData(j, &ft);	

	float dist = 0.0;
	for (unsigned int k = 0; k < ft.Size(); ++k)
	  dist += (ft[k] - ft2[k])*(ft[k] - ft2[k]);
	  
	if (nextbestdist < 0.0 || dist < bestdist)
	  {
	    nextbestdist = bestdist;
	    bestdist=dist;
	    bestft = ft;
	    bestj = j;
	  }	  
      }

      /* Reject "too close" matches */
      if ((bestdist / nextbestdist) >  m_MaxFeatureDistanceRatio)
	continue;	

      /* NEW IDEA -- look to make sure it is a reciprocal best match */
      /* Take the best feature found,  see if pp2 makes the cut */
      bestdist = -1.0;
      nextbestdist = -1.0;
      FeatureType bestft2;
      unsigned int bestj2;

      for (unsigned int j = 0; j < numpoints2; ++j) {
	PointType pp;
	keypoints2->GetPoint(j, &pp);
	FeatureType ft;
	keypoints2->GetPointData(j, &ft);	

	float dist = 0.0;
	for (int unsigned k = 0; k < ft.Size(); ++k)
	  dist += (ft[k] - bestft[k])*(ft[k] - bestft[k]);
	  
	if (nextbestdist < 0.0 || dist < bestdist)
	  {
	    nextbestdist = bestdist;
	    bestdist=dist;
	    bestft2 = ft;
	    bestj2 = j;
	  }	  
      }

      /* Reject if not reciprocal best hit or "too close" matches */
      if ( bestft2 != ft2 || ((bestdist / nextbestdist) >  m_MaxFeatureDistanceRatio))
	continue;	
      /* END NEW IDEA */

      ++numMatchesTried;

      // Check goodness of best feature
      PointType tmpp, pp;
      keypoints1->GetPoint(bestj, &tmpp);

      // Print the match
      std::cout << tmpp << " => " << pp2 << std::endl;

      if (!inverse_transform)
	continue;
      pp = inverse_transform->TransformPoint(tmpp);

      if(pp.EuclideanDistanceTo(pp2) <= MATCH_THRESHOLD)
	++numMatches;
      if(pp.EuclideanDistanceTo(pp2) <= (2*MATCH_THRESHOLD))
	++numMatches2;
      if(pp.EuclideanDistanceTo(pp2) <= (5*MATCH_THRESHOLD))
	++numMatches5;
    }

    std::cout << "\n***Features 2 Matches Attempted: " << numMatchesTried << std::endl;
    std::cout << "Features 2 Matching to Features 1 (<" << MATCH_THRESHOLD << "): " << numMatches << std::endl;
    std::cout << "% of Features 2 Matching (<" << MATCH_THRESHOLD << "):  " << (float) numMatches / numMatchesTried << std::endl;
    std::cout << "Features 2 Matching to Features 1 (<" << 2*MATCH_THRESHOLD << "): " << numMatches2 << std::endl;
    std::cout << "% of Features 2 Matching (<" << 2*MATCH_THRESHOLD << "):  " << (float) numMatches2 / numMatchesTried << std::endl;
    std::cout << "Features 2 Matching to Features 1 (<" << 5*MATCH_THRESHOLD << "): " << numMatches5 << std::endl;
    std::cout << "% of Features 2 Matching (<" << 5*MATCH_THRESHOLD << "):  " << (float) numMatches5 / numMatchesTried << std::endl;

  }
#endif

} // end namespace itk

#endif
