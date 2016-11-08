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
#ifndef otbProspectModel_h
#define otbProspectModel_h

#include "OTBSimulationExport.h"
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
 *
 * \ingroup OTBSimulation
 */

class OTBSimulation_EXPORT ProspectModel : public SimulationStep1Base
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

      typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

      /** Standard macros */
      itkNewMacro(Self);
      itkTypeMacro(ProspectModel, SpectrumGeneratorBase);

      /** Set/Get Input */
      using Superclass::SetInput;
      virtual  void SetInput(const ParametersType &);
      void SetInput(const LeafParametersType *object);
      LeafParametersType * GetInput();

      /** GenerateData */
      void GenerateData() ITK_OVERRIDE;

      /** Get Output reflectance/transmittance*/
      SpectralResponseType * GetReflectance() ITK_OVERRIDE;
      SpectralResponseType * GetTransmittance() ITK_OVERRIDE;

   protected:
      /** Constructor */
      ProspectModel();
      /** Destructor */
      ~ProspectModel() ITK_OVERRIDE;
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

      DataObjectPointer MakeOutput(DataObjectPointerArraySizeType) ITK_OVERRIDE;
      using Superclass::MakeOutput;

      /** Compute Transmission of isotropic radiation across an interface between two dielectrics*/
      double Tav(const int theta, double ref);

   private:
      ProspectModel(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

};

}// end namespace otb

#endif
