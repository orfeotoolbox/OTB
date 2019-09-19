#ifndef QuadraticallyConstrainedSimpleSolver_txx_
#define QuadraticallyConstrainedSimpleSolver_txx_

#include "otbQuadraticallyConstrainedSimpleSolver.h"

namespace otb {

template<class ValueType> QuadraticallyConstrainedSimpleSolver<ValueType>
::QuadraticallyConstrainedSimpleSolver() {
  m_WeightOfStandardDeviationTerm = 1.0;
  oft = Cost_Function_rmse;
}

template<class ValueType> QuadraticallyConstrainedSimpleSolver<ValueType>
::~QuadraticallyConstrainedSimpleSolver() {
}

/*
 * Used to check layout topology consistency (Deep First Search)
 *
 * "m_AreaInOverlaps[i][j]>0" is equivalent to "images i and j are
 * overlapping with a non empty intersection (i.e. non null data)"
 */
template<class ValueType>
void QuadraticallyConstrainedSimpleSolver<ValueType>
::DFS(std::vector<bool> & marked, unsigned int s) const
{

  // mark the s vertex
  marked[s] = true;

  // Get neighborhood
  for (unsigned int i = 0 ; i < m_AreaInOverlaps.rows() ; i++)
    {
    if (s!=i && m_AreaInOverlaps[s][i]>0 && !marked[i])
      {
      DFS(marked, i);
      }
    }

}

/*
 * Check input matrices dimensions, and layout consistency
 */
template<class ValueType>
void QuadraticallyConstrainedSimpleSolver<ValueType>
::CheckInputs() const {

  // Check area matrix is non empty
  const unsigned int n = m_AreaInOverlaps.cols();
  if (n == 0)
    {
    itkExceptionMacro( << "Input area matrix has 0 elements");
    }

  bool inputMatricesAreValid = true;

  // Check "areas" and "means" matrices size
  if ((n != m_AreaInOverlaps.rows()) || (n != m_MeanInOverlaps.cols()) || (n != m_MeanInOverlaps.rows()))
    {
    inputMatricesAreValid = false;
    }

  // Check "std" matrix size
  if ((oft == Cost_Function_musig) || (oft==Cost_Function_weighted_musig) || (oft==Cost_Function_rmse))
    {
    if ((n != m_StandardDeviationInOverlaps.cols()) || (n != m_StandardDeviationInOverlaps.rows()))
      {
      inputMatricesAreValid = false;
      }
    }

  // Check "means of products" matrix size
  if (oft == Cost_Function_musig)
    {
    if ((n != m_MeanOfProductsInOverlaps.cols()) || (n != m_MeanOfProductsInOverlaps.rows()))
      {
      inputMatricesAreValid = false;
      }
    }

  if (!inputMatricesAreValid)
    {
    itkExceptionMacro( << "Input matrices must be square and have the same number of elements.");
    }


}

/*
 * Compute the objective function
 *
 * VNL is not sufficient: it has weak solving routines, and can not deal with QP subject to
 * a linear equality constraint plus lower limits (that is < and = linear constraints)
 * With vnl, we keep it simple and solve only the zero-y intercept linear case
 * but sometimes it fails because numerical instabilities. (vnl quadratic routines are not very reliable)
 *
 * With a good quadratic solver, we could e.g. use a general linear model (Xout = Xin*a+b)
 * Unfortunately it can be done with VNL so far. Tested & implemented successfully with
 * OOQP (Fastest, o(n)) and Quadprog++ (Fast, o(n)), and CGAL exact type solver (very slow, o(n^a) with a>1)
 * but has to rely on external dependencies...
 *
 */
template<class ValueType>
const typename QuadraticallyConstrainedSimpleSolver<ValueType>::DoubleMatrixType
QuadraticallyConstrainedSimpleSolver<ValueType>
::GetQuadraticObjectiveMatrix(const DoubleMatrixType & areas, const DoubleMatrixType & means,
		const DoubleMatrixType & stds, const DoubleMatrixType & mops)
{
    // Set STD matrix weight
    ValueType w;

    if (oft == Cost_Function_mu)
      {
      w = 0.0;
      }
    if (oft == Cost_Function_musig)
      {
      w = 1.0;
      }
    if (oft ==  Cost_Function_weighted_musig)
      {
      w = (ValueType) m_WeightOfStandardDeviationTerm;
      }

    const unsigned int n = areas.cols();

    // Temporary matrices H, K, L
    DoubleMatrixType H(n,n,0), K(n,n,0), L(n,n,0);
    DoubleMatrixType H_RMSE(n,n,0);
    for (unsigned int i = 0 ; i < n ; i++)
      {
      for (unsigned int j = 0 ; j < n ; j++)
        {
        if (i==j)
          {
          // Diag (i=j)
          for (unsigned int k = 0 ; k < n ; k++)
            {
            if (i!=k)
              {
              H[i][j] += areas[i][k] *
                (means[i][k]*means[i][k] + w*stds[i][k]*
                    stds[i][k]);
              K[i][j] += areas[i][k] * means[i][k];
              L[i][j] += areas[i][k];
              H_RMSE[i][j] += areas[i][k]*
                (means[i][k]*means[i][k]+stds[i][k]*
                    stds[i][k]);
              }
            }
          }
        else
          {
          // Other than diag (i!=j)
          H[i][j] = -areas[i][j] *
            (means[i][j]*means[j][i] + w*stds[i][j]*
                stds[j][i]);
          K[i][j] = -areas[i][j] * means[i][j];
          L[i][j] = -areas[i][j];
          H_RMSE[i][j] = -areas[i][j] * mops[i][j];
          }
        }
      }

    if (oft == Cost_Function_rmse)
      {
      H = H_RMSE;
      }

    return H;

}

/*
 * Returns the sub-matrix of mat, composed only by rows/cols in idx
 */
template<class ValueType>
const typename QuadraticallyConstrainedSimpleSolver<ValueType>::DoubleMatrixType
QuadraticallyConstrainedSimpleSolver<ValueType>
::ExtractMatrix(const RealMatrixType & mat, const ListIndexType & idx)
 {
  unsigned int n = idx.size();
  DoubleMatrixType newMat(n,n,0);
  for (unsigned int i = 0 ; i < n ; i++)
    {
    for (unsigned int j = 0 ; j < n ; j++)
      {
      unsigned int mat_i = idx[i];
      unsigned int mat_j = idx[j];
      newMat[i][j] = mat[mat_i][mat_j];
      }
    }
  return newMat;
 }

/*
 * QP Solving using vnl
 */
template<class ValueType>
void
QuadraticallyConstrainedSimpleSolver<ValueType>
::Solve()
{
  // Check matrices dimensions
  CheckInputs();

  // Display a warning if overlap matrix is null
  if (m_AreaInOverlaps.max_value() == 0)
    {
    itkExceptionMacro("No overlap in images!");
    }

  // Identify the connected components
  unsigned int nbOfComponents = m_AreaInOverlaps.rows();
  unsigned int nextVertex = 0;
  std::vector< ListIndexType > connectedComponentsIndices;
  std::vector<bool> markedVertices (nbOfComponents, false);
  bool allMarked = false;
  while (!allMarked)
    {
    // Depth First Search starting from nextVertex
    std::vector<bool> marked (nbOfComponents, false);
    DFS(marked, nextVertex);

    // Id the connected component
    ListIndexType list;
    for (unsigned int i = 0 ; i < nbOfComponents ; i++)
      {
      if (marked[i])
        {
        // Tag the connected component index
        list.push_back(i);
        markedVertices[i] = true;
        }
      else if (!markedVertices[i])
        {
        // if the i-th vertex is not marked, DFS will start from it next
        nextVertex = i;
        break;
        }
      }
    connectedComponentsIndices.push_back(list);

    // Check if vertices are all marked
    allMarked = true;
    for (unsigned int i = 0 ; i < nbOfComponents ; i++)
      if (!markedVertices[i])
        allMarked = false;
    }

  // Prepare output model
  m_OutputCorrectionModel.set_size(nbOfComponents);
  m_OutputCorrectionModel.fill(itk::NumericTraits<ValueType>::One);

  // Extract and solve all connected components one by one
  if (connectedComponentsIndices.size() > 1)
    itkWarningMacro("Seems to be more that one group of overlapping images. Trying to harmonize groups separately.");

  for (unsigned int component = 0 ; component < connectedComponentsIndices.size() ; component++)
    {
    // Indices list
    ListIndexType list = connectedComponentsIndices[component];
    const unsigned int n = list.size();

    // Extract matrices
    DoubleMatrixType sub_areas = ExtractMatrix(m_AreaInOverlaps, list);
    DoubleMatrixType sub_means = ExtractMatrix(m_MeanInOverlaps, list);
    DoubleMatrixType sub_stdev = ExtractMatrix(m_StandardDeviationInOverlaps, list);
    DoubleMatrixType sub_mOfPr = ExtractMatrix(m_MeanOfProductsInOverlaps, list);

    // Objective function
    DoubleMatrixType Q = GetQuadraticObjectiveMatrix(
        sub_areas,
        sub_means,
        sub_stdev,
        sub_mOfPr);
    DoubleVectorType g(n,0);

    // Constraint (Energy conservation)
    DoubleMatrixType A(1,n);
    DoubleVectorType b(1,0);
    for (unsigned int i = 0 ; i < n ; i++)
      {
      double energy = sub_areas[i][i] * sub_means[i][i];
      b[0] += energy;
      A[0][i] = energy;
      }

    // Solution
    DoubleVectorType x(n,1);

    // Change tol. to 0.01 is a quick hack to avoid numerical instability...
    bool solv = vnl_solve_qp_with_non_neg_constraints(Q,g,A,b,x,0.01);
    if (solv)
      {
      for (unsigned int i = 0 ; i < n ; i++)
        {
        m_OutputCorrectionModel[list[i]] = static_cast<double>( x[i] );
        }
      }
    else
      {
      itkWarningMacro( "vnl_solve_qp_with_non_neg_constraints failed for component #" << component );
      }

    }

}

}

#endif
