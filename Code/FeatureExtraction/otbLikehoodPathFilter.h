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
#ifndef _otbLinkPathFilter_h
#define _otbLinkPathFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"

namespace otb
{
/** \class LikehoodPathFilter
 *  \brief 
 */
template <class TPath, class TImage>
class ITK_EXPORT LikehoodPathFilter
  : public PathListToPathListFilter<TPath>
{
 public:
  /** Standard typedefs */
  typedef LikehoodPathFilter                    Self;
  typedef PathListToPathListFilter<TPath>       Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(LikehoodPathFilter, PathListToPathListFilter);
  
  /** Template parameters typedefs */
  typedef typename Superclass::PathType         PathType;
  typedef typename Superclass::PathListType     PathListType;
  typedef typename Superclass::PathPointerType  PathPointerType;
  typedef typename PathListType::Pointer        PathListPointerType;
  typedef typename PathListType::ConstIterator IteratorType;
  typedef typename PathType::VertexType VertexType;
  typedef typename PathType::VertexListType VertexListType;
  typedef typename VertexListType::ConstIterator VertexIteratorType;
  typedef double 		                 RealType;

  typedef TImage ImageType;
  typedef typename ImageType::Pointer ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;
  
  /**
   * Set the input likehood image.
   * \param image The likehood image.
   */
  void SetInputImage(const ImageType * image);
  
  /**
   * Get the input likehood image.
   * \return The input likehood image.
   */
  const ImageType * GetInputImage(void);

protected:
  /** Constructor */
  LikehoodPathFilter();
  /** Destructor */
  virtual ~LikehoodPathFilter() {};
  /** GenerateData method */
  virtual void GenerateData();
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LikehoodPathFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string m_Key;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLikehoodPathFilter.txx"
#endif

#endif
