/*
 * ElmVcaFilter.h
 * Date:     $Date$
 * Version:  $Revision$
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2010 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef ELMVCAFILTER_H_
#define ELMVCAFILTER_H_

#include <limits>
#include <cmath>
#include <ctime>
#include <vector>
#include <list>
#include "vnl/algo/vnl_symmetric_eigensystem.h"
#include <vcl_algorithm.h>
#include <vcl_vector.h>

#include <otbStreamingStatisticsVectorImageFilter2.h>
#include <otbStreamingStatisticsImageFilter.h>
#include <otbVectorImageToImageListFilter.h>
#include <otbImageList.h>
#include <otbImage.h>
#include <otbVectorImage.h>

#include <itkImageToImageFilter.h>
#include <itkImageRegionConstIterator.h>
#include <itkVariableLengthVector.h>
#include <itkCovarianceCalculator.h>
#include <itkImageToListAdaptor.h>
#include <itkListSample.h>

#include "VahineMacro.h"
#include "VcaFilter.h"

namespace otb {
template <class TImage>
class ITK_EXPORT VahineElmVcaFilter
  : public itk::ImageToImageFilter<TImage, TImage> {
public:
  typedef TImage VectorImageType;

  typedef VahineElmVcaFilter                                        Self;
  typedef itk::ImageToImageFilter<VectorImageType, VectorImageType> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineElmVcaFilter, itk::ImageToImageFilter);

  // Input/Output Image
  typedef typename VectorImageType::Pointer           VectorImagePointer;
  typedef typename VectorImageType::ConstPointer      VectorImageConstPointer;
  typedef typename VectorImageType::PixelType         PixelType;
  typedef typename VectorImageType::InternalPixelType InternalPixelType;
  typedef typename VectorImageType::RegionType        RegionType;
  typedef typename VectorImageType::SizeType          SizeType;

  // type for computation
  const static unsigned int Dimension = 2;
  typedef Image<InternalPixelType, Dimension> BandType;

  typedef VahineVCAFilter<VectorImageType>   VCAFilterType;
  typedef typename VCAFilterType::Pointer    VCAFilterPointer;
  typedef typename std::vector<unsigned int> IntegerList;

  bool GetDebug(){
    return true;
  };
  itkGetMacro( NbComponents, unsigned int );
  itkSetMacro( NbComponents, unsigned int );
  IntegerList GetMaxLikeHood();

  /**
   * MVSAFilter produces an image with same resolution that input image but
   * with a different number of bands. As such, this filter needs to provide an
   * implementation for GenerateOutputInformation() in order
   * to inform the pipeline execution model.
   */
  virtual void GenerateOutputInformation();

  // for testing
  friend class ElmVcaFilterFixture;
protected:
  VahineElmVcaFilter();
  virtual ~VahineElmVcaFilter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateData();

  unsigned int m_NumberOfBand;     // protected for testing
private:
  // internal type for computation
  typedef vnl_matrix<double>            VnlMatrix;
  typedef vnl_vector<double>            VnlVector;
  typedef vnl_vector<InternalPixelType> InternalVnlVector;
  typedef vnl_matrix<InternalPixelType> InternalVnlMatrix;

  typedef itk::ImageRegionConstIterator<VectorImageType>             ConstIteratorType;
  typedef itk::ImageRegionIterator<VectorImageType>                  IteratorType;
  typedef otb::StreamingStatisticsVectorImageFilter2<VectorImageType> StatsFilterType;
  typedef typename StatsFilterType::Pointer                          StatsFilterPointer;

  typedef ImageList<BandType>                                          ImageListType;
  typedef typename ImageListType::Pointer                              ImageListPointer;
  typedef typename ImageListType::ConstIterator                        ConstListIteratorType;
  typedef VectorImageToImageListFilter<VectorImageType, ImageListType> DecomposeFilterType;
  typedef typename DecomposeFilterType::Pointer                        DecomposeFilterPointer;
  typedef StreamingStatisticsImageFilter<BandType>                     BandStatsFilterType;
  typedef typename BandStatsFilterType::Pointer                        BandStatsFilterPointer;

  /** Typedef for statistic computing. */
  typedef StreamingStatisticsVectorImageFilter2<VectorImageType>         StreamingStatisticsVectorImageFilterType;
  typedef typename StreamingStatisticsVectorImageFilterType::MatrixType MatrixType;

  typedef typename itk::Statistics::ListSample< itk::VariableLengthVector< InternalPixelType > > SampleType;
  typedef typename itk::Statistics::CovarianceCalculator< SampleType >
  CovarianceAlgorithmType;
  typedef typename CovarianceAlgorithmType::Pointer
  CovarianceAlgorithmPointer;

  std::numeric_limits<InternalPixelType> m_RealLimits;
  unsigned int m_NbComponents;
  IntegerList m_MaxLikeHood;
  VnlVector m_Mean;

  VnlMatrix myCovariance(VectorImagePointer image, unsigned int nbObserv);

  VnlMatrix streamingCovariance(VectorImagePointer image, unsigned int nbObserv);

  unsigned int Demelange(unsigned int);

  void LocalMaximum(VnlVector & likeHood);

  void SpectralMean(VnlVector & mean);

  void LikelihoodLog(VnlVector & eigenR, VnlVector & eigenK, unsigned int N, VnlVector & lH);

  VahineElmVcaFilter(const Self&);     //not implemented
  void UpdateNumberOfComponents();

  void operator=(const Self&);     //not implemented

  static double mysquare(const double x){
    return x*x;
  }
  static double mylog(const double x){
    return log(x);
  }

};

} // end namesapce otb

#include "ElmVcaFilter.txx"

#endif /* MVSAFILTER_H_ */
