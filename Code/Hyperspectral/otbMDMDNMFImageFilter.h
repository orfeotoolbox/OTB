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
 *  M. G. A. Huck and J. Blanc-Talon, IEEE TGRS, vol. 48, no. 6, pp. 2590-2602, 2010.
 *  A. Huck and M. Guillaume, in WHISPERS, 2010, Grenoble.
 *
 *  Let \f$ \mathbf R \f$ be the matrix of the hyperspectral data, whose
 *  \f$ I \f$ columns are the
 *  spectral pixels and the \f$ L \f$ rows are the vectorial spectral band
 *  images.  The linear mixing model can be written as follow :
 *  \f$ \begin{equation}
 *     \mathbf R=\mathbf A \mathbf S + \mathbf N= \mathbf X + \mathbf N
 *  \end{equation} \f$
 *  The \f$ I \f$ columns of \f$ \mathbf R \f$ contain the spectral pixels
 *  and the \f$ I \f$ columns of \f$ \mathbf S \f$ hold their respective sets of abundance
 *  fractions.  The \f$ J \f$ rows of \f$ \mathbf S \f$ are the abundance maps
 *  corresponding to the respective end-members. The \f$ J \f$ columns of
 *  \f$ \mathbf A \f$ are the end members spectra, and \f$ \mathbf X \f$ is the signal
 *  matrix. Both \f$ \mathbf A \f$ and \f$ \mathbf S \f$ are unknown.
 *
 *  The basic NMF formulation is to find two matrices \f$ \hat{\mathbf A} \f$ and
 *  \f$ \hat{ \mathbf S} \f$ such as:
 *  \f$ \begin{equation}
 *    \mathbf X\simeq \hat{\mathbf A} \hat{\mathbf S}
 *  \end{equation} \f$
 *  NMF based algorithms consider the
 *  properties of the dual spaces \f$ span^+(\mathbf A')\f$ and
 *  \f$ span^+(\mathbf S) \f$, in
 *  which \f$ span^+(\mathbf hbf m^1 ...\mathbf hbf m^d)=\{\mathbf hbf v=\sum_i \mathbf hbf
 *  m^i\mathbf hbf a_i|\mathbf hbf a\in \mathbf R _+^d\} \f$. The
 *  positiveness is then a fundamental assumption and is exploited to
 *  restrict the admissible solutions set.
 *
 *  A common used solution is to minimize the reconstruction quadratic
 *  error :
 *  \f$ RQE({\mathbf A}, {\mathbf S})=\|\mathbf R-{\mathbf A} {\mathbf S}\|^2_F \f$. In order to
 *  satisfy the sum-to-one constraint for hyperspectral data, a
 *  regularization term \f$ STU({\mathbf S}) \f$ can be added to the objective
 *  function.
 *
 *  A generic expression for the optimized function is \f$
 *  f(\mathbf A,\mathbf S)=\|\mathbf A \mathbf S-\mathbf R\|_{norm}+\sum_i \lambda_i
 *  D_i(\mathbf A) + \sum_j \lambda_j D_j(\mathbf S) \f$ in which \f$ \|\mathbf A
 *  \mathbf S-\mathbf R\|_{norm} \f$ is the connection-to-the-data term, and
 *  \f$ \lambda_{\{i, j\}} \f$ are regularization parameters for end members and
 *  abundances constraints \f$ D_{\{i, j\}} \f$.
 *  Huck
 *  propose an other regularization term,
 *  \f$ D_A(\mathbf A)=Tr(\mathbf A^T\mathbf A)-\frac{1}{L}Tr\left ( \mathbf A^T \1_{LL}\mathbf A
 *  \right) \f$, which ensures low spectral dispersion on endmembers.
 *  The physical
 *  motivation is based on the assuption that in most situations, the
 *  whole set of pure materials do not appear in each pixel, but
 *  selectively in multiple piece-wise convex sets. As a consequence, the mean value of the abundance,
 *  \f$ \frac{1}{J} \f$, is the least likely one.  The maximum abundance
 *  dispersion condition is given by
 *  \f$ D_s(\mathbf S)=-\|\mathbf S-\frac{1}{J}\1_{JI}\|^2_F \f$. MDMD-NMF algorithm
 *  minimizes the following function \f$ f(\mathbf A,\mathbf S) =RQE(\mathbf A,
 *  \mathbf S)+\delta.STU(\mathbf S)+\lambda_A D_A(\mathbf A)-\lambda_S D_S(\mathbf S)
 *  \f$,
 *  \f$ STU \f$ the sum-to-one constraint.
 *
 *  In the literature, NMF based optimization algorithms are mainly based
 *  on multiplicative rules, or else alternate gradient descent
 *  iterations, or else on alternate least square methods. In MDMD-NMF, the update rules
 *  at each iteration become :
 *  \f$ \begin{tabular}
 *  \mathbf S&\leftarrow &P\left [\mathbf S-\mu_S \left( \bar \mathbf A^T
 *  (\bar\mathbf A\mathbf S-\bar\mathbf R)-\lambda_S(\mathbf S-\frac{1}{J}\1_{JI})\right)\right
 *  ]\\ \nonumber \mathbf A &\leftarrow &P\left [\mathbf A-\mu_A \left(
 *  (\mathbf A\mathbf S-\mathbf R)\mathbf S^T +\lambda_A(\mathbf A-\frac{1}{L}\ \mathbf hbf
 *  1_{LL}\mathbf A)\right)\right ]
 *  \end{tabular} \f$
 *  where \f$ \mu_A\f$ and \f$\mu_S \f$
 *  are the step sizes.
 *  Huck propose a
 *  multiscale method to determine the coefficients \f$ \mu_A \f$ and
 *  \f$ \mu_S \f$.  The projection operator \f$ P \f$ at each step ensures the
 *  positivity constraint for \f$ \mathbf A \f$ and \f$ \mathbf S \f$, and \f$
 *  \bar\mathbf R \f$ and
 *  \f$ \bar\mathbf A \f$ include the sum-to-one constraint:
 *  \f$ \bar\mathbf R=\left[
 *  \begin{array}{c} \mathbf R \\
 *  \delta\cdot\1_{1I}\end{array}\right],\enspace \bar\mathbf A=\left[
 *  \begin{array}{c} \mathbf A \\
 *  \delta\cdot\1_{1J}\end{array}\right]\enspace \f$.
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
  typedef typename InputImageType::PixelType     InputPixelType;

  typedef          TOutputImage                  OutputImageType;
  typedef typename OutputImageType::Pointer      OutputPointerType;
  typedef typename OutputImageType::IndexType    OutputIndexType;
  typedef typename OutputImageType::OffsetType   OutputOffsetType;
  typedef typename OutputImageType::SizeType     OutputSizeType;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputPixelType;

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

  static void AddOneRowOfOnes(const MatrixType & m, MatrixType & M);
  
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
