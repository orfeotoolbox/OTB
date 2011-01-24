/*
 * VCAFilter.h
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

#ifndef VCAFILTER_H_
#define VCAFILTER_H_

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_random.h>
#include <vnl/algo/vnl_svd.h>
#include <vnl/algo/vnl_matrix_inverse.h>

#include <otbVectorImage.h>
#include <itkArray.h>
#include <itkImageToImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>
#include <itkVariableLengthVector.h>


namespace otb {

/** \class VCAImageFilter
 * \brief Estimates the endmembers present in a hyperspectral image
 *
 * This filter applies the VCA (Vertex Component Analysis) algorithm to a
 * hyperspectral image and outputs the estimated endmembers.
 * The output image of this filter is a one-line VectorImage containing,
 * at each pixel location, one of the estimated endmember
 *
 * References :
 * "Vertex Component Analysis: A Fast Algorithm to Unmix Hyperspectral Data",
 * José M. P. Nascimento, and José M. Bioucas Dias,
 * IEEE Transactions on Geoscience and Remote Sensing, Vol. 43, No. 4, April 2005
 *
 * \ingroup Hyperspectral
 */
template <class TImage>
class ITK_EXPORT VCAImageFilter
  : public itk::ImageToImageFilter<TImage, TImage> {
public:
  typedef TImage VectorImageType;

  typedef VCAImageFilter                                            Self;
  typedef itk::ImageToImageFilter<VectorImageType, VectorImageType> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineVCAFilter, itk::ImageToImageFilter);

  // Input/Output Image
  typedef typename Superclass::InputImageConstPointer    ImageConstPointer;
  typedef typename VectorImageType::Pointer              VectorImagePointer;
  typedef typename VectorImageType::PixelType            PixelType;
  typedef typename VectorImageType::RegionType           RegionType;
  typedef typename VectorImageType::SizeType             SizeType;
  typedef typename VectorImageType::IndexType            IndexType;
  typedef itk::ImageRegionConstIterator<VectorImageType> ConstIteratorType;
  typedef itk::ImageRegionIterator<VectorImageType>      IteratorType;
  typedef typename VectorImageType::InternalPixelType    InternalPixelType;

  // type for computation
  typedef vnl_matrix<double>              VnlMatrix;
  typedef vnl_vector<double>              VnlVector;
  typedef typename std::vector<IndexType> IndexArray;
  typedef VnlMatrix::iterator             VnlMatrixIterator;

  itkSetMacro( NumberEndMember, unsigned int );
  itkGetMacro( NumberEndMember, unsigned int );
  itkGetMacro( NumberOfBand, unsigned int);
  itkSetMacro( SNR, double );
  itkGetMacro( SNR, double );
  itkGetMacro( MixingMatrix, VnlMatrix );
  itkGetMacro( ProjectedData, VnlMatrix );
  //itkSetMacro( Y, VnlMatrix );
  itkGetMacro( Y, VnlMatrix );
  IndexArray GetPurePixelsIndex();

  void GenerateOutputInformation();

protected:
  VCAImageFilter();
  virtual ~VCAImageFilter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateData();

  unsigned int m_NumberOfBand; // protected for testing
  SizeType m_InputSize;        // protected for testing
private:
  // type for computation
  typedef typename std::vector<unsigned int> IndiceArray;

  void FillRandom(VnlVector & w);

  void VCA(VnlMatrix data);

  double SNR(VnlMatrix & M);

  double EstimateSNR(VnlMatrix & M, VnlVector mean, VnlMatrix & X);

  double TheoricalSNR(unsigned int p);

  void MeanCalculation(VnlMatrix const& M, VnlMatrix & MMean, VnlMatrix & Centered, VnlMatrix & Result,
                       VnlVector & mean);

  void ProjectivProjection(VnlMatrix const& M, VnlMatrix & Rp);

  void SubspaceProjection(VnlMatrix const& M, VnlMatrix & Rp);

  void SingularValueAndVector(VnlMatrix const& M, VnlMatrix & Ud, unsigned int p);

  void UpdateIndex();

  unsigned int arg_max(const VnlVector &v);

  unsigned int m_NumberEndMember;
  double m_SNR;
  VnlMatrix m_MixingMatrix, m_Y, m_ProjectedData;
  IndiceArray m_PurePixelsIndices;
  IndexArray m_PurePixelsIndex;
  vnl_random m_RandomGenerator;

  // TODO flag to speed calculation
  // if false only calculate mixing matrix.
  //bool m_ProjectData;

  VCAImageFilter(const Self&);    //not implemented
  void operator=(const Self&);     //not implemented

  static double mysquare(const double x){
    return x*x;
  }
  static double myabs(const double x){
    return ( (x<0) ? x* -1 : x);
  }
};

} // end namesapce otb

#include "VcaFilter.txx"

#endif /* VCAFILTER_H_ */
