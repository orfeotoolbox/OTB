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


#ifndef otbLabelMapToSimulatedImageFilter_h
#define otbLabelMapToSimulatedImageFilter_h


#include "itkLabelMapFilter.h"
#include "otbLabelToProSailParameters.h"
#include "otbReduceSpectralResponse.h"
#include "otbGaussianAdditiveNoiseSampleListFilter.h"
#include "otbSatelliteRSR.h"


namespace otb
{

/**
 * \class LabelMapToSimulatedImageFilter
 * \brief Transform a labelMap into an image.
 *
 * LabelMapToSimulatedImageFilter convert a label map into a reflectance image.
 * Reflectance of the image is computed by applying the RSR (relative spectral response) of the satellite to the spectrum obtained
 * by the spectrum simulator (or read in the database). A gaussian noise is added to the reflectance value of
 * each pixel of the object. However the method used to add this noise (itk::Statistics::MersenneTwisterRandomVariateGenerator)
 * is not thread safe, and then (even if all the remaining is multithread) the number of thread must be set to 1.
 *
 * \sa LabelMapFilter
 *
 * \ingroup OTBSimulation
 */

template <class TInputLabelMap, class TSimuStep1, class TSimuStep2, class TOutputImage>
class ITK_EXPORT LabelMapToSimulatedImageFilter :
  public itk::LabelMapFilter <TInputLabelMap, TOutputImage>

{
public:
   /**
   * Standard "Self" & Superclass typedef.
   */
   typedef LabelMapToSimulatedImageFilter                       Self;
   typedef itk::LabelMapFilter <TInputLabelMap, TOutputImage>   Superclass;
   typedef itk::SmartPointer<Self>                              Pointer;
   typedef itk::SmartPointer<const Self>                        ConstPointer;

   /** Some convenient typedefs. */
   typedef TInputLabelMap                              InputLabelMapType;
   typedef typename InputLabelMapType::Pointer         InputLabelMapPointer;
   typedef typename InputLabelMapType::ConstPointer    InputLabelMapConstPointer;
   typedef typename InputLabelMapType::LabelObjectType LabelObjectType;
   typedef typename LabelObjectType::ConstLineIterator ConstLineIteratorType;

   typedef typename LabelObjectType::AttributesValueType AttributesValueType;

   typedef TOutputImage                              OutputImageType;
   typedef typename OutputImageType::ConstPointer    OutputImageConstPointer;
   typedef typename OutputImageType::Pointer         OutputImagePointer;
   typedef typename OutputImageType::IndexType       IndexType;
   typedef typename OutputImageType::InternalPixelType InternalPixelType;

   typedef TSimuStep1                             SimulationStep1Type;
   typedef typename SimulationStep1Type::Pointer  SimulationStep1Pointer;
   typedef TSimuStep2                             SimulationStep2Type;
   typedef typename SimulationStep2Type::Pointer  SimulationStep2Pointer;

   //TODO: template ??
   typedef LabelToProSailParameters<AttributesValueType>    LabelToParametersType;
   typedef typename LabelToParametersType::Pointer          LabelToParametersPointer;

   typedef SpectralResponse<double, double>                 SpectralResponseType;
   typedef typename SpectralResponseType::Pointer           SpectralResponsePointer;
   typedef SatelliteRSR<double, double>                     SatelliteRSRType;
   typedef typename SatelliteRSRType::Pointer               SatelliteRSRPointer;

   typedef ReduceSpectralResponse<SpectralResponseType, SatelliteRSRType> ReduceSpectralResponseType;
   typedef typename ReduceSpectralResponseType::Pointer                   ReduceSpectralResponsePointer;

   typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
   typedef typename RandomGeneratorType::Pointer                  RandomGeneratorPointer;


   /** Standard Macro*/
   itkTypeMacro(LabelMapToSimulatedImageFilter, LabelMapFilter);
   itkNewMacro(Self);

   itkSetMacro(NumberOfComponentsPerPixel, unsigned int);
   itkGetMacro(NumberOfComponentsPerPixel, unsigned int);

   itkSetMacro(SatRSRFilename, std::string);
   itkGetMacro(SatRSRFilename, std::string);

   itkSetMacro(PathRoot, std::string);
   itkGetMacro(PathRoot, std::string);

   itkSetMacro(Mean, double);
   itkGetMacro(Mean, double);

   itkSetMacro(Variance, double);
   itkGetMacro(Variance, double);


protected:
   LabelMapToSimulatedImageFilter();
   ~LabelMapToSimulatedImageFilter() override {}
   void PrintSelf(std::ostream& os, itk::Indent indent) const override;

   void BeforeThreadedGenerateData() override;
   void ThreadedProcessLabelObject( LabelObjectType * labelObject ) override;
   void GenerateOutputInformation() override;


private:
   LabelMapToSimulatedImageFilter(const Self &) = delete;
   void operator =(const Self&) = delete;

   unsigned int m_NumberOfComponentsPerPixel;
   std::string m_SatRSRFilename;
   std::string m_PathRoot;
   /** Mean value of gaussien noise for spectra simulation.*/
   double m_Mean;
   /** Variance of gaussien noise for spectra simulation.*/
   double m_Variance;


};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapToSimulatedImageFilter.hxx"
#endif

#endif
