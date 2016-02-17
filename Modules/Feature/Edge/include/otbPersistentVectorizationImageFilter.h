/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPersistentVectorizationImageFilter_h
#define __otbPersistentVectorizationImageFilter_h

#include "otbPersistentImageFilter.h"

#include "otbObjectList.h"
#include "otbImageToEdgePathFilter.h"
#include "itkMinimumMaximumImageFilter.h"

namespace otb
{

/** \class PersistentVectorizationImageFilter
 *  \brief Perform vectorization in a persistent way.
 *
 * This filter allows vectorizing input labeled image while it is streamed
 * through it. The GetPathList() method allows getting the vectorized boundaries,
 * while the GetOutput() method allows getting the input image itself.
 *
 * The Reset() method allows clearing the result path list.
 *
 * \sa PersistentImageFilter
 *
 *
 * \ingroup OTBEdge
 */
template<class TInputImage, class TOutputPath>
class ITK_EXPORT PersistentVectorizationImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentVectorizationImageFilter              Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentVectorizationImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  // First Input
  typedef TInputImage                             ImageType;
  typedef typename TInputImage::Pointer           InputImagePointer;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;
  typedef typename TInputImage::InternalPixelType InternalPixelType;

  // Output path type
  typedef TOutputPath                                     PathType;
  typedef otb::ObjectList<PathType>                       PathListType;
  typedef typename PathListType::Pointer                  PathListPointerType;
  typedef typename PathType::Pointer                      PathPointerType;
  typedef itk::MinimumMaximumImageFilter<ImageType>       MinMaxFilterType;
  typedef typename MinMaxFilterType::Pointer              MinMaxFilterPointerType;
  typedef otb::ImageToEdgePathFilter<ImageType, PathType> ImageToEdgePathFilterType;
  typedef typename ImageToEdgePathFilterType::Pointer     ImageToEdgePathFilterPointerType;

  virtual void Reset(void);
  virtual void Synthetize(void);

  itkGetObjectMacro(PathList, PathListType);

protected:
  PersistentVectorizationImageFilter();
  virtual ~PersistentVectorizationImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  virtual void GenerateData();

private:
  PersistentVectorizationImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Min max filter (the mini-pipeline)
  MinMaxFilterPointerType m_MinMaxFilter;

  // Where to store the vectorized objects
  PathListPointerType m_PathList;

}; // end of class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentVectorizationImageFilter.txx"
#endif

#endif
