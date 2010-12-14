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
#ifndef __otbProSail_h
#define __otbProSail_h


#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "otbProSailParameters.h"
#include "otbSpectralResponse.h"
#include "dataSpec_P5B.h"

namespace otb
{
/** \class ProSail
   * \brief this class implements the PROSAIL lib.
   *
   * \sa itk::DataObject
 */
template <class TSpectralResponse = otb::SpectralResponse<double,double> >
class ITK_EXPORT ProSail : public itk::DataObject
      {
        public:
           /** Standard class typedefs */
           typedef ProSail Self;
           typedef itk::DataObject Superclass;
           typedef itk::SmartPointer<Self> Pointer;
           typedef itk::SmartPointer<const Self> ConstPointer;
           
           typedef otb::ProSailParameters ProSailParametersType;
           typedef ProSailParametersType::Pointer ProSailParametersPointerType;

           typedef TSpectralResponse SpectralResponseType;
           typedef typename SpectralResponseType::Pointer SpectralResponsePointerType;
           typedef typename SpectralResponseType::PrecisionType PrecisionType;
           typedef typename SpectralResponseType::ValuePrecisionType ValuePrecisionType;
           typedef typename SpectralResponseType::PairType PairType;
          
           typedef std::vector<double>             VectorType;
           typedef std::vector< VectorType >       LRTType;
          
          
           /** Standard macros */
           itkNewMacro(Self);
           itkTypeMacro(ProSail,DataObject);

           /** Set/Get all parameters using  otbProSailParameters*/
           itkGetConstObjectMacro(ProSailParameters, ProSailParametersType);
           itkSetObjectMacro(ProSailParameters, ProSailParametersType);

           /** Main method computing PROSAIL */
           void Compute();
          
           /** Get Outputs */
           //TODO const ou pas const: itkGetConstObjectMacro ou itkGetObjectMacro?
           itkGetObjectMacro(Resh, SpectralResponseType); //hemispherical reflectance
           itkGetObjectMacro(Resv, SpectralResponseType); //Directional reflectance
          
          
           //TODO place into protected ?
           /** compute Leaf Reflectance and Transmittance */
//           void Prospect5B(const double N, const double Cab, const double Car, const double CBrown, const double Cw, const double Cm, LRTType &LRT);
           void Prospect5B(LRTType &LRT);
           
           /** compute Transmission of isotropic radiation across an interface between two dielectrics*/
           double Tav(const int theta, double ref);
           
           /** Compute Leaf Angle Distribution */
           void Calc_LIDF(const double a, VectorType &lidf);
           void Campbell(const double ala, VectorType &freq);
          
           /** J functions */
           double Jfunc1(const double k, const double l, const double t);
           double Jfunc2(const double k, const double l, const double t);
           double Jfunc3(const double k, const double l, const double t);
           /** Volscatt */
           void Volscatt(const double tts, const double tto, const double psi, const double ttl, VectorType &result);


        protected:
           /** Constructor */
           ProSail();
           /** Destructor */
           virtual ~ProSail() {};
           /** PrintSelf method */
           void PrintSelf(std::ostream& os, itk::Indent indent) const;
          
         
          
        private:
           ProSail(const Self&); //purposely not implemented
           void operator=(const Self&); //purposely not implemented
          
           ProSailParametersPointerType m_ProSailParameters;
          
           SpectralResponsePointerType m_Resh;
           SpectralResponsePointerType m_Resv;


      };
}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProSail.txx"
#endif

#endif
