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
#ifndef __MeanShiftSmoothingImageFilter_h
#define __MeanShiftSmoothingImageFilter_h

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include <vcl_algorithm.h>


namespace otb
{
namespace Meanshift
{

template<typename T> inline T simple_pow(T const& v, unsigned int p)
{
  T res = 1;
  for (unsigned int i = 0; i != p; ++i)
    {
    res *= v;
    }
  return res;
}

/** \class SpatialRangeJointDomainTransform
 *
 *
 * Functor returning the joint spatial-range representation of a pixel, i.e. the
 * concatenation of range components and coordinates as a vector. Components are
 * scaled by their respective spatial and range bandwidth.
 *
 * \ingroup OTBSmoothing
 */
template<class TInputImage, class TOutputJointImage>
class SpatialRangeJointDomainTransform
{
public:
  typedef double RealType;

  SpatialRangeJointDomainTransform()
  {
  }
  // ~SpatialRangeJointDomainTransform() {}

  typename TOutputJointImage::PixelType operator()(const typename TInputImage::PixelType & inputPixel,
                                                   const typename TInputImage::IndexType & index) const
  {
    typename TOutputJointImage::PixelType jointPixel(m_ImageDimension + m_NumberOfComponentsPerPixel);

    for (unsigned int comp = 0; comp < m_ImageDimension; comp++)
      {
      jointPixel[comp] = index[comp] + m_GlobalShift[comp];
      }
    for (unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
      {
      jointPixel[m_ImageDimension + comp] = inputPixel[comp];
      }
    return jointPixel;
  }

  void Initialize(unsigned int _ImageDimension, unsigned int numberOfComponentsPerPixel_, typename TInputImage::IndexType globalShift_)
  {
    m_ImageDimension = _ImageDimension;
    m_NumberOfComponentsPerPixel = numberOfComponentsPerPixel_;
    m_OutputSize = m_ImageDimension + m_NumberOfComponentsPerPixel;
    m_GlobalShift = globalShift_;
  }

  unsigned int GetOutputSize() const
  {
    return m_OutputSize;
  }

private:
  unsigned int m_ImageDimension;
  unsigned int m_NumberOfComponentsPerPixel;
  unsigned int m_OutputSize;
  typename TInputImage::IndexType m_GlobalShift;
};

class KernelUniform
{
public:
  typedef double RealType;

  // KernelUniform() {}
  // ~KernelUniform() {}

  RealType operator()(RealType x) const
  {
    return (x <= 1) ? 1.0 : 0.0;
  }

  RealType GetRadius(RealType bandwidth) const
  {
    return bandwidth;
  }
};

class KernelGaussian
{
public:
  typedef double RealType;

  // KernelGaussian() {}
  // ~KernelGaussian() {}

  RealType operator()(RealType x) const
  {
    return vcl_exp(-0.5 * x);
  }

  RealType GetRadius(RealType bandwidth) const
  {
    return 3.0 * bandwidth;
  }
};

/** \class FastImageRegionConstIterator
 *
 * Iterator for reading pixels over an image region, specialized for faster
 * access to pixels in vector images through the method GetPixelPointer
 *
 * \ingroup OTBSmoothing
 */
template<typename TImage>
class FastImageRegionConstIterator: public itk::ImageRegionConstIterator<TImage>
{
public:
  /** Standard class typedef. */
  typedef FastImageRegionConstIterator<TImage> Self;
  typedef itk::ImageRegionConstIterator<TImage> Superclass;

  typedef typename Superclass::ImageType ImageType;
  typedef typename Superclass::RegionType RegionType;

  typedef typename TImage::PixelType PixelType;
  typedef typename TImage::InternalPixelType InternalPixelType;

  itkTypeMacro(FastImageRegionConstIterator, ImageRegionConstIterator)
;

  FastImageRegionConstIterator() :
    Superclass()
  {
  }
  FastImageRegionConstIterator(const ImageType *ptr, const RegionType &region) :
    Superclass(ptr, region)
  {
    m_NumberOfComponentsPerPixel = ptr->GetNumberOfComponentsPerPixel();
  }

  const InternalPixelType * GetPixelPointer() const
  {
    return this->m_Buffer + (this->m_Offset * m_NumberOfComponentsPerPixel);
  }

private:
  unsigned int m_NumberOfComponentsPerPixel;
};

#if 0 //disable bucket mode
/** \class BucketImage
 *
 * This class indexes pixels in a N-dimensional image into a N+1-dimensional
 * array of buckets. Each pixel is stored into one bucket depending on its
 * position on the lattice (the width of a bucket is given at construction) and
 * one spectral component (also given at construction by spectralCoordinate).
 *
 * The (spatially and spectrally) neighboring buckets of pixels can then be
 * obtained by using GetNeighborhoodBucketListIndices().
 *
 * \ingroup OTBSmoothing
 */
template<class TImage>
class BucketImage
{
public:
  typedef TImage ImageType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;
  typedef typename ImageType::PixelType PixelType;
  typedef typename ImageType::InternalPixelType InternalPixelType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::IndexType IndexType;

  typedef double RealType;

  static const unsigned int ImageDimension = ImageType::ImageDimension;

  /** The bucket image has dimension N+1 (ie. usually 3D for most images) */
  typedef std::vector<typename ImageType::SizeType::SizeValueType> BucketImageSizeType;
  typedef std::vector<typename ImageType::IndexType::IndexValueType> BucketImageIndexType;
  //typedef std::vector<long> BucketImageIndexType;


  /** pixel buckets typedefs and declarations */
  typedef const typename ImageType::InternalPixelType * ImageDataPointerType;
  typedef std::vector<ImageDataPointerType> BucketType;
  typedef std::vector<BucketType> BucketListType;

  BucketImage()
  {
  }

  /** Constructor for the bucket image. It operates on the specified
   * region.
   * spatialRadius specifies the width of a bucket in pixels.
   * rangeRadius is the spectral width for the specified spectral coordinate in
   * one bucket.
   * spectralCoordinate is the index of the pixel used for classification in buckets.
   */
  BucketImage(ImageConstPointerType image, const RegionType & region, RealType spatialRadius, RealType rangeRadius,
              unsigned int spectralCoordinate) :
    m_Image(image), m_Region(region), m_SpatialRadius(spatialRadius), m_RangeRadius(rangeRadius),
        m_SpectralCoordinate(spectralCoordinate)
  {

    // Find max and min of the used spectral band
    itk::ImageRegionConstIterator<ImageType> inputIt(m_Image, m_Region);
    inputIt.GoToBegin();
    InternalPixelType minValue = inputIt.Get()[spectralCoordinate];
    InternalPixelType maxValue = minValue;
    ++inputIt;
    while (!inputIt.IsAtEnd())
      {
      const PixelType &p = inputIt.Get();
      minValue = vcl_min(minValue, p[m_SpectralCoordinate]);
      maxValue = vcl_max(maxValue, p[m_SpectralCoordinate]);
      ++inputIt;
      }

    m_MinValue = minValue;
    m_MaxValue = maxValue;

    // Compute bucket image dimensions. Note: empty buckets are at each border
    // to simplify image border issues
    m_DimensionVector.resize(ImageDimension + 1); // NB: pays for a 0-innit
    for (unsigned int dim = 0; dim < ImageDimension; ++dim)
      {
      m_DimensionVector[dim] = m_Region.GetSize()[dim] / m_SpatialRadius + 3;
      }
    m_DimensionVector[ImageDimension] = (unsigned int) ((maxValue - minValue) / m_RangeRadius) + 3;

    unsigned int numBuckets = m_DimensionVector[0];
    for (unsigned int dim = 1; dim <= ImageDimension; ++dim)
      numBuckets *= m_DimensionVector[dim];

    m_BucketList.resize(numBuckets);
    // Build buckets
    itk::ImageRegionConstIteratorWithIndex<ImageType> it(m_Image, m_Region);
    it.GoToBegin();
    // this iterator is only used to get the pixel data pointer
    FastImageRegionConstIterator<ImageType> fastIt(m_Image, m_Region);
    fastIt.GoToBegin();

    while (!it.IsAtEnd())
      {
      const IndexType & index = it.GetIndex();
      const PixelType & pixel = it.Get();

      // Find which bucket this pixel belongs to
      const BucketImageIndexType bucketIndex = GetBucketIndex(pixel, index);

      unsigned int bucketListIndex = BucketIndexToBucketListIndex(bucketIndex);
      assert(bucketListIndex < numBuckets);
      m_BucketList[bucketListIndex].push_back(fastIt.GetPixelPointer());
      ++it;
      ++fastIt;
      }

    // Prepare neighborhood offset vector
    // BucketImageIndexType zeroOffsetIndex(ImageDimension+1);
    std::vector<BucketImageIndexType> neighborsIndexList;
    neighborsIndexList.reserve(simple_pow(3, ImageDimension + 1));
    neighborsIndexList.resize(1, BucketImageIndexType(ImageDimension + 1)); // zeroOffsetIndex
    // neighborsIndexList.push_back(zeroOffsetIndex);
    for (unsigned dim = 0; dim <= ImageDimension; ++dim)
      {
      // take all neighbors already in the list and add their direct neighbor
      // along the current dim
      const unsigned int curSize = neighborsIndexList.size();
      for (unsigned int i = 0; i < curSize; ++i)
        {
        BucketImageIndexType index = neighborsIndexList[i];
        index[dim]--;
        neighborsIndexList.push_back(index);
        index[dim] += 2;
        neighborsIndexList.push_back(index);
        }
      }
    // Convert all neighbors n-dimensional indices to bucket list 1D indices
    const unsigned int neighborhoodOffsetVectorSize = neighborsIndexList.size();
    m_NeighborhoodOffsetVector.reserve(neighborhoodOffsetVectorSize);
    for (unsigned int i = 0; i < neighborhoodOffsetVectorSize; ++i)
      {
      const int listIndex = BucketIndexToBucketListIndex(neighborsIndexList[i]);
      m_NeighborhoodOffsetVector.push_back(listIndex);
      }
  }

  ~BucketImage()
  {
  }

  /** Returns the N+1-dimensional bucket index for a given pixel value at the given index */
  BucketImageIndexType GetBucketIndex(const PixelType & pixel, const IndexType & index)
  {
    BucketImageIndexType bucketIndex(ImageDimension + 1);
    for (unsigned int dim = 0; dim < ImageDimension; ++dim)
      {
      bucketIndex[dim] = (index[dim] - m_Region.GetIndex()[dim]) / m_SpatialRadius + 1;
      }
    bucketIndex[ImageDimension] = (pixel[m_SpectralCoordinate] - m_MinValue) / m_RangeRadius + 1;
    return bucketIndex;
  }

  /** Converts a N+1-dimensional bucket index into the 1D list index useable by
   GetBucket() */
  int BucketIndexToBucketListIndex(const BucketImageIndexType & bucketIndex) const
  {
    int bucketListIndex = bucketIndex[0];
    for (unsigned int dim = 1; dim <= ImageDimension; ++dim)
      {
      bucketListIndex = bucketListIndex * m_DimensionVector[dim] + bucketIndex[dim];
      }
    return bucketListIndex;
  }

  /** Retrieves the list of all buckets in the neighborhood of the given bucket */
  std::vector<unsigned int> GetNeighborhoodBucketListIndices(int bucketIndex) const
  {
    const unsigned int neighborhoodOffsetVectorSize = m_NeighborhoodOffsetVector.size();
    std::vector<unsigned int> indices(neighborhoodOffsetVectorSize);

    for (unsigned int i = 0; i < neighborhoodOffsetVectorSize; ++i)
      {
      indices[i] = bucketIndex + m_NeighborhoodOffsetVector[i];
      }
    return indices;
  }

  /* Returns the list of pixels (actually pointer to pixel data) contained in a bucket */
  const BucketType & GetBucket(unsigned int index) const
  {
    return m_BucketList[index];
  }

  unsigned int GetNumberOfNeighborBuckets() const
  {
    return m_NeighborhoodOffsetVector.size();
  }

private:
  /** Input image */
  ImageConstPointerType m_Image;
  /** Processed region */
  RegionType m_Region;

  /** Spatial radius of one bucket of pixels */
  RealType m_SpatialRadius;
  /** Range radius (at a single dimension) of one bucket of pixels */
  RealType m_RangeRadius;
  /** pixels are separated in buckets depending on their spatial position and
   also their value at one coordinate */
  unsigned int m_SpectralCoordinate;
  /** Min and Max of selected spectral coordinate */
  InternalPixelType m_MinValue;
  InternalPixelType m_MaxValue;

  /** the buckets are stored in this list */
  BucketListType m_BucketList;
  /** This vector holds the dimensions of the 3D (ND?) bucket image */
  BucketImageSizeType m_DimensionVector;
  /** Vector of offsets in the buckets list to get all buckets in the
   * neighborhood
   */
  std::vector<int> m_NeighborhoodOffsetVector;
};
#endif

} // end namespace Meanshift

/** \class MeanShiftSmoothingImageFilter
 *
 *
 * Mean shift is an edge-preserving smoothing algorithm often used in image
 * processing and segmentation. It will iteratively smooth a given pixel with
 * its neighbors that are within a spatial distance (set using
 * SetSpatialBandwidth()) and within a spectral range (set using
 * SetRangeBandwidth()). The resulting filtered image can be retrieved by
 * GetOutput() or GetRangeOutput(). Parameter SetRangeBandwidthRamp()
 * allows linearly adapting the range bandwidth to the intensity of
 * each channel if set greater than 0 (default value is 0).
 *
 * There are additional output images, as explained below.
 * Internally, the algorithm will iteratively update a pixel both in position
 * and spectral value, with respect to its neighbors, until convergence to a
 * local mode. The map of the distance traveled by pixels is obtained by
 * GetSpatialOutput(). A map of detected local modes is also available in
 * GetLabelOutput() and can be seen as a first segmentation of the input image,
 * although usually highly oversegmented.
 * Finally, GetIterationOutput() will return the number of algorithm iterations
 * for each pixel.
 *
 * The class template parameter TKernel allows one to choose how pixels in the
 * spatial and spectral neighborhood of a given pixel participate in the
 * smoothed result. By default, a uniform kernel is used (KernelUniform), giving
 * an equal weight to all neighbor pixels. KernelGaussian can also be used,
 * although the computation time is significantly higher. The TKernel class
 * should define operator(), taking a squared norm as parameter and returning a
 * real value between 0 and 1. It should also define GetRadius(), converting the
 * spatial bandwidth parameter to the spatial radius defining how many pixels
 * are in the processing window local to a pixel.
 *
 * MeanShifVector squared norm is compared with Threshold (set using Get/Set accessor) to define pixel convergence (1e-3 by default).
 * MaxIterationNumber defines maximum iteration number for each pixel convergence (set using Get/Set accessor). Set to 4 by default.
 * ModeSearch is a boolean value, to choose between optimized and non optimized algorithm. If set to true (by default), assign mode value to each pixel on a path covered in convergence steps.
 *
 * For more information on mean shift techniques, one might consider reading the following article:
 *
 * D. Comaniciu, P. Meer, "Mean Shift: A Robust Approach Toward Feature Space Analysis," IEEE Transactions on
 * Pattern Analysis and Machine Intelligence, vol. 24, no. 5, pp. 603-619, May, 2002
 * D. Comaniciu, P. Meer, "Robust analysis of feature spaces: color image segmentation," cvpr, p. 750, 1997
 * IEEE Computer Society Conference on Computer Vision and Pattern Recognition (CVPR'97), 1997
 * D. Comaniciu, P. Meer, "Mean Shift Analysis and Applications," iccv, p. 1197, Seventh International Conference
 * on Computer Vision (ICCV'99) - Volume 2, 1999
 *
 * \sa MeanShiftSegmentationFilter
 *
 * \ingroup ImageSegmentation
 * \ingroup ImageEnhancement
 *
 * \ingroup OTBSmoothing
 */
template<class TInputImage, class TOutputImage, class TKernel = Meanshift::KernelUniform,
    class TOutputIterationImage = otb::Image<unsigned int, TInputImage::ImageDimension> >
class ITK_EXPORT MeanShiftSmoothingImageFilter: public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedef */
  typedef MeanShiftSmoothingImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef double RealType;

  /** Type macro */
  itkTypeMacro(MeanShiftSmoothingImageFilter, ImageToImageFilter)
; itkNewMacro(Self)
;

  /** Template parameters typedefs */

  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename InputImageType::IndexType InputIndexType;
  typedef typename InputImageType::SizeType InputSizeType;
  typedef typename InputImageType::IndexValueType InputIndexValueType;
  typedef typename InputImageType::PointType PointType;
  typedef typename InputImageType::RegionType RegionType;
  typedef typename InputImageType::SizeType SizeType;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  typedef TOutputIterationImage OutputIterationImageType;

  typedef unsigned long LabelType;
  typedef otb::Image<LabelType, InputImageType::ImageDimension> OutputLabelImageType;

  typedef otb::VectorImage<RealType, InputImageType::ImageDimension> OutputSpatialImageType;
  typedef typename OutputSpatialImageType::Pointer OutputSpatialImagePointerType;
  typedef typename OutputSpatialImageType::PixelType OutputSpatialPixelType;

  typedef TKernel KernelType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  typedef itk::VariableLengthVector<RealType> RealVector;
  typedef otb::VectorImage<RealType, InputImageType::ImageDimension> RealVectorImageType;
  typedef otb::Image<unsigned short, InputImageType::ImageDimension> ModeTableImageType;

  /** Sets the spatial bandwidth (or radius in the case of a uniform kernel)
   * of the neighborhood for each pixel
   */
  itkSetMacro(SpatialBandwidth, RealType);
  itkGetConstReferenceMacro(SpatialBandwidth, RealType);

  /** Sets the spectral bandwidth (or radius for a uniform kernel) for pixels
   * to be included in the same mode
   */
  itkSetMacro(RangeBandwidth, RealType);
  itkGetConstReferenceMacro(RangeBandwidth, RealType);

   /** Sets the range bandwidth ramp. If > 0, the range bandwidth
    * will be y = RangeBandwidthRamp * x + RangeBandwidth, where x is
    * the band value. */
  itkSetMacro(RangeBandwidthRamp, RealType);
  itkGetConstReferenceMacro(RangeBandwidthRamp,RealType);

  /** Sets the maximum number of algorithm iterations */
  itkGetConstReferenceMacro(MaxIterationNumber, unsigned int);
  itkSetMacro(MaxIterationNumber, unsigned int);

  /** Sets the threshold value for the mean shift vector's squared norm,
   * under which convergence is assumed
   */
  itkGetConstReferenceMacro(Threshold, double);
  itkSetMacro(Threshold, double);

  /** Toggle mode search, which is enabled by default.
   * When off, the output label image is not available
   * Be careful, with this option, the result will slightly depend on thread number.
   */
  itkSetMacro(ModeSearch, bool);
  itkGetConstReferenceMacro(ModeSearch, bool);

#if 0
  /** Toggle bucket optimization, which is disabled by default.
   */
  itkSetMacro(BucketOptimization, bool);
  itkGetConstReferenceMacro(BucketOptimization, bool);
#endif

  /** Global shift allows tackling down numerical instabilities by
  aligning pixel indices when performing tile processing */
  itkSetMacro(GlobalShift,InputIndexType);

  /** Returns the const spatial image output,spatial image output is a displacement map (pixel position after convergence minus pixel index)  */
  const OutputSpatialImageType * GetSpatialOutput() const;
  /** Returns the const spectral image output */
  const OutputImageType * GetRangeOutput() const;
  /** Returns the const number of iterations map. */
  const OutputIterationImageType * GetIterationOutput() const;
  /** Returns the const image of region labels. This output does not have sense without mode search optimization (each label codes for one mode)*/
  const OutputLabelImageType * GetLabelOutput() const;

  /** Returns the spatial image output,spatial image output is a displacement map (pixel position after convergence minus pixel index)  */
  OutputSpatialImageType * GetSpatialOutput();
  /** Returns the spectral image output */
  OutputImageType * GetRangeOutput();
  /** Returns the number of iterations done at each pixel */
  OutputIterationImageType * GetIterationOutput();
  /** Returns the image of region labels. This output does not have sense without mode search optimization (each label codes for one mode) */
  OutputLabelImageType * GetLabelOutput();

protected:

  /** GenerateOutputInformation
   *  Define output pixel size
   *
   **/
  virtual void GenerateOutputInformation(void);

  virtual void GenerateInputRequestedRegion();

  virtual void BeforeThreadedGenerateData();

  /** MeanShiftFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  virtual void AfterThreadedGenerateData();

  /** Allocates the outputs (need to be reimplemented since outputs have differents type) */
  virtual void AllocateOutputs();

  /** Constructor */
  MeanShiftSmoothingImageFilter();

  /** Destructor */
  virtual ~MeanShiftSmoothingImageFilter();

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void CalculateMeanShiftVector(const typename RealVectorImageType::Pointer inputImagePtr,
                                        const RealVector& jointPixel, const OutputRegionType& outputRegion,
                                        const RealVector& bandwidth,
                                        RealVector& meanShiftVector);
#if 0
  virtual void CalculateMeanShiftVectorBucket(const RealVector& jointPixel, RealVector& meanShiftVector);
#endif

private:
  MeanShiftSmoothingImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Range bandwidth */
  RealType m_RangeBandwidth;

  /** Coefficient */
  RealType m_RangeBandwidthRamp;

  /** Spatial bandwidth */
  RealType m_SpatialBandwidth;

  /** Radius of pixel neighborhood, determined by the kernel from the spatial bandwidth  */
  InputSizeType m_SpatialRadius;

  /** Threshold on the squared norm of the mean shift vector to decide when to stop iterating **/
  double m_Threshold;

  /** Maximum number of iterations **/
  unsigned int m_MaxIterationNumber;

  /** Kernel object, implementing operator() which returns a weight between 0 and 1
   * depending on the squared norm given in parameter **/
  KernelType m_Kernel;

  /** Number of components per pixel in the input image */
  unsigned int m_NumberOfComponentsPerPixel;

  /** Input data in the joint spatial-range domain, scaled by the bandwidths */
  typename RealVectorImageType::Pointer m_JointImage;

  /** Image to store the status at each pixel:
   * 0 : no mode has been found yet
   * 1 : a mode has been assigned to this pixel
   * 2 : pixel is in the path of the currently processed pixel and a mode will
   *     be assigned to it
   */
  typename ModeTableImageType::Pointer m_ModeTable;

  /** Boolean to enable mode search  */
  bool m_ModeSearch;

#if 0
  /** Boolean to enable bucket optimization */
  bool m_BucketOptimization;
#endif

  /** Mode counters (local to each thread) */
  itk::VariableLengthVector<LabelType> m_NumLabels;
  /** Number of bits used to represent the threadId in the most significant bits
   of labels */
  unsigned int m_ThreadIdNumberOfBits;

#if 0
  typedef Meanshift::BucketImage<RealVectorImageType> BucketImageType;
  BucketImageType m_BucketImage;
#endif

  InputIndexType m_GlobalShift;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftSmoothingImageFilter.txx"
#endif

#endif
