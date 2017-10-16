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

#ifndef otbCLHistogramEqualizationFilter_h
#define otbCLHistogramEqualizationFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "otbComputeHistoFilter.h"
#include "otbComputeGainLutFilter.h"
#include "otbApplyGainFilter.h"

namespace otb
{

/** \class CLHistogramEqualizationFilter
 *  \brief 
 * \ingroup OTBContrast
 */

template < class TInputImage , class TOutputImage >
class ITK_EXPORT CLHistogramEqualizationFilter :
  public itk::ImageToImageFilter< TInputImage , TOutputImage >
{
public :
  /** typedef for standard classes. */

  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  typedef CLHistogramEqualizationFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self)
  /** Run-time type information (and related methods). */
  itkTypeMacro(CLHistogramEqualizationFilter, ImageToImageFilter)


protected :
  CLHistogramEqualizationFilter();
  ~CLHistogramEqualizationFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override ;

private :
  CLHistogramEqualizationFilter(const Self &) = delete ;
  void operator =(const Self&) = delete ; 

};

}  // End namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCLHistogramEqualizationFilter.txx"
#endif


#endif
