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
#ifndef __otbSailModel_h
#define __otbSailModel_h


#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "otbSpectralResponse.h"
#include "dataSpec_P5B.h"

namespace otb
{
/** \class SailModel
   * \brief this class implements the SailModel lib.
   *
   * \sa itk::ProcessObject
 */

class ITK_EXPORT SailModel : public itk::ProcessObject
{
   public:
      /** Standard class typedefs */
      typedef SailModel Self;
      typedef itk::ProcessObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      typedef SpectralResponse<double,double>      SpectralResponseType;
      typedef std::vector<double> VectorType;
      
      /** Standard macros */
      itkNewMacro(Self);
      itkTypeMacro(SailModel, ProcessObject);

      
      /** Set Inputs */
      void SetReflectance(const SpectralResponseType *object);
      SpectralResponseType * GetReflectance();
      
      void SetTransmittance(const SpectralResponseType *object);
      SpectralResponseType * GetTransmittance();
      
      /** Set parameters */
      /** Set/Get leaf area index */
      itkSetMacro(LAI, double);
      itkGetMacro(LAI, double);
      
      /** Set/Get average leaf angle (°)*/
      itkSetMacro(Angl, double);
      itkGetMacro(Angl, double);
      
      /** Set/Get soil coefficient*/
      itkSetMacro(PSoil, double);
      itkGetMacro(PSoil, double);
      
      /** Set/Get diffuse/direct radiation*/
      itkSetMacro(Skyl, double);
      itkGetMacro(Skyl, double);
      
      /** Set/Get hot spot */
      itkSetMacro(HSpot, double);
      itkGetMacro(HSpot, double);
      
      /** Set/Get solar zenith angle (°) */
      itkSetMacro(TTS, double);
      itkGetMacro(TTS, double);
      
      /** Set/Get observer zenith angle (°) */
      itkSetMacro(TTO, double);
      itkGetMacro(TTO, double);
      
      /** Set/Get azimuth(°) */
      itkSetMacro(PSI, double);
      itkGetMacro(PSI, double);
      
      /** GenerateData */
      virtual void GenerateData();
      
      /** Get Output */
      virtual SpectralResponseType * GetVerticalReflectance();
      virtual SpectralResponseType * GetHorizontalReflectance();
      
      
      
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
      SailModel();
      /** Destructor */
      virtual ~SailModel() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;
      
      virtual DataObjectPointer MakeOutput(unsigned int);

   private:
      SailModel(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
      
      double m_LAI; //leaf area index
      double m_Angl; //average leaf angle
      double m_PSoil; //soil coefficient
      double m_Skyl; //diffuse/direct radiation
      double m_HSpot; //hot spot
      double m_TTS; //solar zenith angle
      double m_TTO; //observer zenith angle
      double m_PSI; //azimuth

};

}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbSailModel.cxx"
#endif

#endif
