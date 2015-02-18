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
#ifndef __otbPersistentImageFilter_h
#define __otbPersistentImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class PersistentImageFilter
 *  \brief This filter is the base class for all filter persisting data through multiple
 *   update.
 *   For instance, a filter computing global statistics on an image with streaming
 *   capabilities will have to keep the temporary results for each streamed piece of the
 *   image in order to synthesize the global statistics at the end. This filter is an
 *   itk::ImageToImageFilter, providing two additional methods. The first one, Synthetize(),
 *   allows the user to synthesize temporary data produced by the multiple updates on different
 *   pieces of the image to the global result. The second one, Reset(), allows the user to
 *   reset the temporary data for a new input image for instance.
 *
 *  \note This class contains pure virtual method, and can not be instantiated.
 *
 * \sa StatisticsImageFilter
 * \sa StatisticsVectorImageFilter
 *
 * \ingroup OTBStreaming
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT PersistentImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef PersistentImageFilter                              Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Creation through object factory macro */
  itkTypeMacro(PersistentImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  /**
   * Reset the persistent data of the filter.
   */
  virtual void Reset(void) = 0;
  /**
   * Synthesize the persistent data of the filter.
   */
  virtual void Synthetize(void) = 0;

protected:
  /** Constructor */
  PersistentImageFilter() {}
  /** Destructor */
  virtual ~PersistentImageFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  PersistentImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb

#endif
