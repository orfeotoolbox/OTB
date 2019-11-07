/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbVirtualDimensionality_h
#define otbVirtualDimensionality_h

#include "itkObjectFactory.h"
#include "itkLightObject.h"
#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

namespace otb
{

/** \class VirtualDimensionality
 * \brief Estimates the number of endmembers in a hyperspectral image
 *  with the Virtual Dimensionality (HFC) method
 *
 * This filter applies the HFC-VD (Virtual Dimensionality) algorithm to an
 * hyperspectral image and outputs the number of endmembers.
 * It takes as input the covariance and correlation matrices of the input data,
 * the number of observed pixel for thoses matrices estimations,
 * and outputs the number of endmembers, and the log-likelihood.
 *
 * References :
 * C.-I. Chang and Q. Du, "Estimation of number of spectrally distinct
 * signal sources in hyperspectral imagery", IEEE Transactions on
 * Geoscience and Remote Sensing, vol. 43, no. 3, mar 2004.

 * J. Wang and C.-I. Chang, "Applications of independent component
 * analysis in endmember extraction and abundance quantification for
 * hyperspectral imagery", IEEE Transactions on Geoscience and Remote
 * Sensing, vol. 44, no. 9, pp. 2601-1616, sep 2006.
 *
 * \ingroup Hyperspectral
 *
 * \ingroup OTBEndmembersExtraction
 */
template <class TPrecision>
class ITK_EXPORT VirtualDimensionality : public itk::LightObject
{
public:
  /** Standard Self typedef */
  typedef VirtualDimensionality         Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(VirtualDimensionality, itk::LightObject);

  /** Types to use for computations. */
  typedef TPrecision PrecisionType;

  typedef vnl_vector<PrecisionType> VectorType;
  typedef vnl_matrix<PrecisionType> MatrixType;

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

  void SetFAR(double falseAlarmRate)
  {
    if (falseAlarmRate < 0)
      falseAlarmRate = 0;

    if (falseAlarmRate > 1)
      falseAlarmRate = 1;

    m_FAR = falseAlarmRate;
  }

  double GetFAR()
  {
    return m_FAR;
  }

  void Compute();

  unsigned int GetNumberOfEndmembers()
  {
    return m_NumberOfEndmembers;
  }

protected:
  VirtualDimensionality();
  ~VirtualDimensionality() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  VirtualDimensionality(const Self&) = delete;
  void operator=(const Self&) = delete;

  MatrixType m_Covariance;
  MatrixType m_Correlation;

  unsigned int m_NumberOfPixels;
  unsigned int m_NumberOfEndmembers;
  double       m_FAR;
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVirtualDimensionality.hxx"
#endif

#endif
