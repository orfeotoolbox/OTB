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
#ifndef __otbLuminanceToReflectanceImageFilter_h
#define __otbLuminanceToReflectanceImageFilter_h

#include "otb_6S.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbMath.h"
#include "itkNumericTraits.h"
#include "itkVariableLengthVector.h"


namespace otb
{
namespace Functor
{
/** Functor::LuminanceToReflectanceImageFunctor
 *  Multiply by Pi and by an illumination correction coefficient the quotient between the input and the given solar illumination.
 *
 * \ingroup Functor
 */
template <class TInput, class TOutput>
class LuminanceToReflectanceImageFunctor
{
public:
  LuminanceToReflectanceImageFunctor()
  {
    m_SolarIllumination = 1.;
    m_IlluminationCorrectionCoefficient = 1.;
  };
  ~LuminanceToReflectanceImageFunctor() {};

  void SetSolarIllumination(double solarIllumination)
  {
    m_SolarIllumination = solarIllumination;
  };
  void SetIlluminationCorrectionCoefficient(double coef)
  {
    m_IlluminationCorrectionCoefficient = coef;
  };

  double GetSolarIllumination()
  {
    return m_SolarIllumination;
  };
  double GetIlluminationCorrectionCoefficient()
  {
    return m_IlluminationCorrectionCoefficient;
  };

  inline TOutput operator() (const TInput & inPixel)
  {
    TOutput outPixel;
    double temp;
    temp = static_cast<double>(inPixel)
           * static_cast<double>(M_PI)
           * m_IlluminationCorrectionCoefficient
           / m_SolarIllumination;
    outPixel = static_cast<TOutput>(temp);
    return outPixel;
  }

private:
  double m_SolarIllumination;
  double m_IlluminationCorrectionCoefficient;
};
}

/** \class LuminanceToReflectanceImageFilter
 *  \brief Transform a luminance image into the reflectance. For this it uses the functor LuminanceToReflectanceImageFunctor
 *   calling for each component of each pixel.
 *
 * \ingroup ImageToLuminanceImageFunctor
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LuminanceToReflectanceImageFilter :
      public UnaryImageFunctorWithVectorImageFilter< TInputImage,
      TOutputImage,
      ITK_TYPENAME Functor::LuminanceToReflectanceImageFunctor< ITK_TYPENAME TInputImage::InternalPixelType,
      ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::LuminanceToReflectanceImageFunctor<ITK_TYPENAME InputImageType::InternalPixelType,
  ITK_TYPENAME OutputImageType::InternalPixelType> FunctorType;


  /** "typedef" for standard classes. */
  typedef LuminanceToReflectanceImageFilter Self;
  typedef UnaryImageFunctorWithVectorImageFilter< InputImageType, OutputImageType, FunctorType > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(LuminanceToReflectanceImageFilter, UnaryImageFunctorWithVectorImageFiltermageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType                           InputPixelType;
  typedef typename InputImageType::InternalPixelType                   InputInternalPixelType;
  typedef typename InputImageType::RegionType                          InputImageRegionType;
  typedef typename OutputImageType::PixelType                          OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                  OutputInternalPixelType;
  typedef typename OutputImageType::RegionType                         OutputImageRegionType;


  typedef typename itk::VariableLengthVector<double>                   VectorType;

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
  };
  /** Give the flux normalization coefficient. */
  itkGetConstReferenceMacro(FluxNormalizationCoefficient, double);

  /** Set the IsSetFluxNormalizationCoefficient boolean. */
  itkSetMacro(IsSetFluxNormalizationCoefficient, bool);
  /** Give the IsSetFluxNormalizationCoefficient boolean. */
  itkGetConstReferenceMacro(IsSetFluxNormalizationCoefficient, bool);

protected:
  /** Constructor */
  LuminanceToReflectanceImageFilter()
  {
    m_ZenithalSolarAngle = 1.;
    m_FluxNormalizationCoefficient = 1.;
    m_SolarIllumination.SetSize(1);
    m_SolarIllumination.Fill(1.);
    m_Month = 1;
    m_Day = 1;
    m_IsSetFluxNormalizationCoefficient = false;
  };
  /** Destructor */
  virtual ~LuminanceToReflectanceImageFilter() {};

  /** Update the functor list */
  virtual void BeforeThreadedGenerateData(void)
  {
    this->GetFunctorVector().clear();

    for (unsigned int i = 0;i<this->GetInput()->GetNumberOfComponentsPerPixel();++i)
    {
      FunctorType functor;
      double coefTemp = 0.;
      if (!m_IsSetFluxNormalizationCoefficient)
      {
        if (m_Day*m_Month != 0 && m_Day<32 && m_Month<13)
        {
          otb_6s_doublereal dsol = 0.;
          otb_6s_integer day = static_cast<otb_6s_integer>(m_Day);
          otb_6s_integer month = static_cast<otb_6s_integer>(m_Month);
          int cr(0);
          cr = otb_6s_varsol_(&day, &month, &dsol);
          coefTemp = vcl_cos(m_ZenithalSolarAngle*M_PI/180.)*static_cast<double>(dsol);
        }
        else
        {
          itkExceptionMacro( << "Day has to be included between 1 and 31, Month beetween 1 and 12.");
        }
      }
      else
      {
        coefTemp = vcl_cos(m_ZenithalSolarAngle*M_PI/180.)*m_FluxNormalizationCoefficient*m_FluxNormalizationCoefficient;
      }
      functor.SetIlluminationCorrectionCoefficient(1. / coefTemp);
      functor.SetSolarIllumination(static_cast<double>(m_SolarIllumination[i]));

      this->GetFunctorVector().push_back(functor);
    }
  }

private:

  /** Set the zenithal soalr angle. */
  double m_ZenithalSolarAngle;
  /** Flux normalization coefficient. */
  double m_FluxNormalizationCoefficient;
  /* Acquisition day. */
  int m_Day;
  /* Acquisition mounth. */
  int m_Month;
  /** Solar illumination value. */
  VectorType m_SolarIllumination;
  /** Used to know if the user has set a value for the FluxNormalizationCoefficient parameter
   * or if the class has to compute it */
  bool m_IsSetFluxNormalizationCoefficient;

};

} // end namespace otb
#endif
