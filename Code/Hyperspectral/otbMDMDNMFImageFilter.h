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

#ifndef __otbMDMDNMFImageFilter_h
#define __otbMDMDNMFImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkConstShapedNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkImageRegionIterator.h"
#include "itkListSample.h"
#include "itkMeanCalculator.h"
#include "itkCovarianceCalculator.h"
#include "itkVariableSizeMatrix.h"
#include "otbVectorImageToMatrixImageFilter.h"

#include "vnl/vnl_matrix.h"
#include "vnl/vnl_vector.h"

namespace otb
{
/** \class otbMDMDNMFImageFilter
 * \brief Local-RX detector algorithm with multichannel VectorImage data as input
 *
 *
 * \ingroup ImageFilters
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MDMDNMFImageFilter:
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:

  /** Standard class typedefs. */
  typedef MDMDNMFImageFilter                                                     Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage >       Superclass;
  typedef itk::SmartPointer<Self>                                                         Pointer;
  typedef itk::SmartPointer<const Self>                                             ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(MDMDNMFImageFilter, ImageToImageFilter);

  /** typedef related to input and output images */
  typedef TInputImage                                                        InputImageType;
  typedef typename InputImageType::Pointer              InputPointerType;
  typedef typename InputImageType::ConstPointer       InputConstPointerType;
  typedef typename InputImageType::IndexType              InputIndexType;
  typedef typename InputImageType::SizeType              InputSizeType;

  typedef TOutputImage                                                  OutputImageType;
  typedef typename OutputImageType::Pointer              OutputPointerType;
  typedef typename OutputImageType::IndexType              OutputIndexType;
  typedef typename OutputImageType::OffsetType       OutputOffsetType;
  typedef typename OutputImageType::SizeType              OutputSizeType;
  typedef typename OutputImageType::RegionType       OutputImageRegionType;

  /* Matrix type */
  typedef double PrecisionType;
  typedef vnl_matrix<PrecisionType> MatrixType;
  typedef typename otb::VectorImageToMatrixImageFilter<InputImageType> VectorImageToMatrixImageFilterType;
 
  /* vector type */
  typedef vnl_vector<double> VectorType;
  /** Main computation method */
  virtual void GenerateInputRequestedRegion();
  virtual void GenerateData();

  MatrixType GetEndmembersMatrix() const
  {
    return m_Endmembers;
  }
  void SetEndmembersMatrix(const MatrixType& m)
  {
    std::cout << "A " << m << std::endl;
    m_Endmembers = m;
  }

  /** Setters / Getters */
  itkSetMacro(MaxIter, unsigned int);
  itkGetMacro(MaxIter, unsigned int);
protected:
  MDMDNMFImageFilter();
  virtual ~MDMDNMFImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** GenerateOutputInformation
   * Propagate vector length info and modify if needed
   */
  virtual void GenerateOutputInformation();

private:
  MDMDNMFImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void AddOneRowOfOnes(const MatrixType & m,MatrixType & M) const;
  
  double Criterion(const MatrixType & X,
		   const MatrixType & A,
		   const MatrixType & S,
		   const double &delt,
		   const double &lambdS,
		   const double &lambdD) const;

  void EvalGradS(const MatrixType &X,
		 const MatrixType &A,
		 const MatrixType &S,
		 const double &delt,
		 const double &lambdS,
		 MatrixType & gradS) const;

  void EvalGradA(const MatrixType &X,
		 const MatrixType &A,
		 const MatrixType &S,
		 const double &delt,
		 const double &lambdD,
		 MatrixType &gradA) const;

  double Call(const MatrixType & variMat,
	      const MatrixType & fixedMat,
	      const MatrixType & X,
	      const double & delt,
	      const double & lambdS,
	      const double & lambdD, const bool isDirectEvalDirection) const;
  
  void ProjGradOneStep(
    const MatrixType & X,
    const MatrixType & fixedMat,
    const MatrixType & gradVariMat,
    const double & sig,
    const double & betinit,
    const double & delt,
    const double & lambdS,
    const double & lambdD,
    MatrixType & variMat,
    double & alph, const bool isDirectEvalDirection ) const;

  void SetNegativeCoefficientsToZero(MatrixType & M) const;
  
  MatrixType TermByTermMatrixProduct(const MatrixType & M1, const MatrixType & M2) const;

  double SumMatrixElements(const MatrixType & M) const;
  
  bool ArmijoTest(const double & sig,
		  const MatrixType variMat,
		  const MatrixType & newVariMat,
		  const double & evalf,
		  const double & newEvalf,
		  const MatrixType & gradVariMat,
		  const double & alph) const;
  // attribute
  MatrixType m_Endmembers;
  unsigned int m_MaxIter;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMDMDNMFImageFilter.txx"
#endif

#endif
