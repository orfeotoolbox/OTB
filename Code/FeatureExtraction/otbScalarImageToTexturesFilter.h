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

#ifndef __otbScalarImageToTexturesFilter_h
#define __otbScalarImageToTexturesFilter_h

#include "otbGreyLevelCooccurrenceIndexedList.h"
#include "itkImageToImageFilter.h"

namespace otb
{
/**
 * \class ScalarImageToTexturesFilter
 * \brief This class compute 8 local Haralick textures features. The 8 output
 * image channels are: Energy, Entropy, Correlation, Inverse Difference Moment,
 * Inertia, Cluster Shade, Cluster Prominence and Haralick Correlation. They
 * are provided in this exact order in the output image. Thus, this application
 * computes the following Haralick textures over a neighborhood with user
 * defined radius.
 *
 * To improve the speed of computation, a variant of Grey Level Co-occurrence
 * Matrix(GLCM) called Grey Level Co-occurrence Indexed List (GLCIL) is
 * used. Given below is the mathematical explanation on the computation of each
 * textures. Here $ g(i, j) $ is the frequency of element in the GLCIL whose
 * index is i, j. GLCIL stores a pair of frequency of two pixels from the given
 * offset and the cell index (i, j) of the pixel in the neighborhood
 * window. :(where each element in GLCIL is a pair of pixel index and it's
 * frequency, $ g(i, j) $ is the frequency value of the pair having index is i, j).
 *
 * "Energy" \f$ = f_1 = \sum_{i, j}g(i, j)^2 \f$
 *
 * "Entropy" \f$ = f_2 = -\sum_{i, j}g(i, j) \log_2 g(i, j)\f$, or 0 if \f$g(i, j) = 0\f$
 *
 * "Correlation" \f$ = f_3 = \sum_{i, j}\frac{(i - \mu)(j - \mu)g(i, j)}{\sigma^2} \f$
 *
 * "Difference Moment" \f$= f_4 = \sum_{i, j}\frac{1}{1 + (i - j)^2}g(i, j) \f$
 *
 * "Inertia" \f$ = f_5 = \sum_{i, j}(i - j)^2g(i, j) \f$ (sometimes called "contrast")
 *
 * "Cluster Shade" \f$ = f_6 = \sum_{i, j}((i - \mu) + (j - \mu))^3 g(i, j) \f$
 *
 * "Cluster Prominence" \f$ = f_7 = \sum_{i, j}((i - \mu) + (j - \mu))^4 g(i, j) \f$
 *
 * "Haralick's Correlation" \f$ = f_8 = \frac{\sum_{i, j}(i, j) g(i, j) -\mu_t^2}{\sigma_t^2} \f$
 * where \f$\mu_t\f$ and \f$\sigma_t\f$ are the mean and standard deviation of the row
 * (or column, due to symmetry) sums.
 *
 * Above, \f$ \mu =  \f$ (weighted pixel average) \f$ = \sum_{i, j}i \cdot g(i, j) =
 * \sum_{i, j}j \cdot g(i, j) \f$ (due to matrix summetry), and
 *
 * \f$ \sigma =  \f$ (weighted pixel variance) \f$ = \sum_{i, j}(i - \mu)^2 \cdot g(i, j) =
 * \sum_{i, j}(j - \mu)^2 \cdot g(i, j)  \f$  (due to matrix summetry)
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
 * \sa otb::GreyLevelCooccurrenceIndexedList
 * \sa otb::ScalarImageToAdvancedTexturesFiler
 * \sa otb::ScalarImageToHigherOrderTexturesFilter
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 */
template<class TInpuImage, class TOutputImage>
class ScalarImageToTexturesFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToTexturesFilter                       Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToTexturesFilter, ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                           InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::RegionType  InputRegionType;
  typedef typename InputRegionType::SizeType   SizeType;
  typedef typename InputImageType::OffsetType   OffsetType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  typedef GreyLevelCooccurrenceIndexedList< InputPixelType >   CooccurrenceIndexedListType;
  typedef typename CooccurrenceIndexedListType::Pointer       CooccurrenceIndexedListPointerType;
  typedef typename CooccurrenceIndexedListType::ConstPointer  CooccurrenceIndexedListConstPointerType;
  typedef typename CooccurrenceIndexedListType::IndexType              CooccurrenceIndexType;
  typedef typename CooccurrenceIndexedListType::PixelValueType         PixelValueType;
  typedef typename CooccurrenceIndexedListType::RelativeFrequencyType  RelativeFrequencyType;
  typedef typename CooccurrenceIndexedListType::VectorType             VectorType;

  typedef typename VectorType::iterator                    VectorIteratorType;
  typedef typename VectorType::const_iterator              VectorConstIteratorType;

  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius, SizeType);

  /** Set the offset for co-occurence computation */
  itkSetMacro(Offset, OffsetType);

  /** Get the offset for co-occurence computation */
  itkGetMacro(Offset, OffsetType);

  /** Set the number of bin per axis */
  itkSetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Get the number of bin per axis */
  itkGetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Set the input image minimum */
  itkSetMacro(InputImageMinimum, InputPixelType);

  /** Get the input image minimum */
  itkGetMacro(InputImageMinimum, InputPixelType);

  /** Set the input image maximum */
  itkSetMacro(InputImageMaximum, InputPixelType);

  /** Get the input image maximum */
  itkGetMacro(InputImageMaximum, InputPixelType);

  /** Get the energy output image */
  OutputImageType * GetEnergyOutput();

  /** Get the entropy output image */
  OutputImageType * GetEntropyOutput();

  /** Get the correlation output image */
  OutputImageType * GetCorrelationOutput();

  /** Get the inverse difference moment output image */
  OutputImageType * GetInverseDifferenceMomentOutput();

  /** Get the inertia output image */
  OutputImageType * GetInertiaOutput();

  /** Get the cluster shade output image */
  OutputImageType * GetClusterShadeOutput();

  /** Get the cluster prominence image */
  OutputImageType * GetClusterProminenceOutput();

  /** Get the Haralick correlation output image */
  OutputImageType * GetHaralickCorrelationOutput();

protected:
  /** Constructor */
  ScalarImageToTexturesFilter();
  /** Destructor */
  ~ScalarImageToTexturesFilter();
  /** Generate the input requested region */
  virtual void GenerateInputRequestedRegion();
  /** Before Parallel textures extraction */
  virtual void BeforeThreadedGenerateData();
  /** Parallel textures extraction */
  virtual void ThreadedGenerateData(const OutputRegionType& outputRegion, itk::ThreadIdType threadId);

private:
  ScalarImageToTexturesFilter(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType& region1, const OutputRegionType& region2);

  /** Radius of the window on which to compute textures */
  SizeType m_Radius;

  /** Offset for co-occurence */
  OffsetType m_Offset;

  /** Radius of the neighborhood iterator which is minumum of m_Radius */
  SizeType m_NeighborhoodRadius;

  /** Number of bins per axis */
  unsigned int m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType m_InputImageMaximum;

  //TODO: should we use constexpr? only c++11 and problem for msvc
  const double GetPixelValueTolerance() const {return 0.0001; }

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToTexturesFilter.txx"
#endif

#endif
