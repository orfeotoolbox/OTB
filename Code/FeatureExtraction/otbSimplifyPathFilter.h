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
#ifndef _otbSimplifyPathFilter_h
#define _otbSimplifyPathFilter_h

#include "otbPathListToPathListFilter.h"

namespace otb
{
/** \class SimplifyPathFilter
 *  \brief 
 * 
 * \ingroup 
 * \ingroup
 */
template <class TPath>
class ITK_EXPORT SimplifyPathFilter
  : public PathListToPathListFilter<TPath>
{
 public:
  /** Standard typedefs */
  typedef SimplifyPathFilter                         Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(SimplifyPathFilter, PathListToPathList);
  
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

  itkGetMacro(Tolerance,double);
  itkSetMacro(Tolerance,double);

protected:
  /** Constructor */
  SimplifyPathFilter();
  /** Destructor */
  virtual ~SimplifyPathFilter() {};
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
  SimplifyPathFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //// Tolerance for segment consistency (tolerance in terms of distance)
  double m_Tolerance;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimplifyPathFilter.txx"
#endif

#endif
