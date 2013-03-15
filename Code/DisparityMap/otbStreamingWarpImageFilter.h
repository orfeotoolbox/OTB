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
#ifndef __otbStreamingWarpImageFilter_h
#define __otbStreamingWarpImageFilter_h

#include "otbWarpImageFilter.h"
#include "otbStreamingTraits.h"

namespace otb
{

/** \class StreamingWarpImageFilter
 * \brief This class acts like the otb::WarpImageFilter, but it does not request the largest possible region of the image to warp.
 *
 * Instead, the user should assess the maximum deformation in the deformation field and set it via the SetMaximumDeformation() method.
 *
 * The filter will then compute an appropriate security margin according to the image spacing, the maximum deformation and the interpolator
 * radius in otb::StreamingTraits.
 *
 * This security margin is used to stream the input image, making this filter an entirely streamable one.
 *
 * If the maximum deformation is wrong, this filter is likely to request data outside of the input image buffered region. In this case, pixels
 * outside the region will be set to Zero according to itk::NumericTraits.
 *
 * \sa itk::WarpImageFilter
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */

template <class TInputImage, class TOutputImage, class TDeformationField>
class ITK_EXPORT StreamingWarpImageFilter
  :  public otb::WarpImageFilter<TInputImage, TOutputImage, TDeformationField>
{
public:
  /** Standard class typedefs. */
  typedef StreamingWarpImageFilter                                           Self;
  typedef otb::WarpImageFilter<TInputImage, TOutputImage, TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingWarpImageFilter, itk::WarpImageFilter);

  /** template parameters typedef */
  typedef TInputImage                              InputImageType;
  typedef typename  InputImageType::Pointer        InputImagePointerType;
  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::PointType      PointType;
  typedef typename OutputImageType::IndexType      IndexType;
  typedef typename OutputImageType::PixelType      PixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointerType;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef TDeformationField                        DeformationFieldType;
  typedef typename DeformationFieldType::PixelType DeformationValueType;
  typedef typename DeformationFieldType::Pointer   DeformationFieldPointerType;
  typedef typename DeformationFieldType::RegionType DeformationFieldRegionType;

  /** Accessors */
  itkSetMacro(MaximumDeformation, DeformationValueType);
  itkGetConstReferenceMacro(MaximumDeformation, DeformationValueType);

protected:
  /** Constructor */
  StreamingWarpImageFilter();
  /** Destructor */
  virtual ~StreamingWarpImageFilter() {}
  /** PrintSelf */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /**
   * This filters requires only a part of the input and of the deformation field to
   * produce its output. As such, we need to overload the GenerateInputRequestedRegion() method.
   */
  virtual void GenerateInputRequestedRegion();

  /**
   * Re-implement the method ThreadedGenerateData to mask area outside the deformation grid
   */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  StreamingWarpImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Assesment of the maximum deformation for streaming
  DeformationValueType m_MaximumDeformation;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingWarpImageFilter.txx"
#endif

#endif
