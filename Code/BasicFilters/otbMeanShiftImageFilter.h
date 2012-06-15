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
#ifndef __otbMeanShiftImageFilter_h
#define __otbMeanShiftImageFilter_h

#include "vcl_deprecated_header.h"

#include "itkImageToImageFilter.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"
#include "otbObjectList.h"
#include "otbPolygon.h"

namespace otb
{
namespace MeanShift
{
/** \class ScalarBufferConverter
 *  \brief Small utilities class used for buffer conversion with EDISON standard.
 *   This class handles buffer conversion when pixel type is scalar.
 **/
class ScalarBufferConverter
{
public:
  /**
   * Fill the pixel with the float array values at index, using nbComp values scaled by scale.
   */
  template <class TPixel>
  static inline void
  FloatArrayToPixel(const float * data, unsigned int index, TPixel& pixel,
                    const unsigned int /*nbComp*/, double scale)
  {
    pixel = static_cast<TPixel>(scale * data[index]);
  }
  /**
   * Fill the float array data with the pixel values at index, using nbComp values scaled by scale.
   */
  template <class TPixel>
  static inline void
  PixelToFloatArray(float * data, unsigned int index, const TPixel& pixel, double scale)
  {
    data[index] = static_cast<float>(scale * pixel);
  }
};
}

/** \class MeanShiftImageFilter
 *
 *
 * Mean shift is a data clustering algorithm often used in image processing and segmentation.
 * For a given pixel, the mean shift will build a set of neighboring pixels within a given spatial
 * radius (can be set using SetSpatialRadius()) and a color range (can be set using SetRangeRadius()).
 * The spatial and color center of this set is then computed and the algorithm iterates with this new spatial
 * and color center.
 *
 * Mean shift can be used for edge-preserving smoothing, or for clustering. The GetOutput() method will allow
 * you to get the smoothed image, whereas the
 * GetClusteredOutput() methods returns the clustered output. The GetLabeledClusteredOutput() returns
 * a labeled clustered image, and the GetClusterBoundariesOutput()
 * an image of the cluster boundaries.
 *
 * The MinimumRegionSize parameter allows you to prune small clustered regions.
 *
 * This filter uses the Edison mean shift algorithm implementation. Please note that data whose precision
 * is more than float are casted to float before processing.
 *
 * The Scale parameter allows you to stretch the data dynamic
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
 * \sa MeanShiftVectorImageFilter
 *
 * \ingroup ImageEnhancement
 */

template <class TInputImage, class TOutputImage,
    class TLabeledOutput = otb::Image<unsigned short, 2>,
    class TBufferConverter = MeanShift::ScalarBufferConverter>
class ITK_EXPORT MeanShiftImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedef */
  typedef MeanShiftImageFilter                               Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkTypeMacro(MeanShiftImageFilter, ImageToImageFilter);
  itkNewMacro(Self);

  /** Template parameters typedefs */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointerType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef typename InputImageType::PointType    PointType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::PixelType   OutputPixelType;
  typedef typename OutputImageType::RegionType  RegionType;
  typedef TLabeledOutput                        LabeledOutputType;
  typedef typename LabeledOutputType::Pointer   LabeledOutputPointerType;
  typedef typename LabeledOutputType::PixelType LabelType;

  /** Typedefs for vectorized output */
  typedef otb::Polygon<InputPixelType>      PolygonType;
  typedef typename PolygonType::Pointer     PolygonPointerType;
  typedef otb::ObjectList<PolygonType>      PolygonListType;
  typedef typename PolygonListType::Pointer PolygonListPointerType;

  /** Typedef for mean-shift modes map */
  typedef std::map<LabelType, InputPixelType> ModeMapType;

  /** Setters / Getters */
  itkSetMacro(SpatialRadius, unsigned int);
  itkGetMacro(SpatialRadius, unsigned int);
  itkSetMacro(RangeRadius, double);
  itkGetMacro(RangeRadius, double);
  itkSetMacro(MinimumRegionSize, unsigned int);
  itkGetMacro(MinimumRegionSize, unsigned int);
  itkSetMacro(Scale, double);
  itkGetMacro(Scale, double);

  /** Return the const clustered image output */
  const OutputImageType * GetClusteredOutput() const;
  /** Return the clustered image output */
  OutputImageType * GetClusteredOutput();
  /** Return the const labeled clustered image output */
  const LabeledOutputType * GetLabeledClusteredOutput() const;
  /** Return the labeled clustered image output */
  LabeledOutputType * GetLabeledClusteredOutput();
  /** Return the const cluster boundaries image output */
  const LabeledOutputType * GetClusterBoundariesOutput() const;
  /** Return the cluster boundaries image output */
  LabeledOutputType * GetClusterBoundariesOutput();
  /** Return the mean-shift mode by label */
  const ModeMapType& GetModes()
  {
    return m_Modes;
  }

protected:
   virtual void EnlargeOutputRequestedRegion( itk::DataObject *output );

  virtual void GenerateData();

  /** Allocate the outputs (need to be reimplemented since outputs have differents type) */
  virtual void AllocateOutputs();
  /** If modified, we have to reset the list of modes */
  virtual void Modified() const
  {
    Superclass::Modified();
    m_Modes.clear();
  }

  /** Constructor */
  MeanShiftImageFilter();
  /** destructor */
  virtual ~MeanShiftImageFilter() {}

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  MeanShiftImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&);             //purposely not implemented

  /** Spatial radius for mean shift convergence */
  unsigned int m_SpatialRadius;
  /** Range radius for mean shift convergence */
  double m_RangeRadius;
  /** Minimum region size in pixels for clustering */
  unsigned int m_MinimumRegionSize;
  /** Data scale (used to stretch data range) */
  double m_Scale;
  /** A map of the different modes by segmented regions */
  mutable ModeMapType m_Modes;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftImageFilter.txx"
#endif

#endif
