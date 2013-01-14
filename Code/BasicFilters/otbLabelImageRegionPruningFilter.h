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
#ifndef __otbLabelImageRegionPruningFilter_h
#define __otbLabelImageRegionPruningFilter_h

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"

#include <set>

namespace otb
{

/** \class LabelImageRegionPruningFilter
 *
 *
 * This class merges regions in the input label image according to the input
 * image of spectral values and the RangeBandwidth parameter.
 *
 *
 * \ingroup ImageSegmentation
 */
template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage = TInputLabelImage, class TOutputClusteredImage = TInputSpectralImage>
class ITK_EXPORT LabelImageRegionPruningFilter
  : public itk::ImageToImageFilter<TInputLabelImage, TOutputLabelImage>
{
public:
  /** Standard class typedef */
  typedef LabelImageRegionPruningFilter                                Self;
  typedef itk::ImageToImageFilter<TInputLabelImage, TOutputLabelImage> Superclass;
  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;
  typedef double                                                       RealType;

  /** Type macro */
  itkTypeMacro(LabelImageRegionPruningFilter, ImageToImageFilter);
  itkNewMacro(Self);

  /** Template parameters typedefs */
  typedef TInputLabelImage                        InputLabelImageType;
  typedef typename InputLabelImageType::PixelType InputLabelType;

  typedef TInputLabelImage                        InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename InputImageType::IndexType      InputIndexType;
  typedef typename InputImageType::SizeType       InputSizeType;
  typedef typename InputImageType::IndexValueType InputIndexValueType;
  typedef typename InputImageType::PointType      PointType;
  typedef typename InputImageType::RegionType     RegionType;
  typedef typename InputImageType::SizeType       SizeType;

  typedef TInputSpectralImage                     InputSpectralImageType;
  typedef typename TInputSpectralImage::PixelType SpectralPixelType;

  typedef TOutputLabelImage                        OutputLabelImageType;
  typedef typename OutputLabelImageType::PixelType OutputLabelType;

  typedef TOutputLabelImage                     OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::PixelType   OutputPixelType;
  typedef typename OutputImageType::RegionType  OutputRegionType;

  typedef TOutputClusteredImage                   OutputClusteredImageType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputLabelImageType::ImageDimension);

  /** Typedefs for region adjacency map */
  typedef InputLabelType      LabelType;
  typedef std::set<LabelType> AdjacentLabelsContainerType;
  typedef std::vector<AdjacentLabelsContainerType> RegionAdjacencyMapType;

  itkSetMacro(MinRegionSize, RealType);
  itkGetConstMacro(MinRegionSize, RealType);

  /** Returns the const image of region labels */
  const OutputLabelImageType * GetLabelOutput() const;
  /** Returns the image of region labels */
  OutputLabelImageType * GetLabelOutput();
  /** Returns the const clustered output image, with one spectral value per region  */
  const OutputClusteredImageType * GetClusteredOutput() const;
  /** Returns the clustered output image, with one spectral value per region  */
  OutputClusteredImageType * GetClusteredOutput();

  /** Sets the input image where the value of a pixel is the region id */
  void SetInputLabelImage( const InputLabelImageType * labelImage);
  /** Sets the input image representing spectral values */
  void SetInputSpectralImage( const InputSpectralImageType * spectralImage);
  /** Returns input label image */
  InputLabelImageType * GetInputLabelImage();
  /** Returns input spectral image */
  InputSpectralImageType * GetInputSpectralImage();

protected:
  virtual void EnlargeOutputRequestedRegion( itk::DataObject *output );

   virtual void GenerateOutputInformation(void);

   virtual void GenerateData();

  /** Constructor */
  LabelImageRegionPruningFilter();

  /** Destructor */
  virtual ~LabelImageRegionPruningFilter();

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Method to build a map of adjacent regions */
  RegionAdjacencyMapType LabelImageToRegionAdjacencyMap(typename OutputLabelImageType::Pointer inputLabelImage);

private:
  LabelImageRegionPruningFilter(const Self &);     //purposely not implemented
  void operator =(const Self&);             //purposely not implemented

  /** Number of components per pixel in the input image */
  unsigned int                   m_NumberOfComponentsPerPixel;
  unsigned int                   m_MinRegionSize;
  /** This contains the label to which each label will be merged */
  std::vector<LabelType>         m_CanonicalLabels;
  /** Contains the spectral value for each region */
  std::vector<SpectralPixelType> m_Modes;
  /** Number of points in each region */
  std::vector<unsigned int>      m_PointCounts;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageRegionPruningFilter.txx"
#endif

#endif
