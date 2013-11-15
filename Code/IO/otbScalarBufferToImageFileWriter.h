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
#ifndef __otbScalarBufferToImageFileWriter_h
#define __otbScalarBufferToImageFileWriter_h


#include "itkProcessObject.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

namespace otb
{
/** \class ScalarBufferToImageFileWriter
 *
 * \brief Record an image stored in a buffer.
 *
 * TBufferType is the scalar buffer type, TOutputPixelType is the type in which the image will be recorded.
 * User have to precise the image size, the number of channel will be computed automatically.
 * The SetFilename method precises the image name.
 */

template <class TBufferType, class TOutputPixelType=TBufferType>
class ITK_EXPORT ScalarBufferToImageFileWriter : public itk::ProcessObject
{
public:

  typedef ScalarBufferToImageFileWriter    Self;
  typedef itk::ProcessObject    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ScalarBufferToImageFileWriter, itk::ProcessObject);

  /** INput typedefs */
  typedef TBufferType BufferType;
  typedef TOutputPixelType OutputPixelType;

  /** Output image type */
  typedef otb::VectorImage<OutputPixelType, 2> ImageType;
  typedef typename ImageType::PixelType PixelType;
  typedef typename ImageType::RegionType       RegionType;
  typedef typename ImageType::SizeType         SizeType;
  typedef typename ImageType::IndexType         IndexType;

  /** Writer Type*/
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef typename WriterType::Pointer WriterPointer;

  /** Filename accessor */
  otbGetObjectMemberMacro(Writer, FileName, std::string);
  otbSetObjectMemberMacro(Writer, FileName, std::string);

  /** Size accessors */
  itkGetMacro(ImageSize, SizeType);
  itkSetMacro(ImageSize, SizeType);

  /** Number of channels */
  itkGetMacro(NumberOfChannels, unsigned int);
  itkSetMacro(NumberOfChannels, unsigned int);

  /** Inverse X spacing accessors. */
  itkGetMacro(InverseXSpacing, bool);
  itkSetMacro(InverseXSpacing, bool);

  /** Buffer accessors */
  void SetBuffer( BufferType * pBuff )
  {
       m_Buffer = pBuff;
  }

   virtual void GenerateData();

   virtual void Update()
   {
     this->GenerateData();
   }

protected:
  ScalarBufferToImageFileWriter();
  virtual ~ScalarBufferToImageFileWriter() { /* don't call ClearBuffer, user's care */}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

  ScalarBufferToImageFileWriter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented


  /** Writer */
  WriterPointer m_Writer;

  /** Scalar tab buffer to writer */
  BufferType * m_Buffer;

  /**Output image number of channels */
  unsigned int m_NumberOfChannels;

  /** Output image size */
  SizeType m_ImageSize;

  /** Inverse biffer among X */
  bool m_InverseXSpacing;
};

} // end namespace otb

#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbScalarBufferToImageFileWriter.txx"
#endif

#endif
