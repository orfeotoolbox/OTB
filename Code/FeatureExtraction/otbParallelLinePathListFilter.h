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
#ifndef __otbParallelLinePathListFilter_h
#define __otbParallelLinePathListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"

namespace otb
{
/** \class ParallelLinePathListFilter
 *  \brief Detects parallel lines in imagery, the required input data are a PathList object.
 *
 * The class consists of three basic functions that determine the angle between two lines,
 * the distance between the lines and the common part of the lines. First, all input lines
 * are checked if there is a second line running in the same direction. Thereafter, all line
 * pairs that already fulfilled the angular criterion are checked whether they are close to
 * each other or not, i.e. the orthogonal distance between them is calculated. Finally, it
 * has to be verified if the two lines have a common part since lines may fulfill the two
 * first criteria but be located in different parts of the image. In order to adapt the
 * detection algorithm to the user's needs, the thresholds AngularThreshold, DistanceThreshold
 * and CommonDistanceThreshold can be set.
 *
 * A possible processing chain would be to extract lines with a line detector, to convert the
 * result to PathList object (an ObjectList of PolyLineParametricPath), to link short line
 * segments with the otbLinkPathListFilter to longer lines and to finally detect all parallel
 * long lines.
 *
   * \sa LinkPathListFilter
 *
   * \ingroup PathFilters
 *
 */
template <class TPath>
class ITK_EXPORT ParallelLinePathListFilter
  : public PathListToPathListFilter<TPath>
{
public:
  /** Standard typedefs */
  typedef ParallelLinePathListFilter      Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ParallelLinePathListFilter, PathListToPathListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathListType      PathListType;
  typedef typename Superclass::PathPointerType   PathPointerType;
  typedef typename PathListType::Pointer         PathListPointerType;
  typedef typename PathListType::ConstIterator   IteratorType;
  typedef typename PathType::VertexType          VertexType;
  typedef typename PathType::VertexListType      VertexListType;
  typedef typename VertexListType::ConstIterator VertexIteratorType;
  typedef double                                 RealType;

  itkSetMacro(AngularThreshold, RealType);
  itkGetMacro(AngularThreshold, RealType);
  itkSetMacro(DistanceThreshold, RealType);
  itkGetMacro(DistanceThreshold, RealType);
  itkSetMacro(CommonDistanceThreshold, RealType);
  itkGetMacro(CommonDistanceThreshold, RealType);

protected:
  /** Constructor */
  ParallelLinePathListFilter();
  /** Destructor */
  virtual ~ParallelLinePathListFilter() {}
  /** GenerateData method */
  virtual void GenerateData();
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /**
  * Verify the angular condition to find parallel lines.
  * This function verifies if two lines are parallel by
  * computing the angle in relation to the y-axis.
  * First line segment: v1 is the first vertex, v2 the second one
  * (not necessarily the path ending).
  * Second line segment: v3 is the first vertex, v4 the second one
  * (not necessarily the path ending).
  * Return true if the condition is verified.
  *
  * This is the first criteria to be fullfilled.
  **/
  bool VerifyAngularCondition(VertexType v1, VertexType v2, VertexType v3, VertexType v4);

  /**
  * Verify the maximum distance condition to find parallel lines.
  * The orthogonal distance between two parallel lines is calculated.
  * First line segment: v1 is the first vertex, v2 the second one
  * (not necessarily the path ending).
  * Second line segment: v3 is the first vertex, v4 the second one
  * (not necessarily the path ending).
  * Return true if the condition is verified.
  *
  * This is the second criteria to be fullfilled.
  **/
  bool VerifyMaxDistanceCondition(VertexType v1, VertexType v2, VertexType v3, VertexType v4);

  /**
  * Verify the common distance condition to find parallel lines.
  * The overlapping part of the parallel lines is computed. In case
  * no overlapping part exists or the value is below the specified
  * threshold, false is returned.
  * First line segment: v1 is the first vertex, v2 the second one
  * (not necessarily the path ending).
  * Second line segment: v3 is the first vertex, v4 the second one
  * (not necessarily the path ending).
  * Return true if the condition is verified.
  *
  * This is the third criteria to be fullfilled.
  **/
  bool VerifyCommonDistanceCondition(VertexType v1, VertexType v2, VertexType v3, VertexType v4);

  /**
   * Write the first parallel path.
   * p1: First parallel path (line segment),
   */
  PathPointerType WriteParallelPath(PathPointerType p1);

private:
  ParallelLinePathListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  RealType m_AngularThreshold;
  RealType m_DistanceThreshold;
  RealType m_CommonDistanceThreshold;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbParallelLinePathListFilter.txx"
#endif

#endif
