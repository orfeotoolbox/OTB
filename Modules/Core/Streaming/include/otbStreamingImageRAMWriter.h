/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStreamingImageRAMWriter_h
#define otbStreamingImageRAMWriter_h

#include "itkMacro.h"
#include "itkImageToImageFilter.h"
#include "itkImageBase.h"

namespace otb
{

template <class TInputImage >
class ITK_EXPORT StreamingImageRAMWriter 
: public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:

  typedef StreamingImageRAMWriter Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename TInputImage::RegionType OutputImageRegionType;

  itkNewMacro(Self);

  itkTypeMacro(StreamingImageRAMWriter, itk::ImageToImageFilter);

protected:

  StreamingImageRAMWriter(){};

  ~StreamingImageRAMWriter() ITK_OVERRIDE {};

  void AllocateOutputs();

  void GenerateOutputInformation();

  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            itk::ThreadIdType threadId);

private:

  StreamingImageRAMWriter(const Self &); //purposely not implemented
  void operator = (const Self&); //purposely not implemented



};




} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageRAMWriter.txx"
#endif

#endif