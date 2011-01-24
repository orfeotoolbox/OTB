/*
 * GrsirFilter.h
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEGRSIRFILTER_H
#define __VAHINEGRSIRFILTER_H

#include "VahineMacro.h"

#include <utility>
#include <set>
#include <list>
#include <vnl/vnl_vector_ref.h>
#include <vnl/algo/vnl_real_eigensystem.h>
#include <boost/lexical_cast.hpp>

#include "gdal.h"
#include "otbImage.h"

#include "otbPerBandVectorImageFilter.h"
#include "itkMeanImageFilter.h"
#include "CenteringImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "itkMatrix.h"
#include "itkArray.h"
#include "itkVariableSizeMatrix.h"

#include "VahineInterpol.h"
#include "VahineSlice.h"
#include "TikonovFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "InverseFilter.h"
#include "VahineXml.h"

namespace otb {

class GrsirException { };

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

template <class TInputImage, class TOutputImage>
class ITK_EXPORT VahineGrsirFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage> {
public:

  typedef VahineGrsirFilter                                  Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineGrsirFilter, itk::ImageToImageFilter);

  // Type to use for computation
  const static unsigned int Dimension = 2;
  typedef TInputImage                                              InputImageType;
  typedef typename InputImageType::Pointer                         InputImagePointer;
  typedef typename InputImageType::PixelType                       PixelType;
  typedef typename InputImageType::InternalPixelType               InternalPixelType;
  typedef typename InputImageType::IndexType                       IndexType;
  typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef typename itk::VariableSizeMatrix<RealType>               MatrixType;

  typedef Image<InternalPixelType, Dimension> BandType;
  typedef typename BandType::PixelType        BandPixelType;
  typedef typename BandType::IndexType        BandIndexType;
  typedef typename BandType::Pointer          BandPointer;
  typedef itk::ImageRegionIterator<BandType>  BandIterator;
  typedef ImageList<BandType>                 ImageBandListType;
  typedef typename ImageBandListType::Pointer ImageBandListPointer;

  typedef InputImageType                                 VectorImageType;
  typedef typename VectorImageType::Pointer              VectorImagePointer;
  typedef itk::ImageRegionConstIterator<VectorImageType> ConstVectorIteratorType;

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
  typedef otb::VahineTikonovFilter<VectorImageType>     TikonovFilterType;

  typedef typename std::list<vahine::Slice<InternalPixelType> > SliceListType;
  typedef typename SliceListType::iterator                      IteratorSliceListType;

  typedef XmlLog* XmlLogPointer;

  // members accessor
  itkGetMacro( RegulParams, RegulVectorPointer);
  itkSetMacro( RegulParams, RegulVectorPointer);
  itkGetMacro( NSlice, int);
  itkSetMacro( NSlice, int);

  itkSetMacro( Noise, VectorImagePointer );
  itkSetMacro( Learning, VectorImagePointer );
  itkSetMacro( LearningParam, VectorImagePointer );

  itkGetMacro( Epsilon, float);
  itkSetMacro( Epsilon, float);

  itkSetMacro( SearchBestRegulParam, bool );

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

protected:
  VahineGrsirFilter();
  typedef itk::CenteringImageFilter<BandType, BandType>                                   CenteringFilterType;
  typedef PerBandVectorImageFilter<VectorImageType, VectorImageType, CenteringFilterType> PerBandVectorImageFilterType;
  void GenerateData();

private:

  /*** INTERNAL TYPE ***/

  // ParameterPairType :
  // * first element is Y value (float)
  // * second element is its index
  typedef std::pair<RealType, BandIndexType>                               ParameterPairType;
  typedef std::list<ParameterPairType>                                     ParameterListType;
  typedef typename ParameterListType::const_iterator                       ParameterListIteratorType;
  typedef std::vector<unsigned int>                                        BandNumberListType;
  typedef BandNumberListType::iterator                                     BandNumberListIterator;
  typedef itk::ImageRegionConstIterator<BandType>                          ConstIteratorType;
  typedef MultiToMonoChannelExtractROI<BandPixelType, BandPixelType>       ExtractChannelType;
  typedef ImageListToVectorImageFilter<ImageBandListType, VectorImageType> ImageListToVectorImageFilterType;
  typedef typename ImageListToVectorImageFilterType::Pointer               ImageListToVectorImageFilterPointer;

  typedef VahineInverseFilter<InputImageType, BandType> VahineInverseType;
  typedef typename VahineInverseType::Pointer           VahineInversePointer;

  itkSetMacro( Parameters, BandPointer );

  struct EigenStruct {
    double nrmse;
    VnlVectorType eigenVector;
    VahineInterpol<RealType> * interpolation;
    };

  /*** END INTERNAL TYPE ***/

  RegulVectorPointer m_RegulParams;
  MatrixType m_RegularizationMatrix;
  MatrixType m_CovarianceMatrix;
  int m_NSlice;
  BandPointer m_Parameters;
  VectorImagePointer m_Learning;
  VectorImagePointer m_Noise;
  VectorImagePointer m_LearningParam;
  bool m_SearchBestRegulParam;

  float m_Epsilon;

  SliceListType m_SliceList;
  BandNumberListType m_LearningParamBand;

  typename CenteringFilterType::Pointer m_CenteringFilter;
  typename PerBandVectorImageFilterType::Pointer m_CenteringVectorFilter;
  typename TikonovFilterType::Pointer m_TikonovFilter;
  BandPointer m_currentImageInv;
  ImageBandListPointer m_ImageBandList;
  VectorImagePointer m_LearningCenteringImage;

  VectorImagePointer m_OutputImage;

  XmlLogPointer m_XmlLog;
  //VahineInversePointer m_Inverse;

  VahineGrsirFilter(const Self&); //not implemented
  void operator=(const Self&);    //not implemented

  void Slicing(ParameterListType parameterList, VectorImagePointer image);

  void MakeMean(unsigned int nObserv);

  void UpdateTikonovFilter(double regulParam);

  void EstimateEDR(VnlMatrixType &gamma, unsigned int numberOfBand, EigenStruct &eigenStruct);

  double CalculateSIRC(VnlMatrixType &gamma, VnlVectorType &axe);

  void Inverse(unsigned int numberOfBand, unsigned int nObserv);

  void InverseImage(EigenStruct eigenStruct, InputImagePointer image);

  void CalculateNRMSE(EigenStruct &eigenStruct, IteratorProjType &itLearningNoiseEstim);

  void CalculateEigenVector(VnlMatrixType &gamma, EigenStruct &eigenStruct);

};

} // End namespace otb

#include "GrsirFilter.txx"

#endif /* GRSIRFILTER_H_ */