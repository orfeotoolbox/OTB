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
#ifndef __otbDrawLineSpatialObjectListFilter_h
#define __otbDrawLineSpatialObjectListFilter_h

#include "itkImageToImageFilter.h"
#include "otbLineSpatialObjectList.h"

namespace otb
{

/** \class DrawLineSpatialObjectListFilter
 * \brief Composite filter which draw lines in an image.
 *
 * This class implements a composite filter that draws a list of lines in
 * an input Image. This class
 *
 *
 *
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT DrawLineSpatialObjectListFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedefs support for inputs & outputs*/
  typedef TInputImage                          InputImageType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** typedef for the classes standards. */
  typedef DrawLineSpatialObjectListFilter                    Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  typedef LineSpatialObjectList                  LinesListType;
  typedef LinesListType::LineType                LineType;
  typedef LineType::PointListType                PointListType;
  typedef typename LinesListType::const_iterator LineListIterator;

  typedef itk::ProcessObject ProcessObjectType;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(DrawLineSpatialObjectListFilter, ImageToImageFilter);

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef typename OutputImageType::PixelType      OutputPixelType;
  typedef typename OutputImageType::IndexType      OutputIndexType;
  typedef typename OutputIndexType::IndexValueType OutputIndexValueType;

  /** Set/Get the image input of this process object. */
  virtual void SetInputLineSpatialObjectList(const LinesListType * list);
  LinesListType * GetInputLineSpatialObjectList(void);

  /** Get/Set m_Value*/
  itkGetMacro(Value, OutputPixelType);
  itkSetMacro(Value, OutputPixelType);

protected:
  DrawLineSpatialObjectListFilter();
  virtual ~DrawLineSpatialObjectListFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  /**
   * compute the intersection of the segment to draw with the region
   */
  virtual void CropSegment(OutputIndexType *indexToCrop,
                           OutputIndexType *otherIndex,
                           const OutputImageRegionType *outputRegionForThread) const;

  virtual void CropRightSegment(OutputIndexType *indexToCrop,
                                OutputIndexType *otherIndex,
                                const OutputImageRegionType *outputRegionForThread) const;

  virtual bool IsUpsideTheRegion(OutputIndexType *indexToCrop,
                                 const OutputImageRegionType *outputRegionForThread) const;

  virtual bool IsDownsideTheRegion(OutputIndexType *indexToCrop,
                                   const OutputImageRegionType *outputRegionForThread) const;

  virtual bool IsDownsideTheImage(OutputIndexType *indexToCrop) const;

  virtual bool IsColumnOutsideOfTheRegion(OutputIndexType *indexToCheck,
                                          OutputIndexType *otherToCheck,
                                          const OutputImageRegionType *outputRegionForThread) const;

private:
  DrawLineSpatialObjectListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  OutputPixelType m_Value;
  int             m_Length;
  int             m_Width;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawLineSpatialObjectListFilter.txx"
#endif

#endif
