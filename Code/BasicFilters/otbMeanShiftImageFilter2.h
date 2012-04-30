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
#ifndef __otbMeanShiftImageFilter2_h
#define __otbMeanShiftImageFilter2_h

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageToImageFilter.h"
#include <vcl_algorithm.h>


namespace otb
{

/** \class SpatialRangeJointDomainTransform
 *
 *
 * Functor returning the joint spatial-range representation of a pixel, i.e. the
 * concatenation of range components and coordinates as a vector. Components are
 * scaled by their respective spatial and range bandwidth.
 */

template<class TInputImage, class TOutputJointImage>
class SpatialRangeJointDomainTransform
{
public:
  typedef double RealType;

  SpatialRangeJointDomainTransform() {}
  ~SpatialRangeJointDomainTransform() {}

  inline typename TOutputJointImage::PixelType operator()
  (const typename TInputImage::PixelType & inputPixel, const typename TInputImage::IndexType & index)
  {
    typename TOutputJointImage::PixelType jointPixel;
    jointPixel.SetSize(ImageDimension + m_NumberOfComponentsPerPixel);

    for(unsigned int comp = 0; comp < ImageDimension; comp++)
      {
      jointPixel[comp] = index[comp] / m_SpatialBandwidth;
      }
    for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel; comp++)
      {
      jointPixel[ImageDimension + comp] = inputPixel[comp] / m_RangeBandwidth;
      }
    return jointPixel;
  }

  void Initialize(unsigned int _ImageDimension, unsigned int _m_NumberOfComponentsPerPixel,
                  RealType _m_SpatialBandwidth, RealType _m_RangeBandwidth)
  {
    ImageDimension = _ImageDimension;
    m_NumberOfComponentsPerPixel = _m_NumberOfComponentsPerPixel;
    m_SpatialBandwidth = _m_SpatialBandwidth;
    m_RangeBandwidth = _m_RangeBandwidth;
    m_OutputSize = ImageDimension + m_NumberOfComponentsPerPixel;
  }

  unsigned int GetOutputSize()
  {
    return m_OutputSize;
  }

private:
  unsigned int ImageDimension;
  unsigned int m_NumberOfComponentsPerPixel;
  unsigned int m_OutputSize;
  RealType     m_SpatialBandwidth;
  RealType     m_RangeBandwidth;
};


class KernelUniform
{
public:
  typedef double RealType;

  KernelUniform() {
    SetBandwidth(1.0);
  }

  ~KernelUniform() {}
  inline RealType operator() (RealType x) {
    return (x >= -m_Bandwidth || x <= m_Bandwidth) ? m_KernelUniformValue : 0.0;
  }

  RealType GetRadius() {
    return m_Radius;
  }

  RealType GetBandwidth() {
    return m_Bandwidth;
  }

  void SetBandwidth(RealType bw) {
    m_Bandwidth = bw;
    m_KernelUniformValue = 1.0 / (2.0 * m_Bandwidth);
    m_Radius = m_Bandwidth;
  }

private:
  RealType m_Radius;
  RealType m_Bandwidth;
  RealType m_KernelUniformValue;
};

class NormL2
{
};

/** \class MeanShiftImageFilter2
 *
 *
 * Mean shift is a data clustering algorithm often used in image processing and segmentation.
 * For a given pixel, the mean shift will build a set of neighboring pixels within a given spatial
 * bandwidth (can be set using SetSpatialBandwidth()) and a spectral range (can be set using SetRangeBandwidth()).
 * The spatial and spectral center of this set is then computed and the algorithm iterates with this new spatial
 * and spectral center.
 *
 * Mean shift can be used for edge-preserving smoothing, or for clustering. The GetOutput() method
 * return concatenation of spatial and spectral meanshift filtered data GetSpatialOutput() and GetSpectralOutput() gives
 * resp. spatial and Spectral filtering parts
 *
 *
 * GetMetricOutput() method gives mean shift vector
 * GetIterationOutput() returns the number of iterations performed for each pixel.
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
 * \sa MeanShiftImageFilter
 *
 * \ingroup ImageEnhancement
 */
template <class TInputImage, class TOutputImage, class TKernel = KernelUniform, class TNorm = NormL2, class TOutputMetricImage = TOutputImage, class TOutputIterationImage = otb::Image<unsigned int, TInputImage::ImageDimension> >
class ITK_EXPORT MeanShiftImageFilter2
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedef */
  typedef MeanShiftImageFilter2                              Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;
  typedef double                                             RealType;

  /** Type macro */
  itkTypeMacro(MeanShiftImageFilter2, ImageToImageFilter);
  itkNewMacro(Self);

  /** Template parameters typedefs */

  typedef TInputImage                             InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename InputImageType::IndexType      InputIndexType;
  typedef typename InputImageType::SizeType       InputSizeType;
  typedef typename InputImageType::IndexValueType InputIndexValueType;
  typedef typename InputImageType::PointType      PointType;
  typedef typename InputImageType::RegionType     RegionType;
  typedef typename InputImageType::SizeType       SizeType;

  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::PixelType   OutputPixelType;
  typedef typename OutputImageType::RegionType  OutputRegionType;

  typedef TOutputMetricImage                          OutputMetricImageType;
  typedef typename OutputMetricImageType::Pointer     OutputMetricImagePointerType;
  typedef typename OutputMetricImageType::PixelType   OutputMetricPixelType;
  typedef typename OutputMetricImageType::RegionType  MetricRegionType;

  typedef TOutputIterationImage                       OutputIterationImageType;

  typedef otb::VectorImage<RealType, InputImageType::ImageDimension> OutputSpatialImageType;
  typedef typename OutputSpatialImageType::Pointer                   OutputSpatialImagePointerType;
  typedef typename OutputSpatialImageType::PixelType                 OutputSpatialPixelType;

  typedef TKernel KernelType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  typedef itk::VariableLengthVector<RealType>                        RealVector;
  typedef otb::VectorImage<RealType, InputImageType::ImageDimension> RealVectorImageType;
  typedef otb::Image<unsigned short, InputImageType::ImageDimension> ModeTableImageType;

  /** Setters / Getters */
  itkSetMacro(SpatialBandwidth, RealType);
  itkGetMacro(SpatialBandwidth, RealType);
  itkSetMacro(RangeBandwidth, RealType);
  itkGetMacro(RangeBandwidth, RealType);

  itkGetConstMacro(MaxIterationNumber, unsigned int);
  itkSetMacro(MaxIterationNumber, unsigned int);

  itkGetConstMacro(Threshold, double);
  itkSetMacro(Threshold, double);

  itkSetMacro(ModeSearchOptimization, bool);
  itkGetConstMacro(ModeSearchOptimization, bool);

  /** Returns the const spatial image output */
  const OutputSpatialImageType * GetSpatialOutput() const;
  /** Returns the spectral image output */
  const OutputImageType * GetRangeOutput() const;
  /** Returns the mean shift vector computed at the last iteration for each pixel */
  const OutputMetricImageType * GetMetricOutput() const;
  /** Returns the number of iterations done at each pixel */
  const OutputIterationImageType * GetIterationOutput() const;

  /** Returns the const spatial image output */
  OutputSpatialImageType * GetSpatialOutput();
   /** Returns the spectral image output */
  OutputImageType * GetRangeOutput();
  /** Returns the mean shift vector computed at the last iteration for each pixel */
  OutputMetricImageType * GetMetricOutput();
  /** Returns the number of iterations done at each pixel */
  OutputIterationImageType * GetIterationOutput();

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
  void ThreadedGenerateData(const OutputRegionType& outputRegionForThread,
                               int threadId );

  virtual void AfterThreadedGenerateData();

  /** Allocates the outputs (need to be reimplemented since outputs have differents type) */
  virtual void AllocateOutputs();

  /** Constructor */
  MeanShiftImageFilter2();

  /** Destructor */
  virtual ~MeanShiftImageFilter2();

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void CalculateMeanShiftVector(typename RealVectorImageType::Pointer inputImagePtr,
                                        RealVector jointPixel, const OutputRegionType& outputRegion,
                                        RealVector & meanShiftVector);

private:
  MeanShiftImageFilter2(const Self &); //purposely not implemented
  void operator =(const Self&);             //purposely not implemented

  /** Range bandwidth */
  RealType        m_RangeBandwidth;

  /** Spatial bandwidth */
  RealType      m_SpatialBandwidth;

  /** Radius of pixel neighborhood, determined by the kernel from the spatial bandwidth  */
  InputSizeType      m_SpatialRadius;

  /** Threshold on the squared norm of the mean shift vector to decide when to stop iterating **/
  double m_Threshold;

  /** Maximum number of iterations **/
  unsigned int m_MaxIterationNumber;

  /** KernelType to be defined **/
  KernelType m_SpatialKernel;
  KernelType m_RangeKernel;

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

  /** Boolean to enable mode search optimization */
  bool m_ModeSearchOptimization;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftImageFilter2.txx"
#endif

#endif
