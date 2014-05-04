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
#ifndef __otbScalarImageToAdvancedTexturesFilter_h
#define __otbScalarImageToAdvancedTexturesFilter_h

#include "itkImageToImageFilter.h"
#include "itkHistogram.h"
#include "itkArray.h"
#include <vector>

namespace otb
{
/**
 * \class ScalarImageToAdvancedTexturesFilter
 *  \brief This class computes 9 haralick textures from the given input image.
 *
 *  The textures are computed over a sliding window with user defined radius:
 *  (where \f$ g(i, j) \f$ is the element in cell i, j of a normalized GLCIL):
 *
 * "Mean" \f$ = \sum_{i, j}i g(i, j) \f$
 *
 * "Sum of squares: Variance" \f$ = f_4 = \sum_{i, j}(i - \mu)^2 g(i, j) \f$
 *
 * "Sum average" \f$ = f_6 = -\sum_{i}i g_{x+y}(i)
 *
 * "Sum Variance" \f$ = f_7 = \sum_{i}(i - f_8)^2 g_{x+y}(i) \f$
 *
 * "Sum Entropy" \f$= f_8 = -\sum_{i}g_{x+y}(i) log (g_{x+y}(i)) \f$
 *
 * "Difference variance" \f$ = f_10 = variance of g_{x-y}(i)
 *
 * "Difference entropy" \f$ = f_11 = -\sum_{i}g_{x-y}(i) log (g_{x-y}(i)) \f$
 *
 * "Information Measures of Correlation IC1" \f$ = f_12 = \frac{f_9 - HXY1}{H} \f$
 *
 * "Information Measures of Correlation IC2" \f$ = f_13 = \sqrt{1 - \exp{-2}|HXY2 - f_9|} \f$
 *
 * Above, \f$ \mu =  \f$ (weighted pixel average) \f$ = \sum_{i, j}i \cdot g(i, j) =
 * \sum_{i, j}j \cdot g(i, j) \f$ (due to matrix summetry), and
 *
 * \f$ \g_{x+y}(k) =  \sum_{i}\sum_{j}g(i)\f$ where \f$ i+j=k \f$ and \f$ k = 2, 3, .., 2N_[g}  \f$ and
 *
 * \f$ \g_{x-y}(k) =  \sum_{i}\sum_{j}g(i)\f$ where \f$ i-j=k \f$ and \f$ k = 0, 1, .., N_[g}-1  \f$
 *
 * Print references:
 *
 * Haralick, R.M., K. Shanmugam and I. Dinstein. 1973.  Textural Features for
 * Image Classification. IEEE Transactions on Systems, Man and Cybernetics.
 * SMC-3(6):610-620.
 *
 * David A. Clausi and Yongping Zhao. 2002. Rapid extraction of image texture by
 * co-occurrence using a hybrid data structure. Comput. Geosci. 28, 6 (July
 * 2002), 763-774. DOI=10.1016/S0098-3004(01)00108-X
 * http://dx.doi.org/10.1016/S0098-3004(01)00108-X
 *
 * de O.Bastos, L.; Liatsis, P.; Conci, A., Automatic texture segmentation based
 * on k-means clustering and efficient calculation of co-occurrence
 * features. Systems, Signals and Image Processing, 2008. IWSSIP 2008. 15th
 * International Conference on , vol., no., pp.141,144, 25-28 June 2008
 * doi: 10.1109/IWSSIP.2008.4604387
 *
 * Neighborhood size can be set using the SetRadius() method. Offset for co-occurence estimation
 * is set using the SetOffset() method.
 *
 * \sa ScalarImageToCooccurrenceIndexedList
 * \sa ScalarImageToTexturesFiler
 *
 */
template<class TInpuImage, class TOutputImage>
class ScalarImageToAdvancedTexturesFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToAdvancedTexturesFilter               Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToAdvancedTexturesFilter, ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                           InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::RegionType  InputRegionType;
  typedef typename InputImageType::OffsetType  OffsetType;
  typedef typename InputRegionType::SizeType   SizeType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  typedef typename itk::NumericTraits< InputPixelType >::RealType                      MeasurementType;
  typedef itk::Statistics::DenseFrequencyContainer2                                    THistogramFrequencyContainer;
  typedef itk::Statistics::Histogram< MeasurementType, THistogramFrequencyContainer >  HistogramType;
  typedef typename HistogramType::Pointer                                              HistogramPointer;
  typedef typename HistogramType::ConstPointer                                         HistogramConstPointer;
  typedef typename HistogramType::MeasurementVectorType                                MeasurementVectorType;
  typedef typename HistogramType::RelativeFrequencyType                                RelativeFrequencyType;
  typedef typename HistogramType::TotalAbsoluteFrequencyType                           TotalAbsoluteFrequencyType;
  typedef typename HistogramType::IndexType                                            HistogramIndexType;

  /** Lookup array used to store the index of the given pixel pair. This index
    * is calculated from the HistogramType */
  typedef itk::Array<int> LookupArrayType;

  /** struct to hold the index of pixel pair and its frequency. The frequnecy is
    * incremented for the next occurrence of the same pair */

  typedef struct Cooccurrence {
    HistogramIndexType  index;
    RelativeFrequencyType frequency;
  }CooccurrenceType;

  /** array to hold CooccurrenceType. Index of the array where the cooccurrence
    * is stored is saved in the LookupArrayType. */
  typedef std::vector<CooccurrenceType> GreyLevelCooccurrenceListType;

  /** std::vector iterator typedef for GreyLevelCooccurrenceListType */
  typedef typename std::vector<CooccurrenceType>::iterator GreyLevelCooccurrenceListIteratorType;

  /** std::vector const_iterator typedef for GreyLevelCooccurrenceListType */
  typedef typename std::vector<CooccurrenceType>::const_iterator GreyLevelCooccurrenceListConstIteratorType;

  itkStaticConstMacro(MeasurementVectorSize, int, 2 );

  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius, SizeType);

  /** Set the offset for co-occurence computation */
  itkSetMacro(Offset, OffsetType);

  /** Get the offset for co-occurence computation */
  itkGetMacro(Offset, OffsetType);

  /** Set the number of bin per axis for histogram generation */
  itkSetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Get the number of bin per axis for histogram generation */
  itkGetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Set the input image minimum */
  itkSetMacro(InputImageMinimum, InputPixelType);

  /** Get the input image minimum */
  itkGetMacro(InputImageMinimum, InputPixelType);

  /** Set the input image maximum */
  itkSetMacro(InputImageMaximum, InputPixelType);

  /** Get the input image maximum */
  itkGetMacro(InputImageMaximum, InputPixelType);

  /** Get the mean output image */
  OutputImageType * GetMeanOutput();

  /** Get the variance output image */
  OutputImageType * GetVarianceOutput();

  /** Get the sum average output image */
  OutputImageType * GetSumAverageOutput();

  /** Get the sum of variances output image */
  OutputImageType * GetSumVarianceOutput();

  /** Get the sum of entropies output image */
  OutputImageType * GetSumEntropyOutput();

  /** Get the difference of entropies output image */
  OutputImageType * GetDifferenceEntropyOutput();

  /** Get the difference of variance output image */
  OutputImageType * GetDifferenceVarianceOutput();

  /** Get the IC1 image */
  OutputImageType * GetIC1Output();

  /** Get the IC2 output image */
  OutputImageType * GetIC2Output();

protected:
  /** Constructor */
  ScalarImageToAdvancedTexturesFilter();
  /** Destructor */
  ~ScalarImageToAdvancedTexturesFilter();

  /** Before ThreadedGenerateData created a single histogram instance **/
  virtual void BeforeThreadedGenerateData( );

  /** Generate the input requested region */
  virtual void GenerateInputRequestedRegion();

  /** Parallel textures extraction */
  virtual void ThreadedGenerateData(const OutputRegionType& outputRegion, itk::ThreadIdType threadId);

private:
  ScalarImageToAdvancedTexturesFilter(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType& region1, const OutputRegionType& region2);

  /** Radius of the window on which to compute textures */
  SizeType m_Radius;

  /** Radius of the neighborhood iterator which is minumum of m_Radius */
  SizeType m_NeighborhoodRadius;

  /** Offset for co-occurence */
  OffsetType m_Offset;

  /** Number of bins per axis for histogram generation */
  unsigned int m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType m_InputImageMaximum;

  /** Histogram instance used to get index for the pixel pair */
  HistogramPointer m_Histogram;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToAdvancedTexturesFilter.txx"
#endif

#endif
