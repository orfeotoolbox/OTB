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

#ifndef otbSimpleRcsPanSharpeningFusionImageFilter_h
#define otbSimpleRcsPanSharpeningFusionImageFilter_h

#include "otbConvolutionImageFilter.h"
#include "otbImage.h"
#include "otbFunctorImageFilter.h"

#include "itkProgressAccumulator.h"


namespace otb
{
/**
 * \class SimpleRcsPanSharpeningFusionImageFilter
 * \brief This class performs a simple Pan sharpening operation
 *
 * Given a Pan image and the corresponding Xs image (oversampled to have the
 * same number of pixels), this filter realizes a simple Pan sharpening
 * operation:
 *
 * \f[ \frac{XS}{\mathrm{Filtered}(PAN)} PAN  \f]
 *
 * This filter supports streaming and multithreading.
 *
 * \ingroup OTBPanSharpening
 *
 * \example Fusion/PanSharpeningExample.cxx
 *
 */
template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision = float>
class ITK_EXPORT SimpleRcsPanSharpeningFusionImageFilter : public itk::ImageToImageFilter<TXsImageType, TOutputImageType>
{
public:
  /** Standard class typedefs */
  typedef SimpleRcsPanSharpeningFusionImageFilter Self;
  typedef itk::ImageToImageFilter<TXsImageType, TOutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Internal image type used as the smoothing filter output */
  typedef otb::Image<TInternalPrecision, TPanImageType::ImageDimension> InternalImageType;

  /** Typedef for the radius of the smoothing filter */
  typedef typename itk::Array<TInternalPrecision> ArrayType;

  /** Method for creation through object factory */
  itkNewMacro(Self);

  /** Run-time type information */
  itkTypeMacro(SimpleRcsPanSharpeningFusionImageFilter, itk::ImageToImageFilter);

  /** Define the radius type for the smoothing operation */
  typedef typename InternalImageType::SizeType RadiusType;

  /** Set the smoothing filter radius  */
  itkGetMacro(Radius, RadiusType);
  itkSetMacro(Radius, RadiusType);

  /** Set the kernel used for the smoothing filter */
  itkSetMacro(Filter, ArrayType);
  itkGetConstReferenceMacro(Filter, ArrayType);

  virtual void SetPanInput(const TPanImageType* image);
  const TPanImageType* GetPanInput(void) const;

  virtual void SetXsInput(const TXsImageType* path);
  const TXsImageType* GetXsInput(void) const;

protected:
  /** Constructor */
  SimpleRcsPanSharpeningFusionImageFilter();

  /** Destructor */
  ~SimpleRcsPanSharpeningFusionImageFilter() override{};

  /** Call to generate data, wiring composite internal minipipeline */
  void GenerateData() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SimpleRcsPanSharpeningFusionImageFilter(Self&); // intentionally not implemented
  void operator=(const Self&);                    // intentionally not implemented

  /** \class FusionFunctor
   * This functor applies the
   *  \f[ \frac{XS}{\mathrm{Filtered}(PAN)}PAN  \f]
   * operation. It is intended for internal use only.
   *
   * \ingroup OTBPanSharpening
   */
  class FusionFunctor
  {
  public:
    // Implement the fusion as a three arguments operator
    void operator()(typename TOutputImageType::PixelType& output, const typename TXsImageType::PixelType& xsPixel, const TInternalPrecision& smoothPanchroPixel,
                    const typename TPanImageType::PixelType& sharpPanchroPixel) const
    {
      TInternalPrecision scale = 1.;

      if (std::abs(smoothPanchroPixel) > 1e-10)
      {
        scale = sharpPanchroPixel / smoothPanchroPixel;
      }

      // Perform fusion for each band with appropriate casting
      for (unsigned int i = 0; i < xsPixel.Size(); ++i)
      {
        output[i] = static_cast<typename TOutputImageType::InternalPixelType>(xsPixel[i] * scale);
      }
    }

    constexpr size_t OutputSize(const std::array<size_t, 3>& inputsNbBands) const
    {
      return inputsNbBands[0];
    }
  };


  /** \class NoDataFusionFunctor
   * This functor applies the following operation if there is no data :
   *  \f[ \frac{XS}{\mathrm{Filtered}(PAN)}PAN  \f]
   * It is intended for internal use only.
   *
   * \ingroup OTBPanSharpening
   */
  class NoDataFusionFunctor
  {
  public:
    // Implement the fusion as a three arguments operator
    void operator()(typename TOutputImageType::PixelType& output, const typename TXsImageType::PixelType& xsPixel, const TInternalPrecision& smoothPanchroPixel,
                    const typename TPanImageType::PixelType& sharpPanchroPixel) const
    {
      // Check for no data Pan value
      if (m_NoDataValuePanAvailable && sharpPanchroPixel == m_NoDataValuePan)
      {
        for (unsigned int i = 0; i < xsPixel.Size(); ++i)
        {
          output[i] = static_cast<typename TOutputImageType::InternalPixelType>(m_NoDataValuesXs[i]);
        }
        return;
      }

      TInternalPrecision scale = 1.;

      if (std::abs(smoothPanchroPixel) > 1e-10)
      {
        scale = sharpPanchroPixel / smoothPanchroPixel;
      }

      // Perform fusion for each band with appropriate casting
      for (unsigned int i = 0; i < xsPixel.Size(); ++i)
      {
        output[i] = (m_NoDataValuesXsAvailable[i] && (xsPixel[i] == m_NoDataValuesXs[i]))
                        ? static_cast<typename TOutputImageType::InternalPixelType>(xsPixel[i])
                        : static_cast<typename TOutputImageType::InternalPixelType>(xsPixel[i] * scale);
      }
    }

    void SetNoDataValuePanAvailable(bool noDataAvailable)
    {
      m_NoDataValuePanAvailable = noDataAvailable;
    }

    void SetNoDataValuePan(typename TPanImageType::PixelType noDataValue)
    {
      m_NoDataValuePan = noDataValue;
    }

    void SetNoDataValuesXsAvailable(std::vector<bool> noDataValuesAvailable)
    {
      m_NoDataValuesXsAvailable = noDataValuesAvailable;
    }

    void SetNoDataValuesXs(std::vector<typename TXsImageType::InternalPixelType> noDataValues)
    {
      m_NoDataValuesXs = noDataValues;
    }

    constexpr size_t OutputSize(const std::array<size_t, 3> inputsNbBands) const
    {
      return inputsNbBands[0];
    }

    NoDataFusionFunctor() : m_NoDataValuePanAvailable(false), m_NoDataValuePan(0), m_NoDataValuesXsAvailable(false), m_NoDataValuesXs()
    {
    }

  private:
    /** No data flags and values for PAN image */
    bool                                      m_NoDataValuePanAvailable;
    typename TPanImageType::InternalPixelType m_NoDataValuePan;

    /** No data flags and values for XS image */
    std::vector<bool>                                     m_NoDataValuesXsAvailable;
    std::vector<typename TXsImageType::InternalPixelType> m_NoDataValuesXs;
  };


  /**
   *  Typedef of the FunctorImageFilter applying the fusion functor to
   *  p, p_smooth and xs.
   */
  typedef FunctorImageFilter<FusionFunctor> FusionFilterType;

  /**
   *  Typedef of the FunctorImageFilter applying the no data fusion functor to
   *  p, p_smooth and xs.
   */
  typedef FunctorImageFilter<NoDataFusionFunctor> NoDataFusionFilterType;

  /** Typedef of the convolution filter performing smoothing */
  typedef otb::ConvolutionImageFilter<TPanImageType, InternalImageType, itk::ZeroFluxNeumannBoundaryCondition<TPanImageType>, TInternalPrecision>
      ConvolutionFilterType;

  /** Pointer to the internal convolution filter */
  typename ConvolutionFilterType::Pointer m_ConvolutionFilter;

  /** Pointer to the fusion filter */
  typename FusionFilterType::Pointer m_FusionFilter;

  /** Pointer to the fusion filter */
  typename NoDataFusionFilterType::Pointer m_NoDataFusionFilter;

  /** Boolean used for no data */
  bool m_UseNoData;

  /** Radius used for the smoothing filter */
  RadiusType m_Radius;

  /** Kernel used for the smoothing filter */
  ArrayType m_Filter;

  /** The internal progress accumulator */
  typename itk::ProgressAccumulator::Pointer m_ProgressAccumulator;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimpleRcsPanSharpeningFusionImageFilter.hxx"
#endif

#endif
