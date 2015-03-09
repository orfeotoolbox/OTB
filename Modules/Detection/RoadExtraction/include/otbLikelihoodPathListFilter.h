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
#ifndef __otbLikelihoodPathListFilter_h
#define __otbLikelihoodPathListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"

namespace otb
{
/** \class LikelihoodPathListFilter
 *  \brief Affect a value to each path of the list based on the image
 *
 * PathList and image are given as an input, the filter affect a value to each
 * path: this value is equal to the sum of the pixels crossed by the path
 * (using a PolyLineImageConstIterator) divided by the number of points in the
 * path
 *
 *
 * \ingroup OTBRoadExtraction
 */
template <class TPath, class TImage>
class ITK_EXPORT LikelihoodPathListFilter
  : public PathListToPathListFilter<TPath>
{
public:
  /** Standard typedefs */
  typedef LikelihoodPathListFilter        Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LikelihoodPathListFilter, PathListToPathListFilter);

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

  typedef TImage                           ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;

  /**
   * Set the input Likelihood image.
   * \param image The Likelihood image.
   */
  void SetInputImage(const ImageType * image);

  /**
   * Get the input Likelihood image.
   * \return The input Likelihood image.
   */
  const ImageType * GetInputImage(void);

protected:
  /** Constructor */
  LikelihoodPathListFilter();
  /** Destructor */
  virtual ~LikelihoodPathListFilter() {}
  /** GenerateData method */
  virtual void GenerateData();
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LikelihoodPathListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string m_Key;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLikelihoodPathListFilter.txx"
#endif

#endif
