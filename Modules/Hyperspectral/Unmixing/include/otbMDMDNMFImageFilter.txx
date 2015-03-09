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
#ifndef __otbMDMDNMFImageFilter_txx
#define __otbMDMDNMFImageFilter_txx

#include "otbMDMDNMFImageFilter.h"

namespace otb
{
template <class TInputImage, class TOutputImage>
MDMDNMFImageFilter<TInputImage, TOutputImage>
::MDMDNMFImageFilter()
{
  m_MaxIter = 100;
  m_CritStopValue = 00.5;
  m_Delt = 1.;
  m_LambdD = 0.01;
  m_LambdS = 0.01;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Input Endmembers Matrix: " << m_Endmembers << std::endl;
}

template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  const unsigned int nbEndmembers = m_Endmembers.columns();
  if ( nbEndmembers != 0 )
    {
    this->GetOutput()->SetNumberOfComponentsPerPixel(m_Endmembers.columns());
    }
  else
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,
                               "Endmembers matrix columns size required to know the output size",
                               ITK_LOCATION);
    }
}


template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::AddOneRowOfOnes(const MatrixType & m,
                  MatrixType & M)
{
  M.set_size(m.rows()+1, m.cols());

  for (unsigned int i=0; i<M.rows()-1; ++i)
    {
    M.set_row(i, m.get_row(i));
    }
  M.set_row(M.rows()-1, 1.0);
}


template <class TInputImage, class TOutputImage>
double
MDMDNMFImageFilter<TInputImage, TOutputImage>
::Criterion(const MatrixType & X,
            const MatrixType & A,
            const MatrixType & S,
            const double &itkNotUsed(m_Delt),
            const double &m_LambdS,
            const double &m_LambdD)
{
  // This function computes
  // f =        ||Xsu-Asu.S||_2^2 -
  //            m_LambdS * ||1/J*ones - S||_2^2 +
  //            m_LambdD * (trace(transpose(A)*A)-1/L*trace(transpose(A).ones.A));
  //
  //     =      ||E1||_2^2 -
  //            m_LambdS * ||E2||_2^2 +
  //            m_LambdD * (trace(A'*A)-1/L*trace(E3);
  //
  // where      || ||_2 is the L2 frobenius_norm,
  //                    J is the number of endmembers and
  //                    L is the number of spectral bands

  const unsigned int nbEndmembers = A.cols();
  const unsigned nbBands = A.rows();
  MatrixType Xsu, Asu, ones, E1, E2;    //, E3;
  double evalf, sumColsOfA, trE3;

  Xsu.set_size(X.rows()+1, X.cols());
  Asu.set_size(A.rows()+1, A.cols());
  AddOneRowOfOnes(A, Asu);
  AddOneRowOfOnes(X, Xsu);

  //-------   Computing the function blocs E1, E2 and E3   -------//
  // Bloc 1
  E1 = Xsu - Asu*S;

  // Bloc 2
  E2 = S - 1./ ((double) nbEndmembers); // * ones - S;

  // Computing trace(transpose(A)*A)
  double trAtA = 0;
  for (unsigned int i=0; i<A.columns(); ++i)
    {
    trAtA += A.get_column(i).two_norm() * A.get_column(i).two_norm();
    }

  // Bloc 3: computing fast trE3 = trace(transpose(A)*ones*A)
  trE3 = 0;
  for (unsigned int j=0; j<nbEndmembers; ++j)
    {
    sumColsOfA = A.get_column(j).sum();
    trE3 += sumColsOfA * sumColsOfA;
    }


/*      for (int j=0; j<nbEndmembers; ++j)
        {
                sumColsOfA(j) = A.get_column(j).sum();
        }

        E3.set_size(nbEndmembers, nbEndmembers);
        for (int j1=0; j1<nbEndmembers; ++j1)
        {
                for (int j2=0; j2<nbEndmembers; ++j2)
                {
                        E3(j1, j2) = sumColsOfA(j1)*sumColsOfA(j2);
                }
        }
*/
  //--------------------   Computing f   --------------------------//
  evalf = E1.frobenius_norm() * E1.frobenius_norm()
    - m_LambdS * E2.frobenius_norm() * E2.frobenius_norm()
    + m_LambdD * (trAtA - (1./ (static_cast <double> (nbBands)) * trE3));
  return evalf;
}

template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::EvalGradS(const MatrixType &X,
            const MatrixType &A,
            const MatrixType &S,
            const double &itkNotUsed(m_Delt),
            const double &m_LambdS,
            MatrixType & gradS)
{
  // Calculus of: gradS = 2 * Asu' * (Asu*S-Xsu) - lambd * 2 * (S - 1/J*ones(J, I));

  MatrixType Xsu, Asu;
  Xsu.set_size(X.rows()+1, X.cols());
  Asu.set_size(A.rows()+1, A.cols());
  AddOneRowOfOnes(A, Asu);
  AddOneRowOfOnes(X, Xsu);

  gradS = 2. * Asu.transpose() * (Asu*S-Xsu) - m_LambdS * 2. * (S - (1./(static_cast<double>( S.rows() ))));
}

template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::EvalGradA(const MatrixType &X,
            const MatrixType &A,
            const MatrixType &S,
            const double &itkNotUsed(m_Delt),
            const double &m_LambdD,
            MatrixType &gradA)
{
  // Compute gradA
  //    = (A*S-X) * (transpose(S)) + m_LambdD*(A-1/nbBands*ones(L, L)*A)
  //    = (A*S-X) * (transpose(S)) + m_LambdD*A- m_LambdD*/nbBands*ones(L, L)*A)

  MatrixType onesA;
  VectorType sumColulmnsOfA;
  sumColulmnsOfA.set_size(A.cols());
  unsigned int nbBands = A.rows();

  // Computing vector onesA
  for (unsigned int j=0; j<onesA.size(); ++j)
    {
    sumColulmnsOfA(j) = A.get_column(j).sum();
    }

  // Initialize gradA
  gradA = (A*S-X) * S.transpose();

  // 1st update of gradA
  gradA += A*m_LambdD;

  // 2nd and last update id gradA, row by row (for performance reasons)
  for (unsigned int i=0; i<nbBands; ++i)
    {
    gradA.set_row(i, gradA.get_row(i) - ( sumColulmnsOfA*m_LambdD / ( static_cast<double>( nbBands ) ) ) );
    }
}


template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::ProjGradOneStep(
  const MatrixType & X,
  const MatrixType & fixedMat,
  const MatrixType & gradVariMat,
  const double & sig,
  const double & betinit,
  const double & m_Delt,
  const double & m_LambdS,
  const double & m_LambdD,
  MatrixType & variMat,
  double & alph, const bool isDirectEvalDirection )

{
  double evalf, newEvalf, bet;
  evalf = Call(variMat, fixedMat, X, m_Delt, m_LambdS, m_LambdD, isDirectEvalDirection);  // compute evalf

  MatrixType newVariMat = variMat - alph*gradVariMat;
  SetNegativeCoefficientsToZero(newVariMat);
  newEvalf = Call(newVariMat, fixedMat, X, m_Delt, m_LambdS, m_LambdD, isDirectEvalDirection);  // compute newEvalf
  bool bit = ArmijoTest(sig, variMat, newVariMat, evalf, newEvalf, gradVariMat, alph);

  int count = 1;
  if (bit == true)
    {
    while (bit == true)
      {
      bet = pow(betinit, count);
      alph = alph/bet;
      newVariMat = variMat - alph*gradVariMat;
      SetNegativeCoefficientsToZero(newVariMat);
      newEvalf = Call(newVariMat, fixedMat, X, m_Delt, m_LambdS, m_LambdD, isDirectEvalDirection);
      bit = ArmijoTest(sig, variMat, newVariMat, evalf, newEvalf, gradVariMat, alph);
      ++count;
      }
    alph = alph*bet;
    newVariMat = variMat - alph*gradVariMat;
    SetNegativeCoefficientsToZero(newVariMat);
    }
  else
    {
    while (bit == false)
      {
      bet = pow(betinit, count);
      alph = alph*bet;
      newVariMat = variMat - alph*gradVariMat;
      SetNegativeCoefficientsToZero(newVariMat);
      newEvalf = Call(newVariMat, fixedMat, X, m_Delt, m_LambdS, m_LambdD, isDirectEvalDirection);
      bit = ArmijoTest(sig, variMat, newVariMat, evalf, newEvalf, gradVariMat, alph);
      ++count;
      }
    }
  variMat = newVariMat;
}


template <class TInputImage, class TOutputImage>
double
MDMDNMFImageFilter<TInputImage, TOutputImage>
::Call(const MatrixType & variMat,
       const MatrixType & fixedMat,
       const MatrixType & X,
       const double & m_Delt,
       const double & m_LambdS,
       const double & m_LambdD, const bool isDirectEvalDirection)
{
  double evalf;
  if ( isDirectEvalDirection )
    {
    evalf = Criterion(X, variMat, fixedMat, m_Delt, m_LambdS, m_LambdD);
    }
  else
    {
    evalf = Criterion(X, fixedMat, variMat, m_Delt, m_LambdS, m_LambdD);
    }
  return evalf;
}


template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::SetNegativeCoefficientsToZero(MatrixType & M)
{
  for (unsigned int i=0; i<M.rows(); ++i)
    {
    for (unsigned int j=0; j<M.cols(); ++j)
      {
      if (M(i, j)<0)
        M(i, j) = 0;
      }
    }
}

template <class TInputImage, class TOutputImage>
typename MDMDNMFImageFilter<TInputImage, TOutputImage>
::MatrixType
MDMDNMFImageFilter<TInputImage, TOutputImage>
::TermByTermMatrixProduct(const MatrixType & M1, const MatrixType & M2)
{
  MatrixType M;
  M.set_size(M1.rows(), M1.cols());
  for (unsigned int i=0; i<M.rows(); ++i)
    {
    for (unsigned int j=0; j<M.cols(); ++j)
      {
      M(i, j) = M1(i, j) * M2(i, j);
      }
    }
  return M;
}

template <class TInputImage, class TOutputImage>
double
MDMDNMFImageFilter<TInputImage, TOutputImage>
::SumMatrixElements(const MatrixType & M)
{
  double sum = 0;
  for (unsigned int i = 0; i<M.cols(); ++i)
    {
    sum += M.get_column(i).sum();
    }
  return sum;
}

template <class TInputImage, class TOutputImage>
bool
MDMDNMFImageFilter<TInputImage, TOutputImage>
::ArmijoTest(const double & sig,
             const MatrixType variMat,
             const MatrixType & newVariMat,
             const double & evalf,
             const double & newEvalf,
             const MatrixType & gradVariMat,
             const double & alph)
{
  bool bit;

  //const unsigned int I = variMat.rows();
  //const unsigned int J = variMat.cols();

  const MatrixType prod = TermByTermMatrixProduct(gradVariMat, newVariMat-variMat);
  double sumProd = SumMatrixElements(prod);

  if (newEvalf-evalf <= sig*alph*sumProd)
    bit = true;
  else
    bit = false;

  return bit;
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  this->AllocateOutputs();

  // Get the input and output pointers
  InputPointerType  inputPtr =
    const_cast< InputImageType * >( this->GetInput());
  OutputPointerType              outputPtr = this->GetOutput();

  inputPtr->Update();

  // Fill the output buffer with black pixels
  OutputPixelType zero = OutputPixelType();
  itk::NumericTraits<OutputPixelType>::SetLength(zero, outputPtr->GetNumberOfComponentsPerPixel());
  outputPtr->FillBuffer(zero);

  //Adaptation of contribution from A. Huck
  //Convert input image into matrix
  typename VectorImageToMatrixImageFilterType::Pointer inputImage2Matrix = VectorImageToMatrixImageFilterType::New();
  inputImage2Matrix->SetInput(inputPtr);
  inputImage2Matrix->Update();

  //useful const variables
  const unsigned int nbEndmembers = m_Endmembers.columns();
  const unsigned int nbComponentsPerPixel = inputPtr->GetNumberOfComponentsPerPixel();
  const unsigned int nbPixels = inputPtr->GetLargestPossibleRegion().GetNumberOfPixels();

  // otbGenericMsgDebugMacro( << "nbEndmembers " << nbEndmembers );
  // otbGenericMsgDebugMacro( << "nbComponentsPerPixel " << nbComponentsPerPixel );
  // otbGenericMsgDebugMacro( << "nbPixels " << nbPixels );

  // Other declarations
  double critA, critS, crit = 1;
  const unsigned int divisorParam = 10;

  // Tunning the optimized function parameters
  //const double m_Delt =                       1.;
  //const double m_LambdD =                     0.01;
  m_LambdS *= nbEndmembers;

  // Tunning the projected gradient parameters
  double sig = 0.05;
  double bet = 0.99;
  double alphA = 1.;
  double alphS = 1.;

  MatrixType X = inputImage2Matrix->GetMatrix();
  //otbGenericMsgDebugMacro( << "X " << X  );
  //-------   A and S declaration and initialization   --------//
  //---> These values fit with the ones chosen in the matlab
  //---"nmf_validationOtb.m" function to validate the OTB code.

  //A is the endmembers matrix? Output from VCA input of mdmd
  MatrixType A(this->m_Endmembers);

  MatrixType S;
  S.set_size(nbEndmembers, nbPixels);
  S.fill(1.);
  //otbGenericMsgDebugMacro( << "S " << S.cols() );
  //-----------   Declaration of useful variables   -----------//

  MatrixType Sold, Sdiff;
  Sold.set_size(nbEndmembers, nbPixels);
  Sdiff.set_size(nbEndmembers, nbPixels);

  MatrixType Aold, Adiff;
  Aold.set_size(nbComponentsPerPixel, nbEndmembers);
  Adiff.set_size(nbComponentsPerPixel, nbEndmembers);

  MatrixType gradS;
  gradS.set_size(nbEndmembers, nbPixels);
  gradS.fill(0);
  MatrixType gradA;
  gradA.set_size(nbComponentsPerPixel, nbEndmembers);
  gradS.fill(0);
  //-----------------   Optimization loop   -------------------//
  //FA fA;
  //FS fS;

  otbGenericMsgDebugMacro( << "normX = " << X.fro_norm() );

  unsigned int counter = 0;

  while ((crit > m_CritStopValue) && (counter < m_MaxIter))
    {

    //----------------   Update S   -----------------//
    Sold = S;
    //otbGenericMsgDebugMacro( << "gradS1 " << gradS );
    this->EvalGradS(X, A, S, m_Delt, m_LambdS, gradS);  // Compute gradS
    //otbGenericMsgDebugMacro( << "m_LambdS " << m_LambdS );
    //otbGenericMsgDebugMacro( << "gradS " << gradS );
    if (counter%divisorParam == 0)
      {

      otbGenericMsgDebugMacro( << "Iteration = " << counter );
      otbGenericMsgDebugMacro( << "Criterion = " << Criterion(X, A, S, m_Delt, m_LambdS, m_LambdD) );
      otbGenericMsgDebugMacro( << "statGradS = " << gradS.fro_norm() );
      otbGenericMsgDebugMacro( << "gradS(0, 0) = " << gradS(0, 0) );
      otbGenericMsgDebugMacro( << "alphS = " << alphS );
      otbGenericMsgDebugMacro( << "normS = " << S.fro_norm() );
      otbGenericMsgDebugMacro( << "S(0, 0) = " << S(0, 0) );
      }

    ProjGradOneStep(X, A, gradS, sig, bet, m_Delt, m_LambdS, m_LambdD, S, alphS, false);

    if (counter%divisorParam == 0)
      {
      otbGenericMsgDebugMacro( << "alphS = " << alphS );
      otbGenericMsgDebugMacro( << "normS = " << S.fro_norm() );
      otbGenericMsgDebugMacro( << "S(0, 0) = " << S(0, 0) );
      }

    //----------------   Update A   -----------------//
    Aold = A;

    this->EvalGradA(X, A, S, m_Delt, m_LambdD, gradA);  // Compute gradS

    if (counter%divisorParam == 0)
      {
      otbGenericMsgDebugMacro( << "gradA(0, 0) = " << gradA(0, 0) );
      }


    if (counter%divisorParam == 0)
      {
      otbGenericMsgDebugMacro( << "alphA = " << alphA );
      otbGenericMsgDebugMacro( << "normA = " << A.fro_norm() );
      otbGenericMsgDebugMacro( << "A(0, 0) = " << A(0, 0) );
      }
    ProjGradOneStep(X, S, gradA, sig, bet, m_Delt, m_LambdS, m_LambdD, A, alphA, true);

    if (counter%divisorParam == 0)
      {
      otbGenericMsgDebugMacro( << "alphA = " << alphA );
      otbGenericMsgDebugMacro( << "normA = " << A.fro_norm() );
      otbGenericMsgDebugMacro( << "A(0, 0) = " << A(0, 0) );
      }

    //------------   crit evaluation   --------------//
    Adiff = Aold-A;
    critA = Adiff.absolute_value_max();
    Sdiff = Sold - S;
    critS = Sdiff.absolute_value_max();
    crit = std::max(critA, critS);

    if (counter%divisorParam == 0)
      {
      otbGenericMsgDebugMacro( << "critA value: " <<  critA );
      otbGenericMsgDebugMacro( << "critS value: " <<  critS );
      otbGenericMsgDebugMacro( << "crit value: " <<  crit );
      otbGenericMsgDebugMacro( << "criterion value: " <<  Criterion(X, A, S, m_Delt, m_LambdS, m_LambdD) );
      }

    ++counter;
    }

  //---   Putting the rows of in the bands of the output vector image   ---//
  //TODO
  // Could be impoved choosing an imageList for the abundance maps
  // and a vector list for the endmember spectra (columns of A).

  itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputPtr->GetRequestedRegion());

  typename OutputImageType::PixelType vectorPixel;
  vectorPixel.SetSize(outputPtr->GetNumberOfComponentsPerPixel());

  unsigned int i = 0;
  outputIt.GoToBegin();
  while ( !outputIt.IsAtEnd() )
    {
    for (unsigned int j=0; j<nbEndmembers; ++j)
      {
      vectorPixel.SetElement(j, S(j, i));
      }
    outputIt.Set(vectorPixel);
    ++i;
    ++outputIt;
    }
}

/**
*
*/
template <class TInputImage, class TOutputImage>
void
MDMDNMFImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // Call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // Get pointers to the input and output
  InputPointerType  inputPtr =
    const_cast< InputImageType * >( this->GetInput());
  OutputPointerType outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  inputPtr->SetRequestedRegion( inputPtr->GetLargestPossibleRegion() );
}

} // end namespace otb

#endif
