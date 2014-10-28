/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANT2ABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbEigenvalueLikelihoodMaximisation_h
#define __otbEigenvalueLikelihoodMaximisation_h

#include "itkObjectFactory.h"
#include "itkLightObject.h"
#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

namespace otb
{

/** \class EigenvalueLikelihoodMaximisation
 * \brief Estimates the number of endmembers in a hyperspectral image
 *
 * This filter applies the ELM (Eigenvalue Likelihood Maximisation) algorithm to a
 * hyperspectral image and outputs the number of endmember.
 * It takes as input the covariance and correlation matrices of the input data,
 * the number of observed pixel for thoses matrices estimations,
 * and outputs the number of endmembers, and the log-likelihood.
 *
 * References :
 * "Unsupervised Endmember Extraction of Martian Hyperspectral Images",
 * B.Luo, J. Chanussot, S. Dout\'e and X. Ceamanos,
 * IEEE Whispers 2009, Grenoble France, 2009
 *
 * "Unsupervised classification of hyperspectral images by using linear unmixing algorithm",
 * Luo, B. and Chanussot, J.,
 * IEEE Int. Conf. On Image Processing(ICIP) 2009, Cairo, Egypte, 2009
 *
 * \ingroup Hyperspectral
 */
template<class TPrecision>
class ITK_EXPORT EigenvalueLikelihoodMaximisation :
  public itk::LightObject
{
public:
  /** Standard Self typedef */
  typedef EigenvalueLikelihoodMaximisation           Self;
  typedef itk::LightObject                           Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(EigenvalueLikelihoodMaximisation, itk::LightObject);

  /** Types to use for computations. */
  typedef TPrecision PrecisionType;

  typedef vnl_vector<PrecisionType>   VectorType;
  typedef vnl_matrix<PrecisionType>   MatrixType;

  void SetCovariance(const MatrixType& m)
  {
    m_Covariance = m;
  }

  void SetCorrelation(const MatrixType& m)
  {
    m_Correlation = m;
  }

  void SetNumberOfPixels(unsigned int n)
  {
    m_NumberOfPixels = n;
  }

  void Compute();

  unsigned int GetNumberOfEndmembers()
  {
    return m_NumberOfEndmembers;
  }

  const VectorType& GetLikelihood()
  {
    return m_Likelihood;
  }

protected:
  EigenvalueLikelihoodMaximisation();
  virtual ~EigenvalueLikelihoodMaximisation() {}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  EigenvalueLikelihoodMaximisation(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  MatrixType m_Covariance;
  MatrixType m_Correlation;

  unsigned int m_NumberOfPixels;
  unsigned int m_NumberOfEndmembers;
  VectorType m_Likelihood;

};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEigenvalueLikelihoodMaximisation.txx"
#endif

#endif
