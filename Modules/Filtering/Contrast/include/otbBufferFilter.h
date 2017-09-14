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

#ifndef otbBufferFilter_h
#define otbBufferFilter_h

#include "itkInPlaceImageFilter.h"
#include "otbImage.h"
namespace otb
{

template < class TInputImage >
class ITK_EXPORT BufferFilter :
  public itk::InPlaceImageFilter < TInputImage , TInputImage >
{
public:
  typedef TInputImage InputImageType;
  /** typedef for standard classes. */
  typedef BufferFilter Self;
  typedef itk::InPlaceImageFilter< InputImageType, InputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BufferFilter, InPlaceImageFilter);

protected:
  BufferFilter() {};
  ~BufferFilter() ITK_OVERRIDE {}

  virtual void ThreadedGenerateData(const typename InputImageType::RegionType & itkNotUsed(outputRegionForThread) ,
                            itk::ThreadIdType itkNotUsed(threadId) )
    {
    #ifdef DEBUGGING
    typename InputImageType::ConstPointer input = this->GetInput();
    typename InputImageType::Pointer output = this->GetOutput();
    std::cout<<"########################BufferFilter##########################"<<std::endl;
    std::cout<<"input requested index "<<input->GetRequestedRegion().GetIndex()<<std::endl;
    std::cout<<"input requested size "<<input->GetRequestedRegion().GetSize()<<std::endl;
    std::cout<<"output requested index "<<output->GetRequestedRegion().GetIndex()<<std::endl;
    std::cout<<"output requested size "<<output->GetRequestedRegion().GetSize()<<std::endl;
    std::cout<<"########################BufferFilter##########################"<<std::endl;
    #endif
    };

private:
  BufferFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

}  // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif
  
#endif