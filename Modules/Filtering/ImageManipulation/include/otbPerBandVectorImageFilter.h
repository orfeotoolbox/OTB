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
#ifndef otbPerBandVectorImageFilter_h
#define otbPerBandVectorImageFilter_h

#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToImageListApplyFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbImageList.h"
#include "otbImage.h"
#include "itkImageToImageFilter.h"

namespace otb
{
/** \class PerBandVectorImageFilter
 *  \brief This filter is a helper class to apply per band a standard itk::ImageToImageFilter to a VectorImage.
 *
 *  This filter is a composite filter composed of the following filters :
 *  - VectorImageToImageListFilter
 *  - ImageListToImageListApplyFilter
 *  - ImageListToVectorImageFilter
 *
 * \sa VectorImageToImageListFilter
 * \sa ImageListToImageListApplyFilter
 * \sa ImageListToVectorImageFilter
 * \ingroup Streamed
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage, class TFilter>
class ITK_EXPORT PerBandVectorImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef PerBandVectorImageFilter                           Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PerBandVectorImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                                                 InputVectorImageType;
  typedef typename InputVectorImageType::Pointer                      InputVectorImagePointerType;
  typedef typename InputVectorImageType::InternalPixelType            InputPixelType;
  typedef Image<InputPixelType, InputVectorImageType::ImageDimension> InputImageType;
  typedef ImageList<InputImageType>                                   InputImageListType;

  typedef TOutputImage                                                  OutputVectorImageType;
  typedef typename OutputVectorImageType::Pointer                       OutputVectorImagePointerType;
  typedef typename OutputVectorImageType::InternalPixelType             OutputPixelType;
  typedef Image<OutputPixelType, OutputVectorImageType::ImageDimension> OutputImageType;
  typedef ImageList<OutputImageType>                                    OutputImageListType;

  typedef TFilter                      FilterType;
  typedef typename FilterType::Pointer FilterPointerType;

  // Internal filters definition
  typedef VectorImageToImageListFilter<InputVectorImageType, InputImageListType> DecompositionFilterType;
  typedef typename DecompositionFilterType::Pointer                              DecompositionFilterPointerType;
  typedef ImageListToImageListApplyFilter<InputImageListType, OutputImageListType, FilterType>
  ProcessingFilterType;
  typedef typename ProcessingFilterType::Pointer                                   ProcessingFilterPointerType;
  typedef ImageListToVectorImageFilter<OutputImageListType, OutputVectorImageType> RecompositionFilterType;
  typedef typename RecompositionFilterType::Pointer                                RecompositionFilterPointerType;

  /// Accessors
  itkSetObjectMacro(Filter, FilterType);
  itkGetObjectMacro(Filter, FilterType);

  itkSetMacro(OutputIndex, unsigned int);
  itkGetMacro(OutputIndex, unsigned int);

  void GenerateInputRequestedRegion(void) ITK_OVERRIDE;
  void GenerateOutputInformation(void) ITK_OVERRIDE;

protected:
  /** Main computation method */
  void GenerateData(void) ITK_OVERRIDE;
  /** Constructor */
  PerBandVectorImageFilter();
  /** Destructor */
  ~PerBandVectorImageFilter() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  PerBandVectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /// The processing filter
  FilterPointerType m_Filter;

  /// The index of the output of the filter to gather
  unsigned m_OutputIndex;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPerBandVectorImageFilter.txx"
#endif

#endif
