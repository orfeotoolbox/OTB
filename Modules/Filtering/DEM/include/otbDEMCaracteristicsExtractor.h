/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDEMCaracteristicsExtractor_h
#define otbDEMCaracteristicsExtractor_h

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkAtan2ImageFilter.h"
#include "itkAtanImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkAcosImageFilter.h"
#include "itkCosImageFilter.h"
#include "itkSinImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkAddImageFilter.h"

#include "itkGradientImageFilter.h"

namespace otb
{

/** \class DEMCaracteristicsExtractor
   *  \brief Calculates the slope, the orientation incidence and exitance radius values for each pixel.
   *
   * To note that azimuth value are projection over North-South direction and angles have to be given in degres.
   *
   * \ingroup DEMHanler
   * \ingroup DEMToImageGenerator
   * \ingroup Radiometry
 *
 * \ingroup OTBDEM
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT DEMCaracteristicsExtractor : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** "typedef" for standard classes. */
  typedef DEMCaracteristicsExtractor Self;
  typedef itk::ImageToImageFilter<TInputImage, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(DEMCaracteristicsExtractor, ImageToImageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::InternalPixelType  InputInternalPixelType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef itk::CovariantVector<InputInternalPixelType, 2> VectorPixelType;
  typedef Image<VectorPixelType, 2>                       VectorGradientImageType;

  /** Filter definition */
  typedef itk::GradientMagnitudeImageFilter<InputImageType, OutputImageType> GradientMagnitudeFilterType;
  typedef itk::GradientImageFilter<InputImageType, InputInternalPixelType, InputInternalPixelType> GradientRecursiveGaussianImageFilterType;
  typedef typename GradientRecursiveGaussianImageFilterType::OutputImageType tutuType;
  typedef itk::NthElementImageAdaptor<tutuType, InputInternalPixelType> AdaptorType;
  typedef itk::Atan2ImageFilter<AdaptorType, AdaptorType, OutputImageType> Atan2FilterType;
  typedef itk::AtanImageFilter<OutputImageType, OutputImageType> AtanFilterType;

  /** Operation Filters*/
  typedef MultiplyByScalarImageFilter<OutputImageType, OutputImageType> MultiplyByScalarImageFilterType;
  typedef itk::MultiplyImageFilter<OutputImageType, OutputImageType, OutputImageType> MultiplyImageFilterType;
  typedef itk::AcosImageFilter<OutputImageType, OutputImageType>       AcosImageFilterType;
  typedef itk::CosImageFilter<OutputImageType, OutputImageType>        CosImageFilterType;
  typedef itk::SinImageFilter<OutputImageType, OutputImageType>        SinImageFilterType;
  typedef itk::ShiftScaleImageFilter<OutputImageType, OutputImageType> ShiftScaleImageFilterType;
  typedef itk::AddImageFilter<OutputImageType, OutputImageType, OutputImageType> AddImageFilterType;

  /** Get the slop output image */
  OutputImageType* GetSlopOutput()
  {
    return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
  }

  /** Get the aspect output image */
  OutputImageType* GetAspectOutput()
  {
    return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(1));
  }
  /** Get the incidence output image */
  OutputImageType* GetIncidenceOutput()
  {
    return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(2));
  }

  /** Get the exitance output image */
  OutputImageType* GetExitanceOutput()
  {
    return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(3));
  }

  /** Angle parameters*/
  /** Set/Get Solar Angle */
  itkSetMacro(SolarAngle, double);
  itkGetConstMacro(SolarAngle, double);
  /** Set/Get Solar Solar */
  itkSetMacro(SolarAzimut, double);
  itkGetConstMacro(SolarAzimut, double);
  /** Set/Get View Angle */
  itkSetMacro(ViewAngle, double);
  itkGetConstMacro(ViewAngle, double);
  /** Set/Get View Solar */
  itkSetMacro(ViewAzimut, double);
  itkGetConstMacro(ViewAzimut, double);

protected:
  DEMCaracteristicsExtractor();
  ~DEMCaracteristicsExtractor() override;
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData() override;

private:
  /** Angle parameters*/
  /** Solar Angle */
  double m_SolarAngle;
  /** Solar Solar */
  double m_SolarAzimut;
  /** View Angle */
  double m_ViewAngle;
  /** View Solar */
  double m_ViewAzimut;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDEMCaracteristicsExtractor.hxx"
#endif

#endif
