/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbReduceSpectralResponseClassifierRAndNIR_h
#define otbReduceSpectralResponseClassifierRAndNIR_h


#include "otbReduceSpectralResponse.h"
#include "otbVegetationIndicesFunctor.h"


namespace otb
{
/** \class ReduceSpectralResponseClassifierRAndNIR
   * \brief this class represents a hierarchy of vector data.
   *
   * This class contains the spectral response from an object and the relative spectral
   * response of a satellite (one response per band)
   *
   * This class compute the reduce spectral response of each band of a sensor
   *
   * The two templates indicate
   * - the SpectralResponse input (from the JPL ASTER spectral library for example)
   * - the relative spectral response of a sensor
   *
   * The elements in the ReduceSpectralResponseClassifierRAndNIR have their coordinates in the projection
   * specified by the Wkt string. This class also offer the possibility to store
   * the coordinates directly into an image coordinate system, using the origin and
   * spacing in a similar way as the corresponding image.
   *
   * \sa itk::DataObject
   * \sa SpectralResponse
   * \sa SatelliteRSR
   *
 *
 * \ingroup OTBSimulation
 */
template <class TReduceSpectralResponse,
          class TFunction = Functor::NDVI<typename TReduceSpectralResponse::ValuePrecisionType, typename TReduceSpectralResponse::ValuePrecisionType>>
class ReduceSpectralResponseClassifierRAndNIR : public itk::DataObject
{
  // friend class
public:
  /** Standard class typedefs */
  typedef ReduceSpectralResponseClassifierRAndNIR Self;
  typedef itk::DataObject                         Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Template parameters typedef */
  typedef TReduceSpectralResponse                                      InputReduceSpectralResponseType;
  typedef TFunction                                                    FunctorType;
  typedef typename TReduceSpectralResponse::Pointer                    InputReduceSpectralResponsePointerType;
  typedef typename InputReduceSpectralResponseType::ValuePrecisionType ValuePrecisionType;


  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(ReduceSpectralResponseClassifierRAndNIR, DataObject);

  itkGetConstObjectMacro(InputReduceSpectralResponse, InputReduceSpectralResponseType);
  itkSetObjectMacro(InputReduceSpectralResponse, InputReduceSpectralResponseType);

  itkGetConstMacro(RBandNumber, unsigned int);
  itkSetMacro(RBandNumber, unsigned int);

  itkGetConstMacro(NIRBandNumber, unsigned int);
  itkSetMacro(NIRBandNumber, unsigned int);

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  };

  /** Set the functor object.  This replaces the current Functor with a
           * copy of the specified Functor. This allows the user to specify a
           * functor that has ivars set differently than the default functor.
           * This method requires an operator!=() be defined on the functor
           * (or the compiler's default implementation of operator!=() being
           * appropriate). */
          void SetFunctor(const FunctorType& functor)
          {
            m_Functor = functor;
          }
          /** Clear the vector data  */
          virtual bool Clear();


          /** PrintSelf method */
          void PrintSelf(std::ostream& os, itk::Indent indent) const override;


          /**
           * \param ValuePrecisionType
           * \return the indice value.
           */
          inline ValuePrecisionType operator()();


        protected:
          /** Constructor */
          ReduceSpectralResponseClassifierRAndNIR();
          /** Constructor from a ASCII file */
          //ReduceSpectralResponseClassifierRAndNIR( const std::string & filename );
          /** Destructor */
          ~ReduceSpectralResponseClassifierRAndNIR() override {};
          /** PrintSelf method */
          //void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
          ReduceSpectralResponseClassifierRAndNIR(const Self&) = delete;
          void operator=(const Self&) = delete;

          /** Input reduce spectral sensor model of response */
          InputReduceSpectralResponsePointerType  m_InputReduceSpectralResponse;

          /** Number of the red band in the ReduceSpectralResponse vector */
          unsigned int m_RBandNumber;

          /** Number of the NIR band in the ReduceSpectralResponse vector */
          unsigned int m_NIRBandNumber;

          /** Indice classifier */
          FunctorType m_Functor;
      };
}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbReduceSpectralResponseClassifierRAndNIR.hxx"
#endif

#endif
