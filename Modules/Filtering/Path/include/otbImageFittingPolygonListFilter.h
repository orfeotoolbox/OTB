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
#ifndef __otbImageFittingPolygonListFilter_h
#define __otbImageFittingPolygonListFilter_h

#include "otbPathListToPathListFilter.h"
#include "otbMacro.h"
#include "itkLineConstIterator.h"

namespace otb
{
/** \class ImageFittingPolygonListFilter
 *  \brief Slightly deform polygon to reach higher enery from the image
 *
 * <b>Limitations:</b> This filter is currently working with integer position
 * for the polygon vertices. It should be optimized for continuous positions.
 *
 */
template <class TPath, class TImage>
class ITK_EXPORT ImageFittingPolygonListFilter
  : public PathListToPathListFilter<TPath>
{
public:
  /** Standard typedefs */
  typedef ImageFittingPolygonListFilter   Self;
  typedef PathListToPathListFilter<TPath> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageFittingPolygonListFilter, PathListToPathListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::PathType          PathType;
  typedef typename Superclass::PathListType      PathListType;
  typedef typename Superclass::PathPointerType   PathPointerType;
  typedef typename PathListType::Pointer         PathListPointerType;
  typedef typename PathListType::ConstIterator   IteratorType;
  typedef typename PathType::VertexType          VertexType;
  typedef typename PathType::VertexListType      VertexListType;
  typedef typename VertexListType::ConstIterator VertexListConstIteratorType;
  typedef double                                 RealType;

  typedef TImage                           ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;

  typedef itk::LineConstIterator<ImageType> LineConstIteratorType;

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

  /** Set/Get the search radius. */
  itkSetMacro(Radius, unsigned int);
  itkGetMacro(Radius, unsigned int);

  /** Set/Get the number of iteration. */
  itkSetMacro(NumberOfIterations, unsigned int);
  itkGetMacro(NumberOfIterations, unsigned int);

protected:
  /** Constructor */
  ImageFittingPolygonListFilter();
  /** Destructor */
  virtual ~ImageFittingPolygonListFilter() {}
  /** GenerateData method */
  virtual void GenerateData();
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual double computeValue(ImageConstPointerType image,
                              VertexType middlePoint,
                              VertexType previousPoint,
                              VertexType nextPoint) const;

  unsigned int m_Radius;
  unsigned int m_NumberOfIterations;

private:
  ImageFittingPolygonListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFittingPolygonListFilter.txx"
#endif

#endif
