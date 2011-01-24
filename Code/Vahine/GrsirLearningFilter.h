/*
 * GrsirLearningFilter.h
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

#ifndef __GRSIRLEARNINGFILTER_H_
#define __GRSIRLEARNINGFILTER_H_

#include <list>
#include <set>
#include <utility>
#include <math.h>

#include <boost/lexical_cast.hpp>
#include <gdal.h>

#include <otb/IO/otbImage.h>
#include <otb/BasicFilters/otbImageListToVectorImageFilter.h>
#include <otb/BasicFilters/otbPerBandVectorImageFilter.h>
#include <otb/BasicFilters/otbStreamingStatisticsImageFilter.h>
#include <otb/Common/otbMultiToMonoChannelExtractROI.h>

#include <itkArray.h>
#include <itkImageToImageFilter.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageLinearIteratorWithIndex.h>
#include <itkMatrix.h>

#include <vnl/vnl_vector_ref.h>
#include <vnl/algo/vnl_real_eigensystem.h>

#include "VahineMacro.h"
#include "VahineInterpol.h"
#include "VahineSlice.h"
#include "VahineXml.h"

#include "CenteringImageFilter.h"
#include "TikonovFilter.h"

namespace otb {

class GrsirLearningException { };

// functor predicat for order our custom pair
// don't use the internal pair operator< because it use the two elements for
// comparaison
// and we don't have operator< in the second type
template < typename PairType >
class SortPairIncrease {
public:
  bool operator()( const PairType & left , const PairType & right ){
    return left.first < right.first;
  }

};

template < typename PairType >
class SortPairDecrease {
public:
  bool operator()( const PairType & left , const PairType & right ){
    return left.first > right.first;
  }

};

class ManagedTypes {
public:
  // Convert a VariableLengthVector to an Array.
  template < class TElement >
  static itk::Array<TElement> ToManagedVariableLengthVector( itk::VariableLengthVector<TElement> vector ){
    typedef typename itk::Array<TElement> ArrayType;
    ArrayType result;
    result.SetSize(vector.Size() );
    for (unsigned int i=0; i<vector.Size(); ++i)
      result[i] = static_cast<TElement>(vector[i]);
    return result;
  }

};

template <class TImage>
class ITK_EXPORT VahineGrsirLearningFilter
  : public itk::ImageToImageFilter<TImage, TImage> {
public:

  typedef VahineGrsirLearningFilter               Self;
  typedef itk::ImageToImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineGrsirLearningFilter, itk::ImageToImageFilter);

  // Type to use for computation
  const static unsigned int Dimension = 2;
  typedef TImage                                      ImageType;
  typedef typename ImageType::Pointer                 ImagePointer;
  typedef typename Superclass::InputImageConstPointer ImageConstPointer;
  typedef typename ImageType::PixelType               PixelType;
  typedef typename ImageType::InternalPixelType       InternalPixelType;
  typedef typename ImageType::IndexType               IndexType;
  typedef typename ImageType::RegionType              RegionType;

  typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef typename itk::VariableSizeMatrix<RealType>               MatrixType;

  typedef Image<InternalPixelType, Dimension>         BandType;
  typedef typename BandType::PixelType                BandPixelType;
  typedef typename BandType::IndexType                BandIndexType;
  typedef typename BandType::Pointer                  BandPointer;
  typedef itk::ImageRegionIterator<BandType>          BandIterator;
  typedef itk::ImageLinearIteratorWithIndex<BandType> BandLinearIterator;
  typedef ImageList<BandType>                         ImageBandListType;
  typedef typename ImageBandListType::Pointer         ImageBandListPointer;

  typedef itk::ImageRegionConstIterator<ImageType> ConstImageIteratorType;

  typedef vnl_matrix<RealType> VnlMatrixType;
  typedef vnl_vector<RealType> VnlVectorType;

  typedef std::pair<double, VnlVectorType> EigenPairType;
  typedef std::list<EigenPairType>         EigenListType;

  typedef std::vector<double> RegulVectorType;
  typedef RegulVectorType*    RegulVectorPointer;

  typedef Image<InternalPixelType, 2>         ProjType;
  typedef typename ProjType::Pointer          ProjPointer;
  typedef  itk::ImageRegionIterator<ProjType> IteratorProjType;

  // used internal filter
  typedef otb::StreamingStatisticsImageFilter<BandType> StatsFilterType;
  typedef otb::VahineTikonovFilter<ImageType>           TikonovFilterType;

  typedef typename std::list<vahine::Slice<InternalPixelType> > SliceListType;
  typedef typename SliceListType::iterator                      IteratorSliceListType;

  typedef XmlLog* XmlLogPointer;

  /*** members accessor ***/
  /** tikonov regulation parameters */
  itkGetMacro( RegulParameters, RegulVectorPointer);
  void SetRegulParameters(RegulVectorPointer);

  itkGetMacro( NSlice, int);
  itkSetMacro( NSlice, int);

  /** learning spectrums database image and
   * learning noisy spectrums database image
   */
  itkSetMacro( LearningNoiseData, ImagePointer );
  itkSetMacro( LearningData, ImagePointer );

  /** mesurement of difference for two closest parameter value
   * if differrence is smaller than epsilon the two values are
   * in the same slice.
   */
  itkGetMacro( Epsilon, float);
  itkSetMacro( Epsilon, float);

  /** if true calculate and save only the best projection
   * use the minimal nrmse criteria */
  itkSetMacro( SaveOnlyBestRegulParam, bool );

  /** Number of eigen vector that must be calculated */
  itkGetMacro( NumberOfEigenVector, unsigned int );
  itkSetMacro( NumberOfEigenVector, unsigned int );

  void SetTikonovFilter();

  void SetXmlLog(XmlLogPointer xmlLog);

  void PushBackLearningBand(unsigned int);

  bool GetDebug(){
    return true;
  };

  void PrintSelf(std::ostream& os, itk::Indent indent) const {
    Superclass::PrintSelf(os, indent);
  };

  SliceListType &GetSliceList();

  void GenerateOutputInformation();

  void GenerateInputRequestedRegion();

protected:
  VahineGrsirLearningFilter();
  typedef itk::CenteringImageFilter<BandType, BandType>                       CenteringFilterType;
  typedef PerBandVectorImageFilter<ImageType, ImageType, CenteringFilterType> PerBandVectorImageFilterType;
  void GenerateData();

private:

  /*** INTERNAL TYPE ***/

  // ParameterPairType :
  // * first element is Y value (float)
  // * second element is its index
  typedef std::pair<RealType, BandIndexType>                         ParameterPairType;
  typedef std::list<ParameterPairType>                               ParameterListType;
  typedef typename ParameterListType::const_iterator                 ParameterListIteratorType;
  typedef std::vector<unsigned int>                                  BandNumberListType;
  typedef BandNumberListType::iterator                               BandNumberListIterator;
  typedef itk::ImageRegionConstIterator<BandType>                    ConstBandIteratorType;
  typedef MultiToMonoChannelExtractROI<BandPixelType, BandPixelType> ExtractChannelType;
  typedef ImageListToVectorImageFilter<ImageBandListType, ImageType> ImageListToVectorImageFilterType;
  typedef typename ImageListToVectorImageFilterType::Pointer         ImageListToVectorImageFilterPointer;

  itkSetMacro( Parameters, BandPointer );

  struct EigenStruct {
    double nrmse;
    VnlVectorType eigenVector;
    VahineInterpol<RealType> * interpolation;
    };

  /*** END INTERNAL TYPE ***/

  /** tikonov regulation parameters */
  RegulVectorPointer m_RegulParameters;

  MatrixType m_RegularizationMatrix;
  MatrixType m_CovarianceMatrix;
  int m_NSlice;
  BandPointer m_Parameters;

  /** learning spectrums database image and
   * learning noisy spectrums database image
   */
  ImagePointer m_LearningData;
  ImagePointer m_LearningNoiseData;

  /** mesurement of difference for two closest parameter value
       * if differrence is smaller than epsilon the two values are
       * in the same slice.
       */
  float m_Epsilon;

  /** if true calculate and save only the best projection
* use the minimal nrmse criteria */
  bool m_SaveOnlyBestRegulParam;

  /** Number of eigen vector that must be calculated */
  unsigned int m_NumberOfEigenVector;

  SliceListType m_SliceList;
  /** list of number of parameter band for grsisr calculation */
  BandNumberListType m_LearningParamBand;

  typename CenteringFilterType::Pointer m_CenteringFilter;
  typename PerBandVectorImageFilterType::Pointer m_CenteringVectorFilter;
  typename TikonovFilterType::Pointer m_TikonovFilter;
  BandPointer m_currentImageInv;
  ImageBandListPointer m_ImageBandList;
  ImagePointer m_LearningCenteringImage;

  ImagePointer m_OutputImage;

  RegionType m_OutputLargestPossibleRegion;

  XmlLogPointer m_XmlLog;

  VahineGrsirLearningFilter(const Self&); //not implemented
  void operator=(const Self&);            //not implemented

  void Slicing(ParameterListType parameterList, ImagePointer image);

  void MakeMean(unsigned int nObserv);

  void UpdateTikonovFilter(double regulParam);

  void EstimateEDR(VnlMatrixType &gamma, unsigned int numberOfBand, EigenStruct &eigenStruct);

  double CalculateSIRC(VnlMatrixType &gamma, VnlVectorType &axe);

  void CalculateNRMSE(EigenStruct &eigenStruct, IteratorProjType &itLearningNoiseEstim);

  void CalculateEigenVector(VnlMatrixType &gamma, EigenStruct &eigenStruct);

  void Learning(unsigned int numberOfBand, unsigned int nObserv);

  void AddRegularizationParameters(BandLinearIterator &itLearnedFunctionnal, unsigned int bestRegul,
                                   RegulVectorPointer regulVector);

  void AddLearnedFunctionnal(EigenStruct eigenStruct, BandLinearIterator &itLearnedFunctionnal);

  void CompleteLine(BandLinearIterator &itLearnedFunctionnal);

};

} // End namespace otb

#include "GrsirLearningFilter.txx"

#endif /* GRSIRLEARNINGFILTER_H_ */