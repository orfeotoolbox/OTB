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
#ifndef _otbRemoveTortuousPathFilter_h
#define _otbRemoveTortuousPathFilter_h

#include "otbPathListToPathListFilter.h"

namespace otb
{
/** \class RemoveTortuousPathFilter
 *  \brief This filter remove path considered as tortuous.
 *
 *  A path is considered to be tortuous if the mean distance between each consecutive vertices
 *  is lower than the user provided threshold.
 *
 * \sa BreakAngularPathListFilter
 * \sa SimplifyPathFilter
 */
template <class TPath>
class ITK_EXPORT RemoveTortuousPathFilter
  : public PathListToPathListFilter<TPath>
{
 public:
  /** Standard typedefs */
  typedef RemoveTortuousPathFilter                         Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(RemoveTortuousPathFilter, PathListToPathList);
  
  /** Derived typedefs */
  typedef typename Superclass::OutputPathType PathType;
  typedef typename Superclass::OutputPathPointerType PathPointerType;
  typedef typename Superclass::OutputPathListType PathListType;
  typedef typename Superclass::OutputPathListPointerType PathListPointerType;
  typedef typename Superclass::OutputPathListConstPointerType PathListConstPointerType;
  typedef typename PathType::ConstPointer PathConstPointerType;

  typedef typename PathType::VertexListType VertexListType;
  typedef typename VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename VertexListType::Iterator VertexListIteratorType;
  typedef typename VertexListType::ConstIterator VertexListConstIteratorType;
  typedef typename PathType::VertexType VertexType;

  typedef double MeanDistanceThresholdType;
  
  itkGetMacro(MeanDistanceThreshold,MeanDistanceThresholdType);
  itkSetMacro(MeanDistanceThreshold,MeanDistanceThresholdType);

protected:
  /** Constructor */
  RemoveTortuousPathFilter();
  /** Destructor */
  virtual ~RemoveTortuousPathFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData(void);
  /**
   * Compute the mean distance between two consecutive vertices of the path. 
   * \param path The path to process,
   * \return The mean distance.
   */
  virtual double ComputeMeanDistanceBetweenVertices(PathPointerType path);

 private:
  RemoveTortuousPathFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //// Tolerance for segment consistency (tolerance in terms of distance)
  MeanDistanceThresholdType m_MeanDistanceThreshold;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRemoveTortuousPathFilter.txx"
#endif

#endif
