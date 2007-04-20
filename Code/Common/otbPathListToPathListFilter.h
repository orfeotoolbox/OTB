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
#ifndef _otbPathListToPathListFilter_h
#define _otbPathListToPathListFilter_h

#include "otbPathListSource.h"

namespace otb
{
/** \class PathListToPathListFilter
 *  \brief Base class for filter taking a PathList as input a returning a PathList.
 */
template <class TInputPath,class TOutputPath>
class ITK_EXPORT PathListToPathListFilter
  : public PathListSource<TOutputPath>
{
 public:
  /** Standard typedefs */
  typedef PathListToPathListFilter      Self;
  typedef PathListSource<TOutputPath>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(PathListToPathListFilter, PathListSource);
  
  /** Template parameters typedefs */
  typedef TInputPath InputPathType;
  typedef typename InputPathType::Pointer InputPathPointerType;
  typedef otb::ObjectList<InputPathType> InputPathListType;
  typedef typename InputPathListType::Pointer InputPathListPointerType;
  typedef typename InputPathListType::ConstPointer InputPathListConstPointerType;
  /** 
   * Set the input path list.
   * \param pathList The input path list.
   */
  void SetInput( const InputPathListType * pathList);
  /**
   * Get the input path list.
   * \return the input path list.
   */
  const InputPathListType * GetInput(void);

protected:
  /** Constructor */
  PathListToPathListFilter();
  /** Destructor */
  virtual ~PathListToPathListFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PathListToPathListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathListToPathListFilter.txx"
#endif

#endif
