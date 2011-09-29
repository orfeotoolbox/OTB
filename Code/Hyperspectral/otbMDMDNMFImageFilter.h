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
#include "itkVariableSizeMatrix.h"
#include "otbVectorImageToMatrixImageFilter.h"

#include "vnl/vnl_matrix.h"
#include "vnl/vnl_vector.h"

namespace otb
{
/** \class otbMDMDNMFImageFilter
 *  \brief This filter implements unmixing based non negative matrix factorization (NMF) which
 *  finds simultaneously the end members and abundances matrix which
 *  product is the closer to the observed data, based on the following
 *  works:
 *  K. S. F.J. Theis and T. Tanaka, First results on uniqueness of
    sparse non-negative matrix factorisation.
 *  M. G. A. Huck and J. Blanc-Talon, IEEE TGRS, vol. 48, no. 6,pp. 2590-2602, 2010.
 *  A. Huck and M. Guillaume, in WHISPERS, 2010, Grenoble.
 * 
 *  Let $\matR$ be the matrix of the hyperspectral data, whose $I$ columns are the
 *  spectral pixels and the $L$ rows are the vectorial spectral band
 *  images.  The linear mixing model can be written as follow :
 *  \begin{equation} 
 *     \matR=\matA \matS + \matN= \matX + \matN
 *  \end{equation} 
 *  The $I$ columns of $\matR$ contain the spectral pixels
 *  and the $I$ columns of $\matS$ hold their respective sets of abundance
 *  fractions.  The $J$ rows of $\matS$ are the abundance maps
 *  corresponding to the respective end-members. The $J$ columns of
 *  $\matA$ are the end members spectra, and $\matX$ is the signal
 *  matrix. Both $\matA$ and $\matS$ are unknown.
 *  
 *  The basic NMF formulation is to find two matrices $\hat{\matA}$ and
 *  $\hat{ \matS}$ such as: 
 *  \begin{equation} 
 *    \matX\simeq \hat{\matA} \hat{\matS} 
 *  \end{equation} 
 *  NMF based algorithms consider the
 *  properties of the dual spaces $span^+(\matA')$ and $span^+(\matS)$,in
 *  which $span^+(\mathbf m^1 ...\mathbf m^d)=\{\mathbf v=\sum_i \mathbf
 *  m^i\mathbf a_i|\mathbf a\in \matR _+^d\}$. The
 *  positiveness is then a fundamental assumption and is exploited to
 *  restrict the admissible solutions set.
 *  
 *  A common used solution is to minimize the reconstruction quadratic
 *  error $RQE({\matA},{\matS})=\|\matR-{\matA} {\matS}\|^2_F$. In order to
 *  satisfy the sum-to-one constraint for hyperspectral data, a
 *  regularization term $STU(\matS)$ can be added to the objective
 *  function.
 *  
 *  A generic expression for the optimized function is $$
 *  f(\matA,\matS)=\|\matA \matS-\matR\|_{norm}+\sum_i \lambda_i
 *  D_i(\matA) + \sum_j \lambda_j D_j(\matS)$$ in which $\|\matA
 *  \matS-\matR\|_{norm}$ is the connection-to-the-data term, and
 *  $\lambda_{\{i,j\}}$ are regularization parameters for end members and
 *  abundances constraints $D_{\{i,j\}}$.
 *  In \cite{Huck2010a}, they
 *  propose an other regularization term,
 *  $D_A(\matA)=Tr(\matA^T\matA)-\frac{1}{L}Tr\left ( \matA^T \1_{LL}\matA
 *  \right)$, which ensures low spectral dispersion on endmembers.
 *  The physical
 *  motivation is based on the assuption that in most situations, the
 *  whole set of pure materials do not appear in each pixel, but
 *  selectively in multiple piece-wise convex sets., as pointed in
 *  \cite{Zare2010}. As a consequence, the mean value of the abundance,
 *  $\frac{1}{J}$, is the least likely one.  The maximum abundance
 *  dispersion condition is given by
 *  $D_s(\matS)=-\|\matS-\frac{1}{J}\1_{JI}\|^2_F$. MDMD-NMF algorithm
 *  minimizes the following function $ f(\matA,\matS) =RQE(\matA,
 *  \matS)+\delta.STU(\matS)+\lambda_A D_A(\matA)-\lambda_S D_S(\matS)$,
 *  $STU$ the sum-to-one constraint.
 *  
 *  In the literature, NMF based optimization algorithms are mainly based
 *  on multiplicative rules, or else alternate gradient descent
 *  iterations, or else on alternate least square methods. In MDMD-NMF, the update rules
 *  at each iteration become \begin{eqnarray}\label{algomdmd} \matS
 *  &\leftarrow &P\left [\matS-\mu_S \left( \bar \matA^T
 *  (\bar\matA\matS-\bar\matR)-\lambda_S(\matS-\frac{1}{J}\1_{JI})\right)\right
 *  ]\\ \nonumber \matA &\leftarrow &P\left [\matA-\mu_A \left(
 *  (\matA\matS-\matR)\matS^T +\lambda_A(\matA-\frac{1}{L}\ \mathbf
 *  1_{LL}\matA)\right)\right ] \end{eqnarray} where $\mu_A$ and $\mu_S$
 *  are the step sizes.  % and the projection.  % stands for
 *  non-negativity hard constraint : $P(M_{ij})=0$ if $M_{ij}<0$ for
 *  $\mathbf %M=\{\matA,\matS\}$.  Huck propose a
 *  multiscale method to determine the coefficients $\mu_A$ and
 *  $\mu_S$.  The projection operator $P$ at each step ensures the
 *  positivity constraint for $\matA$ and $\matS$, and $\bar\matR$ and
 *  $\bar\matA$ include the sum-to-one constraint: $ \bar\matR=\left[
 *  \begin{array}{c} \matR \\
 *  \delta\cdot\1_{1I}\end{array}\right],\enspace \bar\matA=\left[
 *  \begin{array}{c} \matA \\
 *  \delta\cdot\1_{1J}\end{array}\right]\enspace$.
 * 
 * \ingroup ImageFilters
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MDMDNMFImageFilter:
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:

  /** Standard class typedefs. */
  typedef MDMDNMFImageFilter                                    Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage >  Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(MDMDNMFImageFilter, ImageToImageFilter);

  /** typedef related to input and output images */
  typedef          TInputImage                   InputImageType;
  typedef typename InputImageType::Pointer       InputPointerType;
  typedef typename InputImageType::ConstPointer  InputConstPointerType;
  typedef typename InputImageType::IndexType     InputIndexType;
  typedef typename InputImageType::SizeType      InputSizeType;

  typedef          TOutputImage                  OutputImageType;
  typedef typename OutputImageType::Pointer      OutputPointerType;
  typedef typename OutputImageType::IndexType    OutputIndexType;
  typedef typename OutputImageType::OffsetType   OutputOffsetType;
  typedef typename OutputImageType::SizeType     OutputSizeType;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;

  /* Matrix type */
  typedef double                                                       PrecisionType;
  typedef vnl_matrix<PrecisionType>                                    MatrixType;
  typedef typename otb::VectorImageToMatrixImageFilter<InputImageType> VectorImageToMatrixImageFilterType;
  
  /* vector type */
  typedef vnl_vector<double> VectorType;

  MatrixType GetEndmembersMatrix() const
  {
    return m_Endmembers;
  }
  void SetEndmembersMatrix(const MatrixType& m)
  {
    m_Endmembers = m;
  }

  /** Setters / Getters */
  itkSetMacro(MaxIter, unsigned int);
  itkGetMacro(MaxIter, unsigned int);
  
  itkSetMacro(CritStopValue, double);
  itkGetMacro(CritStopValue, double);
  
  itkSetMacro(Delt, double);
  itkGetMacro(Delt, double);
  
  itkSetMacro(LambdD, double);
  itkGetMacro(LambdD, double);
  
  itkSetMacro(LambdS, double);
  itkGetMacro(LambdS, double);

protected:
  MDMDNMFImageFilter();
  virtual ~MDMDNMFImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateInputRequestedRegion();

  /** GenerateOutputInformation
   * Propagate vector length info and modify if needed
   */
  virtual void GenerateOutputInformation();

  /** Main computation method */
  virtual void GenerateData();


private:
  MDMDNMFImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  static void AddOneRowOfOnes(const MatrixType & m,MatrixType & M);
  
  static double Criterion(const MatrixType & X,
                   const MatrixType & A,
                   const MatrixType & S,
                   const double &delt,
                   const double &lambdS,
                   const double &lambdD);

  static void EvalGradS(const MatrixType &X,
                 const MatrixType &A,
                 const MatrixType &S,
                 const double &delt,
                 const double &lambdS,
                 MatrixType & gradS);

  static void EvalGradA(const MatrixType &X,
                 const MatrixType &A,
                 const MatrixType &S,
                 const double &delt,
                 const double &lambdD,
                 MatrixType &gradA);

  static double Call(const MatrixType & variMat,
              const MatrixType & fixedMat,
              const MatrixType & X,
              const double & delt,
              const double & lambdS,
              const double & lambdD, const bool isDirectEvalDirection);
  
  static void ProjGradOneStep(
    const MatrixType & X,
    const MatrixType & fixedMat,
    const MatrixType & gradVariMat,
    const double & sig,
    const double & betinit,
    const double & delt,
    const double & lambdS,
    const double & lambdD,
    MatrixType & variMat,
    double & alph, const bool isDirectEvalDirection );

  static void SetNegativeCoefficientsToZero(MatrixType & M);
  
  static MatrixType TermByTermMatrixProduct(const MatrixType & M1, const MatrixType & M2);

  static double SumMatrixElements(const MatrixType & M);
  
  static bool ArmijoTest(const double & sig,
                  const MatrixType variMat,
                  const MatrixType & newVariMat,
                  const double & evalf,
                  const double & newEvalf,
                  const MatrixType & gradVariMat,
                  const double & alph);
  
  // attributes
  MatrixType   m_Endmembers;
  unsigned int m_MaxIter;
  double       m_CritStopValue;

  // Optimized function parameters
  double m_Delt;
  double m_LambdD;
  double m_LambdS;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMDMDNMFImageFilter.txx"
#endif

#endif
