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

// Adapted from otbImageToPathListAlignFilter.h
#ifndef __otbRegionImageToRectangularPathListFilter_h
#define __otbRegionImageToRectangularPathListFilter_h

#include "itkImageSource.h"
#include "itkConceptChecking.h"
#include "otbImage.h"
#include "otbImageToPathListFilter.h"
#include <vector>
#include "otbVectorDataSource.h"
#include "otbVectorData.h"

namespace otb
{

/** \class RegionImageToRectangularPathListFilter
 * \brief Base class used to implement filter to model regions by rectangles.
 *
 *
 * \ingroup OTBPath
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT RegionImageToRectangularPathListFilter : public ImageToPathListFilter<TInputImage, TOutputPath>
{
public:
  /** Standard class typedefs. */
  typedef RegionImageToRectangularPathListFilter          Self;
  typedef ImageToPathListFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RegionImageToRectangularPathListFilter, ImageToPathListFilter);
//  itkTypeMacro(ImageToPathListAlignFilter, itk::ImageSource);

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Some convenient typedefs. */
  typedef typename Superclass::OutputPathListType OutputPathListType;

  typedef typename Superclass::InputImageType       InputImageType;
  typedef typename Superclass::InputImageRegionType InputImageRegionType;
  typedef typename InputImageType::Pointer          InputImagePointer;
  typedef typename InputImageType::ConstPointer     InputImageConstPointer;

  typedef typename InputImageType::SizeType  SizeType;
  typedef typename InputImageType::ValueType ValueType;
  typedef typename InputImageType::PixelType PixelType;

  typedef typename Superclass::OutputPathType OutputPathType;
  // typedef typename Superclass::OutputPathListType     OutputPathListType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;

//  typedef          float                                        RealType;
//typedef typename itk::NumericTraits<PixelType>::RealType       RealType;
  typedef double RealType;
//  typedef typename itk::Image<RealType, InputImageDimension>      RealImageType;
  typedef typename otb::Image<RealType, InputImageDimension> RealImageType;
  typedef typename RealImageType::Pointer                    RealImageTypePointer;
  typedef typename RealImageType::IndexType                  RealImageTypeIndexType;

  /** The minimum rectangular fit of returned rectangles. */
  itkSetMacro(MinimumFit, double);
  itkGetConstReferenceMacro(MinimumFit, double);

  /** The minimum size of returned rectangles. */
  itkSetMacro(MinimumSize, double);
  itkGetConstReferenceMacro(MinimumSize, double);

  //jgc
  /** Dimension of the image the iterator walks.  This constant is needed so
   * that functions that are templated over image iterator type (as opposed to
   * being templated over pixel type and dimension) can have compile time
   * access to the dimension of the image that the iterator walks. */
  itkStaticConstMacro(NDimensions, unsigned int, TInputImage::ImageDimension);

protected:
  RegionImageToRectangularPathListFilter();
  ~RegionImageToRectangularPathListFilter() ITK_OVERRIDE;

  void GenerateOutputInformation() ITK_OVERRIDE{}  // do nothing
  void GenerateData() ITK_OVERRIDE;

  double m_MinimumFit;
  double m_MinimumSize;
  int    m_CrossTermFormula;

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  //jgc
  typedef Image<unsigned char, itkGetStaticConstMacro(NDimensions)> MarkerImageType;
  typename MarkerImageType::Pointer markerImage;

private:
  RegionImageToRectangularPathListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRegionImageToRectangularPathListFilter.txx"
#endif

#endif
