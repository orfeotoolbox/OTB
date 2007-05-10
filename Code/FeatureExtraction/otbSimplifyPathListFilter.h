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
#ifndef _otbSimplifyPathListFilter_h
#define _otbSimplifyPathListFilter_h

#include "otbPathListToPathListFilter.h"

namespace otb
{
/** \class SimplifyPathListFilter
 *  \brief This filter performs a simplification of the path in the input list.
 * 
 *  It reduces the number of vertices in each path, according to a tolerance criterion. It aims at 
 *  removing aligned vertices while keeping sharp angular points.
 *
 *  In order to ensure the unicity of its output, each path is considered first from begin to end, then 
 *  from begin to the first vertex before the end. At each step, the consistency of the path is checked : 
 *  the equation of the line passing by the first and last vertices is computed. Then, for each
 *  vertices between them, the euclidean distance to this line is computed. If for one vertex, this distance
 *  is upper than the tolerance threshold, the path is considered to be inconsistent and no vertices can be removed.
 *
 *  If the past is considered constistent (which will occure at least with a 2 vertices path), only the beginning and ending
 *  vertices are kept and a new search iteration begin at its end.
 *
 *  This filter is part of the road extraction framework.
 *
 * \sa BreakAngularPathListFilter
 * \sa RemoveTortuousPathFilter.
 */
template <class TPath>
class ITK_EXPORT SimplifyPathListFilter
  : public PathListToPathListFilter<TPath>
{
 public:
  /** Standard typedefs */
  typedef SimplifyPathListFilter                         Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(SimplifyPathListFilter, PathListToPathList);
  
  /** Derived typedefs */
  typedef typename Superclass::OutputPathType PathType;
  typedef typename Superclass::OutputPathPointerType PathPointerType;
  typedef typename Superclass::OutputPathListType PathListType;
  typedef typename Superclass::OutputPathListPointerType PathListPointerType;
  typedef typename Superclass::OutputPathListConstPointerType PathListConstPointerType;

  typedef typename PathType::VertexListType VertexListType;
  typedef typename VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename VertexListType::Iterator VertexListIteratorType;
  typedef typename VertexListType::ConstIterator VertexListConstIteratorType;
  typedef typename PathType::VertexType VertexType;
  
  typedef double ToleranceType;  

  itkGetMacro(Tolerance,ToleranceType);
  itkSetMacro(Tolerance,ToleranceType);

protected:
  /** Constructor */
  SimplifyPathListFilter();
  /** Destructor */
  virtual ~SimplifyPathListFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData(void);
  /**
   * Test if the segment composed of the points in vertextlist between the begin and the end iterators is valid.
   * \param vertexList The list of vertices composing the segment,
   * \param begin The Beginning of the segment,
   * \param end The end of the segment, 
   * \return True if the segment is valid.
   */
  virtual bool TestPathConsistency(VertexListConstPointerType vertexList, VertexListConstIteratorType begin, VertexListConstIteratorType end);
private:
  SimplifyPathListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //// Tolerance for segment consistency (tolerance in terms of distance)
  ToleranceType m_Tolerance;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimplifyPathListFilter.txx"
#endif

#endif
