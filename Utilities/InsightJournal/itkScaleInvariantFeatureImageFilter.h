/*=========================================================================
Program: ITK nSIFT Implemention (Header)
Module: $RCSfile: itkScaleInvariantFeatureImageFilter.h,v $
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

#ifndef SIFTKEY_H
#define SIFTKEY_H

#include "itkImageRegistrationMethod.h"

#include "itkLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkImage.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkAffineTransform.h"
#include "itkIdentityTransform.h"
#include "itkResampleImageFilter.h"

#include "itkDiscreteGaussianImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkGradientImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkMultiplyImageFilter.h"

#include "itkGaussianImageSource.h"

#include "itkConstNeighborhoodIterator.h"

#include "itkPointSet.h"
#include "itkVector.h"

#include <cstdio>


namespace itk
{  

/** \class ScaleInvariantFeatureImageFilter
 * \brief Generate and match scale invariant features from an image input.
 *
 * This class takes the input image and locates scale invariant features in
 * the image.  The result is a set of keypoints, each with an associated
 * vector of values (the corresponding feature).  Two sets of keypoints
 * can then be matched, to determine which points in the sets are likely
 * to correspond, by comparing feature vectors.
 *
 * Three different features can be compiled from this source using
 * preprocessor \#define commands: 
 * 
 * - default (no special \#define)
 *    - Histogram Feature
 * - \#define REORIENT 
 *    - Histogram Feature with reoriented to the highest magnitude histogram bin
 * - \#define SIFT_FEATURE
 *    - n-Dimensional adaptation of the SIFT feature.
 */

  template <class TFixedImageType, unsigned int VDimension=2> 
    class ITK_EXPORT ScaleInvariantFeatureImageFilter
    {
      public:

      /** typedefs to facilitate access to the image-related types */
      typedef typename TFixedImageType::PixelType PixelType;
      typedef typename TFixedImageType::Pointer  FixedImagePointer;
      typedef typename TFixedImageType::IndexType  IndexType;

      /** used for validating results from synthetic images */
      typedef typename itk::ScalableAffineTransform< double, VDimension > TransformType;

#ifdef GENERATE_KEYS    
      /** multidimensional histogram for the features */
      typedef Array< float > FeatureType;
      /** keypoints, storing the associated feature as well */
      typedef PointSet< FeatureType, VDimension,
      	      DefaultStaticMeshTraits< FeatureType, VDimension, VDimension, double > > PointSetType;
#else
      typedef PointSet< float, VDimension,
      	      DefaultStaticMeshTraits< FeatureType, VDimension, VDimension, double > > PointSetType;
#endif 
      typedef typename PointSetType::PointType PointType;
      typedef typename PointSetType::Pointer PointSetTypePointer;

      /** Filters for scaling and resampling images for the multiple scales */
      typedef ResampleImageFilter< TFixedImageType, TFixedImageType > ResampleFilterType;
      typedef typename ResampleFilterType::Pointer ResampleFilterPointerType;
      
      /** Constructor to set default values */
      ScaleInvariantFeatureImageFilter();

      /** Used for debugging. Writes an image to disk */
      void writeImage(FixedImagePointer fixedImage, const char *filename);
      
      /** Create a filter that resamples the image (scale up or down)  */
      ResampleFilterPointerType getScaleResampleFilter ( FixedImagePointer fixedImage, float scale );

      void SetDoubling (bool tmp);
      void SetNumBins( unsigned int tmp);
      void SetSigma( double tmp);
      void SetNumScales ( unsigned int tmp);
      void SetMatchRatio ( float tmp);

      void SetTemporaryDir(char* dir)
      { m_TemporaryDir = dir;}
      
      /** Generate and return the scale invariant keypoints and features */
      PointSetTypePointer getSiftFeatures(FixedImagePointer fixedImage);

      /** Match keypoints purely based on position.  Upper bounds
       *  the performance when matching using features. Supply
       *  the inverse transform to get performance measures.
       *  Mainly used with synthetic tests.
       */
      void MatchKeypointsPos(PointSetTypePointer keypoints1, PointSetTypePointer keypoints2,
			     typename TransformType::Pointer inverse_transform);

#ifdef GENERATE_KEYS
      /** Match keypoints based on similarity of features. Matches are printed
       *  to standard out.  Supply the inverse transform to get performance 
       *  measures.
       */
      void MatchKeypointsFeatures(PointSetTypePointer keypoints1, PointSetTypePointer keypoints2,
			     typename TransformType::Pointer inverse_transform);
#endif


      private:

      /** Filter to obtain gradient directions and magnitudes */
      typedef itk::GradientImageFilter<TFixedImageType> GradientFilterType;
      typedef typename GradientFilterType::OutputImageType GradientImageType;
      typedef itk::GradientMagnitudeImageFilter<TFixedImageType, TFixedImageType> GradientMagFilterType;

      /** Shorthand for an identity transform */
      typedef IdentityTransform< double, VDimension >  IdentityTransformType;
      typename IdentityTransformType::Pointer m_IdentityTransform;

      /** Scale up the original image by a factor of 2 before processing */
      bool m_DoubleOriginalImage;
      /** Number of image pyramid levels (scales) to test */
      unsigned int    m_ImageScalesTestedNumber;
      /** Factor by which the images are scaled between pyramid levels */
      float  m_ScalingFactor;

      /** The range of Gaussian sigma that will be tested */
      float m_GaussianSigma;  
      /** Number of Gaussian Images that will be used to sample the range of 
        * sigma values ...
        */
      unsigned int    m_GaussianImagesNumber;
      /** ... determining the number of difference of gaussian images ... */
      unsigned int    m_DifferenceOfGaussianImagesNumber;
      /** ... determining the number of sets of images we can test for 
        * feature points  
        */
      unsigned int m_DifferenceOfGaussianTestsNumber;

      /** Number of bins to cover 2pi radians
        * Note that the multidimensional histogram has this many bins 
        * for each dimension when describing the angle in hyperspherical
 	* coordinates
        */
      unsigned int m_HistogramBinsNumber;
      /** Minimum voxel intensity for a feature point */
      float m_MinKeypointValue;
      /** When looking for difference of Gaussian extrema in the images, 
        * consider a voxel to be extremal even if there are voxels 
        * that are this much more extreme.
        */
      float m_ErrorThreshold;

      /** When matching points, reject a match if the ratio: 
        * \f[	
        *   \frac{\textrm{distance to best match}}{\textrm{distance to second-best match}}
	* \f]
 	* is greater than this value
        */
      float m_MaxFeatureDistanceRatio;

      PointSetTypePointer m_KeypointSet;
      long m_PointsCount;

      /** Distance from the centre to the edge of the hypercube
        * summarised by the nSIFT feature
        */
      unsigned int m_SIFTHalfWidth;
      /** The hypercube edge length (in voxels) of the subregion summarised
        * by each multidimensional histogram of the nSIFT feature
	*/
      unsigned int m_SIFTSubfeatureWidth;
      /** Number of bin used to summarise the hyperspherical coordinates
        * of the voxels in each subregion
        */
      unsigned int m_SIFTSubfeatureBins;
  
      /** Temporary directory where image file are written */
      char * m_TemporaryDir;
      
      /** The sigma for the Gaussian filtering for the j-th image on a level
        * of the image pyramid
        */
      double GetGaussianScale( int j ); 

      /** The total size of the (reoriented) histogram feature */
      unsigned int HistFeatureSize();

      /** Returns an image where all gradients are converted to hyperspherical
        * coordinates
	*/
      typename GradientImageType::Pointer GetHypersphericalCoordinates(typename GradientImageType::Pointer inputImg);

      /** Convert a histogram bin number into an angle in hyperspherical 
        * coordinates 
	*/
      typename GradientImageType::PixelType BinToVector (unsigned int maxbin);

      /** Size of the nSIFT feature */
      unsigned int SiftFeatureSize();

      /** Used when generating nSIFT features to figure out what portion
        * of the nSIFT feature vector the current summary histogram starts at
	*/ 
      unsigned int DeltaToSiftIndex (int delta[]);

#ifdef GENERATE_KEYS
      /** Generate nSIFT feature for pixel at pixelIndex */
      FeatureType GetSiftKey(typename GradientImageType::Pointer inputImg,
			     FixedImagePointer multImg,
			     IndexType pixelIndex);


      /** Generate weighted histogram feature for voxel at pixelIndex 
        * Feature point is weighted due to the Gaussian.
        */
      FeatureType GetHistogram(typename GradientImageType::Pointer inputImg,
			       FixedImagePointer multImg);
#endif


      /** Determine whether voxel at pixelIndex with intensity
        * pixelvalue is the local max/min
        */
      void CheckLocalExtrema(FixedImagePointer image, IndexType pixelIndex,
			     PixelType pixelValue,
			     bool &isMax, bool &isMin,
			     bool checkCentre);

#ifdef GENERATE_KEYS
      /** Generate the (reoriented)histogram/nSIFT feature at a point
        */
      FeatureType GetFeatures( FixedImagePointer fixedImage, 
			       typename GradientImageType::Pointer hgradImage,
			       PointType &point, float currScale);
#endif

   };
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScaleInvariantFeatureImageFilter.txx"
#endif

#endif /* SIFTKEY_H */
