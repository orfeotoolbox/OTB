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
#ifndef __otbPathListToPathListFilter_h
#define __otbPathListToPathListFilter_h

#include "otbObjectListToObjectListFilter.h"
#include "otbObjectList.h"

namespace otb
{
/** \class PathListToPathListFilter
 *  \brief Base class for filter taking a PathList as input a returning a PathList.
 *
 * \ingroup OTBPath
 */
template <class TPath>
class ITK_EXPORT PathListToPathListFilter
  : public ObjectListToObjectListFilter<ObjectList<TPath>, ObjectList<TPath> >
{
public:
  /** Standard typedefs */
  typedef PathListToPathListFilter                                            Self;
  typedef ObjectListToObjectListFilter<ObjectList<TPath>, ObjectList<TPath> > Superclass;
  typedef itk::SmartPointer<Self>                                             Pointer;
  typedef itk::SmartPointer<const Self>                                       ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PathListToPathListFilter, ObjectListToObjectListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::InputObjectType   PathType;
  typedef typename Superclass::InputListType     PathListType;
  typedef typename PathType::Pointer             PathPointerType;
  typedef typename PathListType::Pointer         PathListPointerType;
  typedef typename PathListType::ConstPointer    PathListConstPointerType;
  typedef typename Superclass::OutputObjectType  OutputPathType;
  typedef typename PathType::Pointer             OutputPathPointerType;
  typedef typename Superclass::OutputListType    OutputPathListType;
  typedef typename Superclass::OutputListPointer OutputPathListPointerType;

protected:
  /** Constructor */
  PathListToPathListFilter() {};
  /** Destructor */
  virtual ~PathListToPathListFilter() {}

private:
  PathListToPathListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb

#endif
