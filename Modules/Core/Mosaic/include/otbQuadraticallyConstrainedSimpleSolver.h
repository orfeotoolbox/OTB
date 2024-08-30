/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2016-2019 IRSTEA
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
#ifndef QuadraticallyConstrainedSimpleSolver_H_
#define QuadraticallyConstrainedSimpleSolver_H_

#include "itkObjectFactory.h"
#include "itkLightObject.h"
#include "itkNumericTraits.h"
#include <vnl/vnl_matrix.h>
#include "vnl/algo/vnl_solve_qp.h"

namespace otb
{

/**
 * \class QuadraticallyConstrainedSimpleSolver
 * \brief Solves the optimisation problem for radiometric harmonisation of multiple
 * overlapping images.
 *
 * This solver inputs statistics of the overlapping images, and produces a
 * zero-y intercept linear correction model. Various cost functions can be
 * employed: RMSE based, Mean based, Mean+Standard deviation based, and Mean
 * + weighted Standard deviation bases.
 *
 * Inputs:
 * -N x N Matrix of mean of overlaps ij
 * -N x N Matrix of standard deviation of overlaps ij
 * -N x N Matrix of area of overlaps ij
 * -N x N Matrix of mean of pixels products of overlaps ij
 *
 * For all i and j, m_{ij} = stats of image i in overlap ij
 *
 * Output:
 * N x 1 Vector of scales to apply to images
 *
 *  For more details, see Cresson, Remi, and Nathalie Saint-Geours.
 *  "Natural color satellite image mosaicking using quadratic programming in decorrelated color space."
 *  IEEE Journal of Selected Topics in Applied Earth Observations and Remote Sensing 8.8 (2015): 4151-4162.
 *
 *  https://doi.org/10.1109/JSTARS.2015.2449233
 *  https://hal.archives-ouvertes.fr/hal-01373314/file/cresson2015.pdf
 *
 * \ingroup OTBMosaic
 */

template <class ValueType>
class ITK_EXPORT QuadraticallyConstrainedSimpleSolver : public itk::LightObject
{
public:
  /** Standard class typedef */
  typedef QuadraticallyConstrainedSimpleSolver Self;
  typedef itk::LightObject                     Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(QuadraticallyConstrainedSimpleSolver, LightObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Typedefs */
  typedef vnl_matrix<ValueType>     RealMatrixType;
  typedef vnl_vector<ValueType>     RealVectorType;
  typedef vnl_matrix<double>        DoubleMatrixType;
  typedef vnl_vector<double>        DoubleVectorType;
  typedef std::vector<unsigned int> ListIndexType;

  /** Enum for objective function type */
  enum ObjectiveFunctionType
  {
    Cost_Function_rmse,          // Root mean square error based
    Cost_Function_musig,         // Mean and standard deviation based
    Cost_Function_mu,            // Mean based
    Cost_Function_weighted_musig // Mean and weighted standard deviation based
  };

  /** Mean-in-overlaps matrix */
  void SetMeanInOverlaps(const RealMatrixType& matrix)
  {
    m_MeanInOverlaps = RealMatrixType(matrix);
  }
  const RealMatrixType GetMeanInOverlaps()
  {
    return m_MeanInOverlaps;
  }

  /** Standard-deviation-in-overlaps matrix  */
  void SetStandardDeviationInOverlaps(const RealMatrixType& matrix)
  {
    m_StandardDeviationInOverlaps = RealMatrixType(matrix);
  }
  const RealMatrixType GetStandardDeviationInOverlaps()
  {
    return m_StandardDeviationInOverlaps;
  }

  /** Area-in-overlaps matrix */
  void SetAreaInOverlaps(const RealMatrixType& matrix)
  {
    m_AreaInOverlaps = RealMatrixType(matrix);
  }
  const RealMatrixType GetAreaInOverlaps()
  {
    return m_AreaInOverlaps;
  }

  /** Mean-of-pixels-products-in-overlaps matrix */
  void SetMeanOfProductsInOverlaps(const RealMatrixType& matrix)
  {
    m_MeanOfProductsInOverlaps = RealMatrixType(matrix);
  }
  const RealMatrixType GetMeanOfProductsInOverlaps()
  {
    return m_MeanOfProductsInOverlaps;
  }

  /** Output correction model */
  const RealVectorType GetOutputCorrectionModel()
  {
    return m_OutputCorrectionModel;
  }

  /**
   * STD weight in harmonization
   * if value is near 0, importance is accorded to MEAN
   * if value is 1, importance is the same than MEAN
   * if value is higher than 1, importance is accorder to STD
   */
  void SetWeightOfStandardDeviationTerm(ValueType weight)
  {
    m_WeightOfStandardDeviationTerm = weight;
  }
  ValueType GetWeightOfStandardDeviationTerm()
  {
    return m_WeightOfStandardDeviationTerm;
  }

  /** Solving routine */
  void Solve();

  /** Set the cost function type */
  void SetMeanBased()
  {
    oft = Cost_Function_mu;
  }
  void SetMeanAndStandardDeviationBased()
  {
    oft = Cost_Function_musig;
  }
  void SetRMSEBased()
  {
    oft = Cost_Function_rmse;
  }
  void SetWeightedMeanAndStandardDeviationBased()
  {
    oft = Cost_Function_weighted_musig;
  }

protected:
  QuadraticallyConstrainedSimpleSolver();
  virtual ~QuadraticallyConstrainedSimpleSolver();

private:
  // Check inputs
  void CheckInputs(void) const;

  // Deep First Search
  void DFS(std::vector<bool>& marked, unsigned int s) const;

  // Compute the objective matrix
  const DoubleMatrixType GetQuadraticObjectiveMatrix(const DoubleMatrixType& areas, const DoubleMatrixType& means, const DoubleMatrixType& stds,
                                                     const DoubleMatrixType& mops);

  // Extract a sub matrix from indices list
  const DoubleMatrixType ExtractMatrix(const RealMatrixType& mat, const ListIndexType& idx);

  // Input
  RealMatrixType m_MeanInOverlaps;
  RealMatrixType m_StandardDeviationInOverlaps;
  RealMatrixType m_AreaInOverlaps;
  RealMatrixType m_MeanOfProductsInOverlaps;

  // Params
  ValueType m_WeightOfStandardDeviationTerm; // could be manually tuned for different results

  // Output correction models
  RealVectorType m_OutputCorrectionModel;

  // objective function type (enum)
  ObjectiveFunctionType oft;
};

} /* namespace otb */
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbQuadraticallyConstrainedSimpleSolver.hxx"
#endif
#endif /* QuadraticallyConstrainedSimpleSolver_H_ */
