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
#ifndef __otbStreamingShrinkImageFilter_h
#define __otbStreamingShrinkImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkDataObject.h"

namespace otb
{
/** \class StreamingShrinkImageFilter
 *  \brief This class performs a streaming isotropic shrinking operation without smoothing.
 *
 * It is intended to be used where a fast quicklook generation is needed for huge images
 * (for instance for visualization applications).
 *
 * It computes the size of the output according to the size of the input image, a read only
 * the strip of the input image needed to build a line of the output image. In this strip,
 * the pixel are directly selected and passed to the output image.
 *
 * For example, with a 6000X6000 image and a 10 shrinkFactor, it will read 600 lines of 5990 pixels
 * instead of the whole image.
 */
template <class TInputImage,class TOutputImage>
class ITK_EXPORT StreamingShrinkImageFilter
      : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard typedefs */
  typedef StreamingShrinkImageFilter               Self;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage>   Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(StreamingShrinkImageFilter,ImageToImageFilter);

  /** Template parameter typedefs */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;

  /** Shrink factor accessor */
  itkSetMacro(ShrinkFactor,unsigned int);
  itkGetMacro(ShrinkFactor, unsigned int);

  /**
   * StreamingShrinkImageFilter produces an ouptut whose size is different from its input.
   * As such, it must override the GenerateOutputInformation method in order to compute
   * the output size from the input size.
   */
  virtual void GenerateOutputInformation(void);

  virtual void GenerateInputRequestedRegion(void);
  /** Main computation method */
  virtual void UpdateOutputData(itk::DataObject *itkNotUsed(output));

protected:
  /** Constructor */
  StreamingShrinkImageFilter();
  /** Destructor */
  ~StreamingShrinkImageFilter();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StreamingShrinkImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The shrink factor */
  unsigned int m_ShrinkFactor;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingShrinkImageFilter.txx"
#endif

#endif
