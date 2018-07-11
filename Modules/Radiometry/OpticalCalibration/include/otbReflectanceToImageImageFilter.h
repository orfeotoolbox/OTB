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

#ifndef otbReflectanceToImageImageFilter_h
#define otbReflectanceToImageImageFilter_h

#include "otbRadianceToImageImageFilter.h"
#include "otbReflectanceToRadianceImageFilter.h"

namespace otb
{
namespace Functor
{
/** \class ReflectanceToImageImageFunctor
   *  \brief Call the ReflectanceToRadianceFunctor over the input and the RadianceToImageFunctor to this result.
   *
   *
   * \sa ReflectanceToImageImageFilter
   *
   * \ingroup Functor
   * \ingroup RadianceToImageFunctor
   * \ingroup ReflectanceToRadianceFunctor
   * \ingroup Radiometry
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInput, class TOutput>
class ReflectanceToImageImageFunctor
{
public:
  ReflectanceToImageImageFunctor() {}
  virtual ~ReflectanceToImageImageFunctor() {}

  typedef Functor::RadianceToImageImageFunctor<TInput, TOutput>       LumToImFunctorType;
  typedef Functor::ReflectanceToRadianceImageFunctor<TInput, TOutput> ReflecToLumFunctorType;

  void SetAlpha(double alpha)
  {
    m_LumToImFunctor.SetAlpha(alpha);
  }
  void SetBeta(double beta)
  {
    m_LumToImFunctor.SetBeta(beta);
  }
  void SetSolarIllumination(double solarIllumination)
  {
    m_ReflecToLumFunctor.SetSolarIllumination(solarIllumination);
  }
  void SetIlluminationCorrectionCoefficient(double coef)
  {
    m_ReflecToLumFunctor.SetIlluminationCorrectionCoefficient(coef);
  }

  double GetAlpha()
  {
    return m_LumToImFunctor.GetAlpha();
  }
  double GetBeta()
  {
    return m_LumToImFunctor.GetBeta();
  }
  double GetSolarIllumination()
  {
    return m_ReflecToLumFunctor.GetSolarIllumination();
  }
  double GetIlluminationCorrectionCoefficient()
  {
    return m_ReflecToLumFunctor.GetIlluminationCorrectionCoefficient();
  }

  inline TOutput operator ()(const TInput& inPixel) const
  {
    TOutput outPixel;
    TOutput tempPix;
    tempPix = m_ReflecToLumFunctor(inPixel);
    outPixel = m_LumToImFunctor(tempPix);

    return outPixel;
  }

private:
  LumToImFunctorType     m_LumToImFunctor;
  ReflecToLumFunctorType m_ReflecToLumFunctor;

};
}

/** \class ReflectanceToImageImageFilter
 *  \brief Convert a reflectance into a raw value value
 *
 *  Transform a reflectance image into a classical image image. For this it uses the functor ReflectanceToImageFunctor calling for each component of each pixel.
 *  The flux normalization coefficient (that is the ratio solar distance over mean solar distance) can be directly set or the user can
 *  give the day and the month of the observation and the class will used a coefficient given by a 6S routine that will give the corresponding coefficient.
 *  To note that in the case, 6S gives the square of the distances ratio.
 *
 *
 * For Spot image in the dimap format, the correction parameters are
 * retrieved automatically from the metadata
 *
 * \ingroup ReflectanceToImageImageFunctor
 * \ingroup RadianceToImageImageFilter
 * \ingroup ReflectanceToRadianceImageFilter
 * \ingroup Radiometry
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReflectanceToImageImageFilter :
  public UnaryImageFunctorWithVectorImageFilter<TInputImage,
      TOutputImage,
      typename Functor::ReflectanceToImageImageFunctor<typename
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
  typedef typename Functor::ReflectanceToImageImageFunctor<typename InputImageType::InternalPixelType,
      typename OutputImageType::InternalPixelType> FunctorType;

  /** "typedef" for standard classes. */
  typedef ReflectanceToImageImageFilter                                                        Self;
  typedef UnaryImageFunctorWithVectorImageFilter<InputImageType, OutputImageType, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>                                                              Pointer;
  typedef itk::SmartPointer<const Self>                                                        ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(ReflectanceToImageImageFilter, UnaryImageFunctorWithVectorImageFiltermageFilter);

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

  /** Set the absolute calibration gains. */
  itkSetMacro(Alpha, VectorType);
  /** Give the absolute calibration gains. */
  itkGetConstReferenceMacro(Alpha, VectorType);

  /** Set the absolute calibration bias. */
  itkSetMacro(Beta, VectorType);
  /** Give the absolute calibration bias. */
  itkGetConstReferenceMacro(Beta, VectorType);
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

  /** Set the flux normalization coefficient. */
  void SetFluxNormalizationCoefficient(double coef)
  {
    m_FluxNormalizationCoefficient = coef;
    m_IsSetFluxNormalizationCoefficient = true;
    this->Modified();
  }

  /** Set the acquisition day. */
  itkSetClampMacro(Day, int, 1, 31);
  /** Get the acquisition day. */
  itkGetConstReferenceMacro(Day, int);
  /** Set the acquisition month. */
  itkSetClampMacro(Month, int, 1, 12);
  /** Set the  acquisition month. */
  itkGetConstReferenceMacro(Month, int);

protected:
  /** Constructor */
  ReflectanceToImageImageFilter() :
    m_ZenithalSolarAngle(120.), //invalid value which will lead to negative radiometry
    m_FluxNormalizationCoefficient(1.),
    m_IsSetFluxNormalizationCoefficient(false),
    m_Day(0),
    m_Month(0)
    {
    m_Alpha.SetSize(0);
    m_Beta.SetSize(0);
    m_SolarIllumination.SetSize(0);
    };

  /** Destructor */
  ~ReflectanceToImageImageFilter() override {}

  /** Update the functor list and input parameters */
  void BeforeThreadedGenerateData(void) override
  {

    OpticalImageMetadataInterface::Pointer imageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(
      this->GetInput()->GetMetaDataDictionary());
    if (m_Alpha.GetSize() == 0)
      {
      m_Alpha = imageMetadataInterface->GetPhysicalGain();
      }

    if (m_Beta.GetSize() == 0)
      {
      m_Beta = imageMetadataInterface->GetPhysicalBias();
      }

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

    otbMsgDevMacro(<< "Using correction parameters: ");
    otbMsgDevMacro(<< "Alpha (gain): " << m_Alpha);
    otbMsgDevMacro(<< "Beta (bias):  " << m_Beta);
    otbMsgDevMacro(<< "Day:               " << m_Day);
    otbMsgDevMacro(<< "Month:             " << m_Month);
    otbMsgDevMacro(<< "Solar irradiance:  " << m_SolarIllumination);
    otbMsgDevMacro(<< "Zenithal angle:    " << m_ZenithalSolarAngle);

    if ((m_Alpha.GetSize() != this->GetInput()->GetNumberOfComponentsPerPixel())
        || (m_Beta.GetSize() != this->GetInput()->GetNumberOfComponentsPerPixel())
        || (m_SolarIllumination.GetSize() != this->GetInput()->GetNumberOfComponentsPerPixel()))
      {
      itkExceptionMacro(
        << "Alpha, Beta and SolarIllumination parameters should have the same size as the number of bands");
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
      functor.SetAlpha(m_Alpha[i]);
      functor.SetBeta(m_Beta[i]);
      functor.SetSolarIllumination(m_SolarIllumination[i]);
      this->GetFunctorVector().push_back(functor);
      }
  }

private:
  /** Ponderation declaration*/
  VectorType m_Alpha;
  VectorType m_Beta;
  /** Set the zenithal soalr angle. */
  double m_ZenithalSolarAngle;
  /** Flux normalization coefficient. */
  double m_FluxNormalizationCoefficient;
  /** Solar illumination value. */
  VectorType m_SolarIllumination;
  /** Used to know if the user has set a value for the FluxNormalizationCoefficient parameter
   * or if the class has to compute it */
  bool m_IsSetFluxNormalizationCoefficient;
  /** Acquisition Day*/
  int m_Day;
  /** Acquisition Month*/
  int m_Month;
};

} // end namespace otb

#endif
