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


#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "otbLeafParameters.h"
#include "otbSpectralResponse.h"
// #include "otbLeafOpticalProperties.h"
#include "dataSpec_P5B.h"

namespace otb
{
/** \class ProspectModel
   * \brief this class implements the Prospect Model,
   * \brief which computes leaf reflectance and transmittance from a set of parameters (contained in an otbLeafPrameters)
   *
   * \sa itk::DataObject
 */

class ITK_EXPORT ProspectModel : public itk::ProcessObject
{
   public:
      /** Standard class typedefs */
      typedef ProspectModel Self;
      typedef itk::ProcessObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      typedef otb::LeafParameters        LeafParametersType;
//       typedef otb::LeafOpticalProperties LeafOpticalPropertiesType;
      typedef otb::SpectralResponse<double,double>      SpectralResponseType;
      
      /** Standard macros */
      itkNewMacro(Self);
      itkTypeMacro(ProspectModel,ProcessObject);
      
      /** Set/Get Input */
      void SetInput(const LeafParametersType *object);
      LeafParametersType * GetInput();
      
      /** GenerateData */
      virtual void GenerateData();
      
      /** Get Output reflectance/transmittance*/
//       virtual LeafOpticalPropertiesType * GetOutput();
      virtual SpectralResponseType * GetReflectance();
      virtual SpectralResponseType * GetTransmittance();

      /** Compute Transmission of isotropic radiation across an interface between two dielectrics*/
      double Tav(const int theta, double ref);

   protected:
      /** Constructor */
      ProspectModel();
      /** Destructor */
      virtual ~ProspectModel() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;
      
      DataObjectPointer MakeOutput(unsigned int);

   private:
      ProspectModel(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
      
      LeafParametersType::Pointer m_LeafParameters;

};

}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbProspectModel.cxx"
#endif

#endif
