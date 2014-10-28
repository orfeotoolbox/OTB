/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbReflectanceToLuminanceImageFilter_h
#define __otbReflectanceToLuminanceImageFilter_h

#include "otb_6S.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbMath.h"
#include "itkVariableLengthVector.h"
#include "otbMacro.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"
#include <iomanip>

namespace otb
{
namespace Functor
{
/**
   * \class ReflectanceToLuminanceImageFunctor
   * \brief Compupute luminance from the reflectance value
   *
   *  Divide by Pi and multiply by an illumination correction coefficient
   *  and the given solar illumination.
   *
   *
   * \sa ReflectanceToLuminanceImageFilter
   *
   * \ingroup Functor
   * \ingroup Radiometry
   *
 */
template <class TInput, class TOutput>
class ReflectanceToLuminanceImageFunctor
{
public:
  ReflectanceToLuminanceImageFunctor() :
    m_SolarIllumination(1.0),
    m_IlluminationCorrectionCoefficient(1.0)
  {}

  virtual ~ReflectanceToLuminanceImageFunctor() {}

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

/** \class ReflectanceToLuminanceImageFilter
 *  \brief Convert reflectance value into luminance value
 *
 * Transform a reflectance image into the luminance. For this it uses the
 * functor ReflectanceToLuminanceImageFunctor calling for each component of each pixel.
 *
 *
 * For Spot image in the dimap format, the correction parameters are
 * retrieved automatically from the metadata
 *
 * \ingroup ImageToLuminanceImageFunctor
 * \ingroup Radiometry
 *
 * \example Radiometry/AtmosphericCorrectionSequencement.cxx
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReflectanceToLuminanceImageFilter :
  public UnaryImageFunctorWithVectorImageFilter<TInputImage,
      TOutputImage,
      typename Functor::ReflectanceToLuminanceImageFunctor<typename
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
  typedef typename Functor::ReflectanceToLuminanceImageFunctor<typename InputImageType::InternalPixelType,
      typename OutputImageType::InternalPixelType>
  FunctorType;

  /** "typedef" for standard classes. */
  typedef ReflectanceToLuminanceImageFilter                                                    Self;
  typedef UnaryImageFunctorWithVectorImageFilter<InputImageType, OutputImageType, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>                                                              Pointer;
  typedef itk::SmartPointer<const Self>                                                        ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(ReflectanceToLuminanceImageFilter, UnaryImageFunctorWithVectorImageFiltermageFilter);

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

  /** Set the mounth. */
  itkSetClampMacro(Month, int, 1, 12);
  /** Give the mounth. */
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
  ReflectanceToLuminanceImageFilter() :
    m_ZenithalSolarAngle(120.0), //invalid value which will lead to negative radiometry
    m_FluxNormalizationCoefficient(1.),
    m_Day(0),
    m_Month(0),
    m_IsSetFluxNormalizationCoefficient(false)
    {
    m_SolarIllumination.SetSize(0);
    };

  /** Destructor */
  virtual ~ReflectanceToLuminanceImageFilter() {}

  /** Update the functor list and input parameters */
  virtual void BeforeThreadedGenerateData(void)
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
          otb_6s_doublereal dsol = 0.;
          otb_6s_integer    day = static_cast<otb_6s_integer>(m_Day);
          otb_6s_integer    month = static_cast<otb_6s_integer>(m_Month);
          //int               cr(0);
          otb_6s_varsol_(&day, &month, &dsol);
          coefTemp = vcl_cos(m_ZenithalSolarAngle * CONST_PI_180) * static_cast<double>(dsol);
          }
        else
          {
          itkExceptionMacro(<< "Day has to be included between 1 and 31, Month beetween 1 and 12.");
          }
        }
      else
        {
        coefTemp =
          vcl_cos(m_ZenithalSolarAngle *
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
  /** Acquisition mounth. */
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
