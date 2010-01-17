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
#ifndef __otbLabelObjectToPolygonFunctor_h
#define __otbLabelObjectToPolygonFunctor_h

#include <vector>

//TODO change this include  have to define from what inherate this class
#include "otbPolyLineParametricPathWithValue.h" //for vcl_abs

namespace otb
{

namespace Functor
{

/** \class LabelObjectToPolygonFunctor
 *  \brief This class vectorizes a LabelObject to a Polygon.
 *
 * The algorithm follows a finite states machine described in the
 * following paper:
 *
 * "An algorithm for the rapid computation of boundaries of run-length
 * encoded regions", Francis K. H. Queck, in Pattern Recognition 33
 * (2000), p 1637-1649.
 *
 * Lines in the RLE are first sorted in lexicographical order (to
 * ensure ordered RLE), and the finite states machine tracks the edge
 * following 8 canonical states given by the configuration of two
 * consecutive raws of lines.
 *
 * Iterations are done until convergence which is guaranteed to
 * happen.
 *
 * Please be aware that this functor is not thread-safe.
 *
 */
template <class TLabelObject, class TPolygon >
class LabelObjectToPolygonFunctor
{
public:
  /** Template parameters typedefs */
  typedef TLabelObject                                LabelObjectType;
  typedef typename LabelObjectType::LineContainerType LineContainerType;
  typedef typename LabelObjectType::LineType          LineType;
  typedef typename LineType::IndexType                IndexType;
  typedef TPolygon                                    PolygonType;
  typedef typename PolygonType::Pointer               PolygonPointerType;
 
  /**
   * \param labelObject the label object to vectorize
   * \return The vectorized label object as a polygon.
   */
  inline PolygonType * operator()(const LabelObjectType * labelObject);
  
private:
  /// Internal structures
  typedef std::vector<LineType>                       RunsPerLineType;
  typedef std::vector<RunsPerLineType>                RunsPerLineVectorType;
  typedef std::vector<IndexType>                      IndexVectorType;

  /// Internal enums
  enum StateType{UP_LEFT,UP_RIGHT,DOWN_LEFT,DOWN_RIGHT};
  enum PositionFlagType{LEFT_END,RIGHT_END};

 /// Compare two line in the lexicographical order
  static bool LexicographicalLineCompare(const LineType &  l1, const LineType & l2);

  /// Check if the given run index (index in line,line) is valid
  inline bool IsRunIndexValid(const IndexType & index) const;

  /// Check if the point lies within the range of the line
  inline IndexType Within(const IndexType & point, unsigned int line);
  
  /// Return the left-end of the run
  inline IndexType LeftEnd(const IndexType & runIndex) const;

  /// Return the right-end of the run
  inline IndexType RightEnd(const IndexType & runIndex) const;
 
  /** Return the right-most run whose left end lies within the range of
   * the given run.
   */
  inline IndexType RightMostLeftEndInside(unsigned int line, const IndexType & point, const IndexType & run) const;

  /** Return the left-most run whose right end lies within the range of
   * the given run.
   */
  inline IndexType LeftMostRightEndInside(unsigned int line, const IndexType & point, const IndexType & run) const;
  
  /// Walk left to update the finite states machine.
  inline void WalkLeft(unsigned int line,const IndexType & startPoint, const IndexType & endPoint, PolygonType * polygon, const StateType state);

  /// Walk right to update the finite states machine.
  inline void WalkRight(unsigned int line,const IndexType & startPoint, const IndexType & endPoint, PolygonType * polygon, const StateType state);
 
  PolygonPointerType m_Polygon;

  // Internal structure to store runs
  RunsPerLineVectorType m_InternalDataSet;

  /// The current state
  StateType             m_CurrentState;
  /// The position flag
  PositionFlagType      m_PositionFlag;
  
  // The starting point for vectorization
  IndexType             m_StartingPoint;
  
  /// The current point for vectorization
  IndexType             m_CurrentPoint;
  
  /// The current run for vectorization
  IndexType             m_CurrentRun;
  
  /// The current line for vectorization
  unsigned int          m_CurrentLine;
  
  /// The vector of vectorized boundaries
  IndexVectorType       m_Solution;

  /// The line offset from start of the region
  unsigned int          m_LineOffset;

}; // end class LabelObjectToPolygonFunctor

} // end namespace Functor

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelObjectToPolygonFunctor.txx"
#endif

#endif


