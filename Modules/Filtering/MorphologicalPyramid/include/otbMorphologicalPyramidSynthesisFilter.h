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
#ifndef __otbMorphologicalPyramidSynthesisFilter_h
#define __otbMorphologicalPyramidSynthesisFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/**
 * \class MorphologicalPyramidSynthesisFilter
 * \brief Recomposition of the multiscale analysis performed with
 * a MorphologicalPyramidAnalyseFilter.
 *
 * Once a morphological pyramidal decompisition has been made, it is
 * possible to rebuild without any loss of information the original
 * image at full resolution. The MorphologicalPyramidsynthesisFilter
 * provides such a functionality. At each iteration, it perfoms the
 * up-sampling of the current image using the
 * otb::morphologicalPyramid::Resampler filter, the addition of
 * the details lost in down-sampling, and the addition of the details
 * lost in smoothing.
 *
 * \ingroup MultiScale
 * \sa MorphologicalPyramidAnalyseFilter
 * \sa MorphologicalPyramidResampler
 *
 * \ingroup OTBMorphologicalPyramid
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MorphologicalPyramidSynthesisFilter
  : public ImageToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MorphologicalPyramidSynthesisFilter               Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(MorphologicalPyramidSynthesisFilter, MorphologicalPyramidSynthesisFilter);
  /** Input image dimension constant macro */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Output related typedefs */
  typedef typename Superclass::OutputImageType                      OutputImageType;
  typedef typename Superclass::OutputImageListType                  OutputImageListType;
  typedef typename OutputImageListType::Pointer                     OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType               OutputImagePointerType;
  typedef typename Superclass::OutputImageListType::Iterator        ImageListIterator;
  typedef typename Superclass::OutputImageListType::ReverseIterator ImageListReverseIterator;
  /** Input related typedefs */
  typedef typename Superclass::InputImageType       InputImageType;
  typedef typename Superclass::InputImageRegionType InputImageRegionType;
  typedef typename InputImageType::Pointer          InputImagePointer;
  typedef typename InputImageType::ConstPointer     InputImageConstPointer;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef typename InputImageType::PixelType        PixelType;
  typedef typename InputImageType::SpacingType      SpacingType;
  typedef otb::ImageList<InputImageType>            InputImageListType;
  typedef typename InputImageListType::Pointer      InputImageListPointerType;
  /** Size vector typedefs */
  typedef std::vector<SizeType>                     SizeVectorType;
  typedef typename SizeVectorType::iterator         SizeIterator;
  typedef typename SizeVectorType::reverse_iterator SizeReverseIterator;
  /**
   * Set The SupFilter details
   * \param imageList The brighter details extracted from the filtering operation.
   */
  void SetSupFilter(InputImageListType * imageList);
  /**
   * Set The InfFilter details
   * \param imageList The darker details extracted from the filtering operation.
   */
  void SetInfFilter(InputImageListType * imageList);
  /**
   * Set The SupDeci details
   * \param imageList The brighter details extracted from the filtering operation.
   */
  void SetSupDeci(InputImageListType * imageList);
  /**
   * Set The InfDeci details
   * \param imageList The darker details extracted from the filtering operation.
   */
  void SetInfDeci(InputImageListType * imageList);
  /**
   * Get The SupFilter details
   * \return The brighter details extracted from the filtering operation.
   */
  InputImageListType* GetSupFilter(void);
  /**
   * Get The InfFilter details
   * \return The darker details extracted from the filtering operation.
   */
  InputImageListType* GetInfFilter(void);
  /**
   * Get The SupDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  InputImageListType* GetSupDeci(void);
  /**
   * Get The InfDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  InputImageListType* GetInfDeci(void);

protected:
  /** Constructor */
  MorphologicalPyramidSynthesisFilter();
  /** Destructor */
  ~MorphologicalPyramidSynthesisFilter() ITK_OVERRIDE;
  void GenerateOutputInformation() ITK_OVERRIDE {}  // does nothing
  /** Main computation method */
  void GenerateData() ITK_OVERRIDE;
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidSynthesisFilter.txx"
#endif

#endif
