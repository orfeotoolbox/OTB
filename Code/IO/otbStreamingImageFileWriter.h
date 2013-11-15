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
#ifndef __otbStreamingImageFileWriter_h
#define __otbStreamingImageFileWriter_h

#include "vcl_deprecated_header.h"

#include "otbImageFileWriter.h"

namespace otb
{
/** \class StreamingImageFileWriter
 *  \brief Deprecated streaming-enabled image writing class
 *
 *  StreamingImageFileWriter is deprecated and provided for backward
 *  compatibility only. Full support of streaming is now provided by
 *  otb::ImageFileWriter, and StreamingImageFileWriter is a simple
 *  alias of this class.
 *
 *  \sa ImageFileWriter
 */
template <class TInputImage> class ITK_EXPORT StreamingImageFileWriter : public ImageFileWriter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingImageFileWriter       Self;
  typedef ImageFileWriter<TInputImage>   Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileWriter, itk::ImageFileWriter);

 /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      Superclass::InputImageDimension);

protected:
  /** Constructor */
  StreamingImageFileWriter(){}

  /** Destructor */
  virtual ~StreamingImageFileWriter(){}

private:
  StreamingImageFileWriter(const StreamingImageFileWriter &); //purposely not implemented
  void operator =(const StreamingImageFileWriter&); //purposely not implemented
};

} // end namespace otb

#endif
