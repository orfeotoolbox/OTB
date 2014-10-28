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
#ifndef __otbProspectModel_h
#define __otbProspectModel_h


#include "itkObjectFactory.h"
#include "otbLeafParameters.h"

#include "otbDataSpecP5B.h"
#include "otbSimulationStep1Base.h"

namespace otb
{
/** \class ProspectModel
   * \brief This class implements the Prospect Model (see http://teledetection.ipgp.jussieu.fr/prosail/),
   *
   * It computes leaf reflectance and transmittance from a set of parameters (contained in a LeafParameters class)
   *
   * \sa itk::DataObject
   * \sa SimulationStep1Base
 */

class ITK_EXPORT ProspectModel : public SimulationStep1Base
{
   public:
      /** Standard class typedefs */
      typedef ProspectModel Self;
      typedef SimulationStep1Base   Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      typedef otb::LeafParameters                   LeafParametersType;
      typedef Superclass::SpectralResponseType      SpectralResponseType;
      typedef Superclass::ParametersType            ParametersType;

      /** Standard macros */
      itkNewMacro(Self);
      itkTypeMacro(ProspectModel, SpectrumGeneratorBase);

      /** Set/Get Input */
      virtual  void SetInput(const ParametersType &);
      void SetInput(const LeafParametersType *object);
      LeafParametersType * GetInput();

      /** GenerateData */
      virtual void GenerateData();

      /** Get Output reflectance/transmittance*/
      virtual SpectralResponseType * GetReflectance();
      virtual SpectralResponseType * GetTransmittance();

   protected:
      /** Constructor */
      ProspectModel();
      /** Destructor */
      ~ProspectModel();
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

      DataObjectPointer MakeOutput(unsigned int);

      /** Compute Transmission of isotropic radiation across an interface between two dielectrics*/
      double Tav(const int theta, double ref);

   private:
      ProspectModel(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

};

}// end namespace otb

#endif
