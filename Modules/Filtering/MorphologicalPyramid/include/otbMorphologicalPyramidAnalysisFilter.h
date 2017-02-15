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
#ifndef otbMorphologicalPyramidAnalysisFilter_h
#define otbMorphologicalPyramidAnalysisFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/** \class MorphologicalPyramidAnalysisFilter
 * \brief Multiscale analysis filter using the morphological pyramid algorithm.
 *
 * Pyramidal decomposition is based on the following statement : once an
 * image has been smoothed with a linear filter, it does not contain
 * any more high-frequency details. Therefore, it can be down-sampled
 * without any loss of information, according to Shannon Theorem. By
 * iterating the same smoothing on the down-sampled image, a
 * multi-resolution decomposition of the scene is
 * computed. If the smoothing filter is a morphological filter, this
 * is no longer true, as the filter is not linear. However, by keeping
 * the details possibly lost in the down-sampling operation, such a
 * decomposition can be used.
 *
 * The MorphologicalPyramidAnalysisFilter provide such a
 * decomposition. It's computation process is an iterative analyse
 * involving smoothing by the morphological filter, computing the
 * details lost in the smoothing, down-sampling the current image, and
 * computing the details lost in the down-sampling.
 *
 * \ingroup MultiScale
 * \sa MorphologicalPyramidSynthesisFilter, OpeningClosingMorphologicalFilter,
 * ClosingOpeningMorphologicalFilter
 *
 * \ingroup OTBMorphologicalPyramid
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
class ITK_EXPORT MorphologicalPyramidAnalysisFilter
  : public ImageToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MorphologicalPyramidAnalysisFilter                Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(MorphologicalPyramidAnalysisFilter, MorphologicalPyramidAnalysisFilter);
  /** Input image dimension constant macro */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Template parameters typedefs */
  typedef TMorphoFilter                         MorphoFilterType;
  typedef typename MorphoFilterType::KernelType KernelType;
  /** Output related typedefs */
  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImageListType    OutputImageListType;
  typedef typename OutputImageListType::Pointer       OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  /** Input related typedefs */
  typedef typename Superclass::InputImageType       InputImageType;
  typedef typename Superclass::InputImageRegionType InputImageRegionType;
  typedef typename InputImageType::Pointer          InputImagePointerType;
  typedef typename InputImageType::ConstPointer     InputImageConstPointerType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef typename InputImageType::PixelType        PixelType;
  typedef typename InputImageType::SpacingType      SpacingType;
  /** Accessors */
  itkSetMacro(NumberOfLevels, int);
  itkSetMacro(DecimationRatio, double);
  itkGetConstReferenceMacro(NumberOfLevels, int);
  itkGetConstReferenceMacro(DecimationRatio, double);
  /**
   * Get The Analyse image at each level of the pyramid.
   * \return The analysed image at each level of the pyramid.
   */
  OutputImageListType* GetOutput(void) ITK_OVERRIDE;
  /**
   * Get The SupFilter details
   * \return The brighter details extracted from the filtering operation.
   */
  OutputImageListType * GetSupFilter(void);
  /**
   * Get The InfFilter details
   * \return The darker details extracted from the filtering operation.
   */
  OutputImageListType * GetInfFilter(void);
  /**
   * Get The SupDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  OutputImageListType * GetSupDeci(void);
  /**
   * Get The InfDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  OutputImageListType * GetInfDeci(void);

protected:
  /** Constructor */
  MorphologicalPyramidAnalysisFilter();
  /** Destructor */
  ~MorphologicalPyramidAnalysisFilter() ITK_OVERRIDE;
  void GenerateOutputInformation() ITK_OVERRIDE {}  // does nothing
  /** Main computation method */
  void GenerateData() ITK_OVERRIDE;
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  /** Number of levels of the algorithm */
  int m_NumberOfLevels;
  /** Subsampling scale used */
  double m_DecimationRatio;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidAnalysisFilter.txx"
#endif

#endif
