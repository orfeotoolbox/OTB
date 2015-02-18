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
#ifndef __otbMorphologicalPyramidSegmentationFilter_h
#define __otbMorphologicalPyramidSegmentationFilter_h

#include "otbImageListToImageListFilter.h"
namespace otb
{
/** \class MorphologicalPyramidSegmentationFilter
 * \brief This filter performs a segmentation of the details supFilter and infFilter extracted
 * with the morphological pyramid.
 *
 * The first step is to convert the multi-resolution analysis from the pyramid to a multi-scale one using the
 * MRToMSConverter filter.
 *
 * The segmentation algorithm used is based on seeds extraction using the ImageToPointSetFilter, followed by
 * a connected threshold segmentation using the ConnectedThresholdImageFilter. A final relabelling step is done
 * with the LabelImageFilter adn RelabelImageFilter to remove object whose sizes are to small regarding the
 * MinimumObjectSize parameter. The threshold for seeds extraction and segmentation are computed using quantiles.
 *
 * A pre processing step is applied by multiplying the full resolution brighter details (resp. darker details)
 * with the original image (resp. the inverted original image). This perfoms an enhancement of the regions contour
 * precision.
 *
 * The details from the pyramid are set via the SetBrighterDetails() and SetDarkerDetails() methods. The brighter and
 * darker details depends on the filter used in the pyramid analysis. If the OpeningClosing filter is used, then the
 * brighter details are those from the supFilter image list, whereas if the ClosingOpening filter is used, the brighter
 * details are those from the infFilter list.
 *
 * The output of the segmentation filter is a single segmentation images list, containing first the brighter details
 * segmentation from higher scale to lower, and then the darker details in the same order. The attention of the used
 * is drawn to the fact that since the label filter used internally will deal with a large number of labels, so the
 * OutputPixelType is required to be sufficiently precise. Unsigned short or Unsigned long would be a good choice,
 * unless the user has a very good reason to think that a less precise type will be sufficient.
 *
 * \ingroup MultiScale
 * \sa MorphologicalPyramidSynthesisFilter, OpeningClosingMorphologicalFilter,
 * ClosingOpeningMorphologicalFilter, Segmenter, MRToMSConverter, Resampler
 *
 * \ingroup OTBMorphologicalPyramid
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MorphologicalPyramidSegmentationFilter
  : public ImageListToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MorphologicalPyramidSegmentationFilter                Self;
  typedef ImageListToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(MorphologicalPyramidSegmentationFilter, ImageListToImageListFilter);
  /** Input image dimension constant macro */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Output related typedefs */
  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename Superclass::OutputImageListType    OutputImageListType;
  typedef typename OutputImageListType::Pointer       OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename OutputImageListType::Iterator      OutputImageListIteratorType;
  /** Input related typedefs */
  typedef typename Superclass::InputImageType     InputImageType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename Superclass::InputImageListType InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  /** Vector of number of objects */
  typedef std::vector<OutputPixelType> NumberOfObjectsVectorType;
  /** Min object size parameter accessor */
  itkSetMacro(MinimumObjectSize, unsigned long);
  itkGetMacro(MinimumObjectSize, unsigned long);
  /** Quantile for seeds extraction */
  itkSetMacro(SeedsQuantile, double);
  itkGetMacro(SeedsQuantile, double);
  /** Quantile for connected thresholding */
  itkSetMacro(ConnectedThresholdQuantile, double);
  itkGetMacro(ConnectedThresholdQuantile, double);
  /**
   * Get a vector containing the number of segmented objects for each image.
   * \return The vector.
   */
  NumberOfObjectsVectorType GetNumberOfObjectsVector(void);
  /**
   * Set the reference image.
   * \param image The reference image which was decomposed by the pyramid.
   */
  void SetReferenceImage(InputImageType * image);
  /**
   * Set the brighter details from the pyramid. The brighter details are the supFilter or
   * infFilter details from the pyramid, depending on the morphological filter used.
   * \param imageList The brighter details extracted from the filtering operation.
   */
  void SetBrighterDetails(InputImageListType * imageList);
  /**
   * Set the darker details from the pyramid. The darker details are the supFilter or
   * infFilter details from the pyramid, depending on the morphological filter used.
   * \param imageList The darker details extracted from the filtering operation.
   */
  void SetDarkerDetails(InputImageListType * imageList);
  /**
   * Get the reference image
   * \return The reference image.
   */
  InputImageType* GetReferenceImage(void);
  /**
   * Get the brighter details from the pyramid. The brighter details are the supFilter or
   * infFilter details from the pyramid, depending on the morphological filter used.
   * \return The brighter details extracted from the resampling operation.
   */
  InputImageListType* GetBrighterDetails(void);
  /**
   * Get the darker details from the pyramid. The darker details are the supFilter or
   * infFilter details from the pyramid, depending on the morphological filter used.
   * \return The darker details extracted from the resampling operation.
   */
  InputImageListType *GetDarkerDetails(void);

protected:
  /** Constructor */
  MorphologicalPyramidSegmentationFilter();
  /** Destructor */
  virtual ~MorphologicalPyramidSegmentationFilter();
  virtual void GenerateOutputInformation() {}  // does nothing
  /** Main computation method */
  virtual void GenerateData();
  /** Printself method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
private:
  unsigned long m_MinimumObjectSize;
  /** Quantile for seeds determination */
  double m_SeedsQuantile;
  /** Quantile to set the connectedThresholdFilter threshold */
  double m_ConnectedThresholdQuantile;
  /** Number of segmented objects per images */
  NumberOfObjectsVectorType m_NumberOfObjectsVector;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidSegmentationFilter.txx"
#endif

#endif
