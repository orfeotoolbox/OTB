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

#ifndef __otbSimulationStep1Base_h
#define __otbSimulationStep1Base_h


#include "itkProcessObject.h"
#include "otbSpectralResponse.h"


namespace otb
{

/**
 * \class SimulationStep1Base
 * \brief Base class for all method that simulate the Reflectance and Transmittance of an object. (ex: Prospect)
 *
 * \sa ProcessObject
 */


class ITK_EXPORT SimulationStep1Base : public itk::ProcessObject
{
public:

   /**Standard "Self" & Superclass typedef*/
   typedef SimulationStep1Base                 Self;
   typedef itk::ProcessObject                  Superclass;
   typedef itk::SmartPointer<Self>             Pointer;
   typedef itk::SmartPointer<const Self>       ConstPointer;

   /** Some convenient typedefs. */
   typedef double                                ParametersValueType;
   typedef itk::Array<ParametersValueType>       ParametersType;
   typedef otb::SpectralResponse<double, double>  SpectralResponseType;


   /** Standard Macro*/
   itkTypeMacro(SimulationStep1Base, ProcessObject);

   itkSetMacro(Parameters, ParametersType)
   itkGetMacro(Parameters, ParametersType)
   //virtual  void SetInput(const ParametersType &) = 0;

   virtual SpectralResponseType * GetReflectance() = 0;
   virtual SpectralResponseType * GetTransmittance() = 0;


protected:
   SimulationStep1Base(){};
   virtual ~SimulationStep1Base() {}



private:
   SimulationStep1Base(const Self &); //purposely not implemented
   void operator =(const Self&); //purposely not implemented

   ParametersType m_Parameters;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
