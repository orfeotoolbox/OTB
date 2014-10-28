/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbReduceSpectralResponseClassifierRAndNIR_h
#define __otbReduceSpectralResponseClassifierRAndNIR_h



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
 */
  template <class TReduceSpectralResponse , class TFunction = Functor::NDVI< typename TReduceSpectralResponse::ValuePrecisionType, typename TReduceSpectralResponse::ValuePrecisionType,
  typename TReduceSpectralResponse::ValuePrecisionType > >
      class ReduceSpectralResponseClassifierRAndNIR
  : public itk::DataObject
      {
        //friend class
        public:
          /** Standard class typedefs */
          typedef ReduceSpectralResponseClassifierRAndNIR Self;
          typedef itk::DataObject Superclass;
          typedef itk::SmartPointer<Self> Pointer;
          typedef itk::SmartPointer<const Self> ConstPointer;

          /** Template parameters typedef */
          typedef TReduceSpectralResponse InputReduceSpectralResponseType;
          typedef TFunction   FunctorType;
          typedef typename TReduceSpectralResponse::Pointer InputReduceSpectralResponsePointerType;
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
          void PrintSelf(std::ostream& os, itk::Indent indent) const;


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
          virtual ~ReduceSpectralResponseClassifierRAndNIR() {};
          /** PrintSelf method */
          //void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
          ReduceSpectralResponseClassifierRAndNIR(const Self&); //purposely not implemented
          void operator=(const Self&); //purposely not implemented

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
#include "otbReduceSpectralResponseClassifierRAndNIR.txx"
#endif

#endif
