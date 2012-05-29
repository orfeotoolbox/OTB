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
#ifndef __otbMeanShiftSegmentationFilter_h
#define __otbMeanShiftSegmentationFilter_h

#include "itkMacro.h"

#include "otbMeanShiftImageFilter2.h"
#include "otbLabelImageRegionMergingFilter.h"

namespace otb {

/** \class MeanShiftSegmentationFilter
*
*  Performs segmentation of an image by chaining a mean shift filter and region
*  merging filter.
*
*/


template <class TInputImage,  class TOutputLabelImage, class TOutputClusteredImage = TInputImage, class TKernel = KernelUniform>
class MeanShiftSegmentationFilter : public itk::ImageToImageFilter<TInputImage, TOutputLabelImage>
{
public:
  /** Standard Self typedef */
  typedef MeanShiftSegmentationFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputLabelImage> Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage           InputSpectralImageType;
  typedef TOutputLabelImage     OutputLabelImageType;
  typedef TOutputClusteredImage OutputClusteredImageType;
  typedef TKernel               KernelType;
  typedef double                RealType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MeanShiftSegmentationFilter, ImageToImageFilter);

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  // Mean shift filter
  typedef OutputClusteredImageType                                                               MeanShiftFilteredImageType;
  typedef MeanShiftImageFilter2<InputSpectralImageType, MeanShiftFilteredImageType, KernelType>  MeanShiftFilterType;
  typedef typename MeanShiftFilterType::Pointer                                                  MeanShiftFilterPointerType;
  // Region merging filter
  typedef typename MeanShiftFilterType::OutputLabelImageType                             InputLabelImageType;
  typedef LabelImageRegionMergingFilter<InputLabelImageType, MeanShiftFilteredImageType,
                                        OutputLabelImageType, OutputClusteredImageType>  RegionMergingFilterType;
  typedef typename RegionMergingFilterType::Pointer                                      RegionMergingFilterPointerType;

  /** Sets the spatial bandwidth (or radius in the case of a uniform kernel)
    * of the neighborhood for each pixel
    */
  virtual void SetSpatialBandwidth(const RealType _arg)
  {
    this->m_MeanShiftFilter->SetSpatialBandwidth(_arg);
  }
  virtual RealType GetSpatialBandwidth ()
  {
    return this->m_MeanShiftFilter->GetSpatialBandwidth();
  }


  /** Sets the spectral bandwidth (or radius for a uniform kernel) for pixels
    * to be included in the same mode
    */
  virtual void SetRangeBandwidth(const RealType _arg)
  {
    this->m_MeanShiftFilter->SetRangeBandwidth(_arg);
  }
  virtual RealType GetRangeBandwidth ()
  {
    return this->m_MeanShiftFilter->GetRangeBandwidth();
  }


  /** Returns the const image of region labels */
  const OutputLabelImageType * GetLabelOutput() const;
  /** Returns the image of region labels */
  OutputLabelImageType * GetLabelOutput();
  /** Returns the const clustered output image, with one spectral value per region  */
  const OutputClusteredImageType * GetClusteredOutput() const;
  /** Returns the clustered output image, with one spectral value per region  */
  OutputClusteredImageType * GetClusteredOutput();

protected:
  MeanShiftSegmentationFilter();

  virtual ~MeanShiftSegmentationFilter();

//  virtual void GenerateOutputInformation(void);

  virtual void GenerateData();

private:

  MeanShiftFilterPointerType     m_MeanShiftFilter;
  RegionMergingFilterPointerType m_RegionMergingFilter;

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftSegmentationFilter.txx"
#endif

#endif
