/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbReflectanceToRadianceImageFilter_h
#define otbReflectanceToRadianceImageFilter_h

#include "otbVarSol.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbMath.h"
#include "otbMacro.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"
#include <iomanip>

namespace otb
{
namespace Functor
{
/**
   * \class ReflectanceToRadianceImageFunctor
   * \brief Compute radiance from the reflectance value
   *
   *  Divide by Pi and multiply by an illumination correction coefficient
   *  and the given solar illumination.
   *
   *
   * \sa ReflectanceToRadianceImageFilter
   *
   * \ingroup Functor
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInput, class TOutput>
class ReflectanceToRadianceImageFunctor
{
public:
  ReflectanceToRadianceImageFunctor() :
    m_SolarIllumination(1.0),
    m_IlluminationCorrectionCoefficient(1.0)
  {}

  virtual ~ReflectanceToRadianceImageFunctor() {}

  void SetSolarIllumination(double solarIllumination)
  {
    m_SolarIllumination = solarIllumination;
  }
  void SetIlluminationCorrectionCoefficient(double coef)
  {
    m_IlluminationCorrectionCoefficient = coef;
  }

  double GetSolarIllumination()
  {
    return m_SolarIllumination;
  }
  double GetIlluminationCorrectionCoefficient()
  {
    return m_IlluminationCorrectionCoefficient;
  }

  inline TOutput operator ()(const TInput& inPixel) const
  {
    TOutput outPixel;
    double  temp;
    temp = static_cast<double>(inPixel)
           / static_cast<double>(CONST_PI)
           * m_IlluminationCorrectionCoefficient
           * m_SolarIllumination;

    outPixel = static_cast<TOutput>(temp);
    return outPixel;
  }

private:
  double m_SolarIllumination;
  double m_IlluminationCorrectionCoefficient;
};
}

/** \class ReflectanceToRadianceImageFilter
 *  \brief Convert reflectance value into radiance value
 *
 * Transform a reflectance image into the radiance. For this it uses the
 * functor ReflectanceToRadianceImageFunctor calling for each component of each pixel.
 *
 *
 * For Spot image in the dimap format, the correction parameters are
 * retrieved automatically from the metadata
 *
 * \ingroup ImageToRadianceImageFunctor
 * \ingroup Radiometry
 *
 * \example Radiometry/AtmosphericCorrectionSequencement.cxx
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReflectanceToRadianceImageFilter :
  public UnaryImageFunctorWithVectorImageFilter<TInputImage,
      TOutputImage,
      typename Functor::ReflectanceToRadianceImageFunctor<typename
          TInputImage::
          InternalPixelType,
          typename
          TOutputImage::
          InternalPixelType> >
{
public:
  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename Functor::ReflectanceToRadianceImageFunctor<typename InputImageType::InternalPixelType,
      typename OutputImageType::InternalPixelType>
  FunctorType;

  /** "typedef" for standard classes. */
  typedef ReflectanceToRadianceImageFilter                                                    Self;
  typedef UnaryImageFunctorWithVectorImageFilter<InputImageType, OutputImageType, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>                                                              Pointer;
  typedef itk::SmartPointer<const Self>                                                        ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(ReflectanceToRadianceImageFilter, UnaryImageFunctorWithVectorImageFiltermageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::InternalPixelType  InputInternalPixelType;
  typedef typename InputImageType::RegionType         InputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;

  typedef typename itk::VariableLengthVector<double> VectorType;

  /** Image size "typedef" definition. */
  typedef typename InputImageType::SizeType SizeType;

  /** Set the solar illumination value. */
  itkSetMacro(SolarIllumination, VectorType);
  /** Give the solar illumination value. */
  itkGetConstReferenceMacro(SolarIllumination, VectorType);

  /** Set the zenithal solar angle. */
  itkSetMacro(ZenithalSolarAngle, double);
  /** Give the zenithal solar angle. */
  itkGetConstReferenceMacro(ZenithalSolarAngle, double);

  /** Set/Get the sun elevation angle (internally handled by the zenithal angle)*/
  virtual void SetElevationSolarAngle(double elevationAngle)
  {
    double zenithalAngle = 90.0 - elevationAngle;
    if (this->m_ZenithalSolarAngle != zenithalAngle)
      {
      this->m_ZenithalSolarAngle = zenithalAngle;
      this->Modified();
      }
  }

  virtual double GetElevationSolarAngle() const
  {
    return 90.0 - this->m_ZenithalSolarAngle;
  }

  /** Set the day. */
  itkSetClampMacro(Day, int, 1, 31);
  /** Give the day. */
  itkGetConstReferenceMacro(Day, int);

  /** Set the month. */
  itkSetClampMacro(Month, int, 1, 12);
  /** Give the month. */
  itkGetConstReferenceMacro(Month, int);

  /** Set the flux normalization coefficient. */
  void SetFluxNormalizationCoefficient(double coef)
  {
    m_FluxNormalizationCoefficient = coef;
    m_IsSetFluxNormalizationCoefficient = true;
    this->Modified();
  }
  /** Give the flux normalization coefficient. */
  itkGetConstReferenceMacro(FluxNormalizationCoefficient, double);

  /** Set the IsSetFluxNormalizationCoefficient boolean. */
  itkSetMacro(IsSetFluxNormalizationCoefficient, bool);
  /** Give the IsSetFluxNormalizationCoefficient boolean. */
  itkGetConstReferenceMacro(IsSetFluxNormalizationCoefficient, bool);

  /** Set the UseClamp boolean. */
  itkSetMacro(UseClamp, bool);
  /** Give the UseClamp boolean. */
  itkGetConstReferenceMacro(UseClamp, bool);

protected:
  /** Constructor */
  ReflectanceToRadianceImageFilter() :
    m_ZenithalSolarAngle(120.0), //invalid value which will lead to negative radiometry
    m_FluxNormalizationCoefficient(1.),
    m_Day(0),
    m_Month(0),
    m_IsSetFluxNormalizationCoefficient(false)
    {
    m_SolarIllumination.SetSize(0);
    };

  /** Destructor */
  ~ReflectanceToRadianceImageFilter() override {}

  /** Update the functor list and input parameters */
  void BeforeThreadedGenerateData(void) override
  {
    OpticalImageMetadataInterface::Pointer imageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(
      this->GetInput()->GetMetaDataDictionary());
    if ((m_Day == 0) && (!m_IsSetFluxNormalizationCoefficient))
      {
      m_Day = imageMetadataInterface->GetDay();
      }

    if ((m_Month == 0) && (!m_IsSetFluxNormalizationCoefficient))
      {
      m_Month = imageMetadataInterface->GetMonth();
      }

    if (m_SolarIllumination.GetSize() == 0)
      {
      m_SolarIllumination = imageMetadataInterface->GetSolarIrradiance();
      }

    if (m_ZenithalSolarAngle == 120.0)
      {
      //the zenithal angle is the complementary of the elevation angle
      m_ZenithalSolarAngle = 90.0 - imageMetadataInterface->GetSunElevation();
      }

    std::cout << "Using correction parameters: " << std::endl;
    std::cout<< "Day:               " << m_Day << std::endl;
    std::cout<< "Month:             " << m_Month << std::endl;
    std::cout<< "Solar irradiance:  " << m_SolarIllumination << std::endl;
    std::cout<< "Zenithal angle:    " << m_ZenithalSolarAngle << std::endl;

    if ((m_SolarIllumination.GetSize() != this->GetInput()->GetNumberOfComponentsPerPixel()))
      {
      itkExceptionMacro(<< "SolarIllumination parameter should have the same size as the number of bands");
      }

    this->GetFunctorVector().clear();

    for (unsigned int i = 0; i < this->GetInput()->GetNumberOfComponentsPerPixel(); ++i)
      {
      FunctorType functor;
      double      coefTemp = 0.;
      if (!m_IsSetFluxNormalizationCoefficient)
        {
        if (m_Day * m_Month != 0 && m_Day < 32 && m_Month < 13)
          {
          coefTemp = std::cos(m_ZenithalSolarAngle * CONST_PI_180) * VarSol::GetVarSol(m_Day,m_Month);
          }
        else
          {
          itkExceptionMacro(<< "Day has to be included between 1 and 31, Month between 1 and 12.");
          }
        }
      else
        {
        coefTemp =
          std::cos(m_ZenithalSolarAngle *
                  CONST_PI_180) * m_FluxNormalizationCoefficient * m_FluxNormalizationCoefficient;
        }
      functor.SetIlluminationCorrectionCoefficient(coefTemp);
      functor.SetSolarIllumination(static_cast<double>(m_SolarIllumination[i]));

      this->GetFunctorVector().push_back(functor);
      }
  }

private:

  /** Set the zenithal soalr angle. */
  double m_ZenithalSolarAngle;
  /** Flux normalization coefficient. */
  double m_FluxNormalizationCoefficient;
  /** Acquisition day. */
  int m_Day;
  /** Acquisition month. */
  int m_Month;
  /** Solar illumination value. */
  VectorType m_SolarIllumination;
  /** Used to know if the user has set a value for the FluxNormalizationCoefficient parameter
   * or if the class has to compute it */
  bool m_IsSetFluxNormalizationCoefficient;
  /** Clamp values to [0,1] */
  bool m_UseClamp;

};

} // end namespace otb
#endif
