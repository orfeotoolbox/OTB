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
#ifndef _otbLinkPathListFilter_h
#define _otbLinkPathListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"

namespace otb
{
/** \class LinkPathListFilter
 *  \brief CBase class for breaking angulars the input PathList, returning a PathList.
 */
template <class TPath>
class ITK_EXPORT LinkPathListFilter
  : public PathListToPathListFilter<TPath>
{
 public:
  /** Standard typedefs */
  typedef LinkPathListFilter            Self;
  typedef PathListToPathListFilter<TPath>       Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(LinkPathListFilter, PathListToPathListFilter);
  
  /** Template parameters typedefs */
  typedef typename Superclass::PathType         PathType;
  typedef typename Superclass::PathListType     PathListType;
  typedef typename Superclass::PathPointerType  PathPointerType;
  typedef typename PathListType::Pointer        PathListPointerType;
  
protected:
  /** Constructor */
  LinkPathListFilter();
  /** Destructor */
  virtual ~LinkPathListFilter() {};
  /** GenerateData method */
  virtual void GenerateData();
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LinkPathListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLinkPathListFilter.txx"
#endif

#endif
