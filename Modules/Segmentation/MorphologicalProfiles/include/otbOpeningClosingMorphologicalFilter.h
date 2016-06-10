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
#ifndef __otbOpeningClosingMorphologicalFilter_h
#define __otbOpeningClosingMorphologicalFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/**
 * \class OpeningClosingMorphologicalFilter
 * \brief This filter implements an opening grayscale morphological operation
 * followed by a closing grayscale morphological operation. It tends to simplify
 * image by obliterating details with a lower extent than the structuring element.
 *
 * This filter implements an opening grayscale morphological operation
 * followed by a closing grayscale morphological operation. It tends to simplify
 * image by obliterating details with a lower extent than the structuring element.
 *
 * This filter can be used in the morphological pyramid analyse filter. Due to the order
 * of composition of the two basic morphological operation, the filtered details are dark
 * on a brighter background.
 *
 * \sa ClosingOpeningMorphologicalFilter,
 * MorphologicalPyramidAnalyseFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT OpeningClosingMorphologicalFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef OpeningClosingMorphologicalFilter                  Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(OpeningClosingMorphologicalFilter, ImageToImageFilter);

  /** Template parameter typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TKernel      KernelType;

  /** Useful typedefs */
  typedef typename InputImageType::Pointer     InputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename TInputImage::PixelType      PixelType;

  /** Kernel accessors */
  itkSetMacro(Kernel, KernelType);
  itkGetConstReferenceMacro(Kernel, KernelType);

protected:
  /** Constructor */
  OpeningClosingMorphologicalFilter();
  /** Destructor */
  ~OpeningClosingMorphologicalFilter() ITK_OVERRIDE {}

  /* void GenerateInputRequestedRegion(); */
  /*   void EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output)); */

  /** Main computation method */
  void GenerateData() ITK_OVERRIDE;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  OpeningClosingMorphologicalFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  /** Morphological operation kernel  */
  KernelType m_Kernel;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOpeningClosingMorphologicalFilter.txx"
#endif
#endif
