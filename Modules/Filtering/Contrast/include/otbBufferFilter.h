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

/** \class BufferFilter
 *  \brief This filter has the only purpose to recall regions
 *
 *  This class is implemented to recall regions. Due to ITK implementation 
 *  if the pipeline of the algorithm has branch (diamond) one might have 
 *  an input with two requested regions : one from branch 1 (A) and one from 
 *  branch 2 (B). Problem is after updating and generating data on branch 1 
 *  branch 2 will not propagate its region B again,and will use region A 
 *  instead. By memorizing the region this buffer filter can be placed in 
 *  in front of each branch so that the requested region will be saved.v
 *
 * \ingroup OTBContrast
 */

template < class TInputImage >
class ITK_EXPORT BufferFilter :
  public itk::InPlaceImageFilter < TInputImage , TInputImage >
{
public:
  /** typedef for standard classes. */
  typedef TInputImage InputImageType;

  typedef BufferFilter Self;
  typedef itk::InPlaceImageFilter< InputImageType, InputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self)

  /** Run-time type information (and related methods). */
  itkTypeMacro(BufferFilter, InPlaceImageFilter)

protected:
  BufferFilter() {}
  ~BufferFilter() override {}

  virtual void ThreadedGenerateData(
      const typename InputImageType::RegionType & 
        itkNotUsed(outputRegionForThread) ,
      itk::ThreadIdType itkNotUsed(threadId) ) override{}

private:
  BufferFilter(const Self &) = delete ;
  void operator =(const Self&) = delete ;

};

}  // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif
  
#endif
