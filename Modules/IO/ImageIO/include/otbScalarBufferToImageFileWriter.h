/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbScalarBufferToImageFileWriter_h
#define otbScalarBufferToImageFileWriter_h


#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "OTBImageIOExport.h"

namespace otb
{
/** \class ScalarBufferToImageFileWriter
 *
 * \brief Record an image stored in a buffer.
 *
 * TBufferType is the scalar buffer type, TOutputPixelType is the type in which the image will be recorded.
 * User have to precise the image size, the number of channel will be computed automatically.
 * The SetFilename method precises the image name.
 *
 * \ingroup OTBImageIO
 */

template <class TBufferType, class TOutputPixelType = TBufferType>
class OTBImageIO_EXPORT_TEMPLATE ScalarBufferToImageFileWriter : public itk::ProcessObject
{
public:
  typedef ScalarBufferToImageFileWriter Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ScalarBufferToImageFileWriter, itk::ProcessObject);

  /** INput typedefs */
  typedef TBufferType      BufferType;
  typedef TOutputPixelType OutputPixelType;

  /** Output image type */
  typedef otb::VectorImage<OutputPixelType, 2> ImageType;
  typedef typename ImageType::PixelType  PixelType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::SizeType   SizeType;
  typedef typename ImageType::IndexType  IndexType;

  /** Writer Type*/
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef typename WriterType::Pointer    WriterPointer;

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
  void SetBuffer(BufferType* pBuff)
  {
    m_Buffer = pBuff;
  }

  void GenerateData() override;

  void Update() override
  {
    this->GenerateData();
  }

protected:
  ScalarBufferToImageFileWriter();
  ~ScalarBufferToImageFileWriter() override
  { /* don't call ClearBuffer, user's care */
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ScalarBufferToImageFileWriter(const Self&) = delete;
  void operator=(const Self&) = delete;


  /** Writer */
  WriterPointer m_Writer;

  /** Scalar tab buffer to writer */
  BufferType* m_Buffer;

  /**Output image number of channels */
  unsigned int m_NumberOfChannels;

  /** Output image size */
  SizeType m_ImageSize;

  /** Inverse biffer among X */
  bool m_InverseXSpacing;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarBufferToImageFileWriter.hxx"
#endif

#endif
