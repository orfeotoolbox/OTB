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
#ifndef __otbBreakAngularPathListFilter_h
#define __otbBreakAngularPathListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"

namespace otb
{
/** \class BreakAngularPathListFilter
 *  \brief CBase class for breaking angulars the input PathList, returning a PathList.
   *
   * \example FeatureExtraction/ExtractRoadByStepsExample.cxx
   *
 *
 * \ingroup OTBRoadExtraction
 */
template <class TPath>
class ITK_EXPORT BreakAngularPathListFilter
  : public PathListToPathListFilter<TPath>
{
public:
  /** Standard typedefs */
  typedef BreakAngularPathListFilter      Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(BreakAngularPathListFilter, PathListToPathListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::PathType        PathType;
  typedef typename Superclass::PathListType    PathListType;
  typedef typename Superclass::PathPointerType PathPointerType;
  typedef typename PathListType::Pointer       PathListPointerType;

  typedef double MaxAngleType;

  /** Set/Get the max angle */
  itkSetMacro(MaxAngle, MaxAngleType);
  itkGetConstMacro(MaxAngle, MaxAngleType);

protected:
  /** Constructor */
  BreakAngularPathListFilter();
  /** Destructor */
  ~BreakAngularPathListFilter() ITK_OVERRIDE {}
  /** GenerateData method */
  void GenerateData() ITK_OVERRIDE;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  BreakAngularPathListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Calculate break angular for a path */
  void BreakAngularPath(const MaxAngleType maxAngle,
                        const PathPointerType inputPath,
                        PathListPointerType outputPathList);

  /** Max angle value */
  MaxAngleType m_MaxAngle;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBreakAngularPathListFilter.txx"
#endif

#endif
