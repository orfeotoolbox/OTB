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
#ifndef __otbStreamingLineSegmentDetector_h
#define __otbStreamingLineSegmentDetector_h

#include <vector>

#include "itkImageRegion.h"
#include "itkPreOrderTreeIterator.h"

#include "itkDataObjectDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbLineSegmentDetector.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbPersistentImageToVectorDataFilter.h"

namespace otb
{
/** \class PersistentStreamingLineSegmentDetector
 *  \brief Perform Line segment detector in a persistent way.
 *
 *  This filter is a generic PersistentImageFilter, which encapsulate
 *  the Line Segment detector filter.
 *
 * \sa PersistentImageToVectorDataFilter
 *
 */
template <class TImageType>
class PersistentStreamingLineSegmentDetector
  : public otb::PersistentImageToVectorDataFilter<TImageType,
     typename otb::LineSegmentDetector<TImageType, double>::VectorDataType >
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingLineSegmentDetector                              Self;
  typedef PersistentImageToVectorDataFilter<TImageType,
      typename otb::LineSegmentDetector<TImageType, double>::VectorDataType > Superclass;
  typedef itk::SmartPointer<Self>                                             Pointer;
  typedef itk::SmartPointer<const Self>                                       ConstPointer;

  typedef otb::LineSegmentDetector<TImageType, double>     LSDType;
  typedef typename Superclass::InputImageType              InputImageType;
  typedef typename Superclass::InputImagePointer           InputImagePointerType;

  typedef typename Superclass::OutputVectorDataType        OutputVectorDataType;
  typedef typename Superclass::OutputVectorDataPointerType OutputVectorDataPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingLineSegmentDetector, PersistentImageToVectorDataFilter);

protected:
  PersistentStreamingLineSegmentDetector();

  virtual ~PersistentStreamingLineSegmentDetector();

  void GenerateInputRequestedRegion();

private:
  PersistentStreamingLineSegmentDetector(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OutputVectorDataPointerType ProcessTile();
};

template <class TImageType>
class StreamingLineSegmentDetector
{
public:

  // define the PersistentStreamingLineSegmentDetector template
  typedef PersistentStreamingLineSegmentDetector<TImageType>
    PersistentStreamingLineSegmentDetectorType;

  typedef typename PersistentStreamingLineSegmentDetectorType::InputImageType
      InputImageType;
  typedef typename PersistentStreamingLineSegmentDetectorType::OutputVectorDataType
      OutputVectorDataType;

  // typedef for streaming capable filter
  typedef PersistentFilterStreamingDecorator<PersistentStreamingLineSegmentDetectorType>
    FilterType;

};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingLineSegmentDetector.txx"
#endif

#endif
