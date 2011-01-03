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
#ifndef __otbLeafParametersToProspectLeafOpticalProperties_h
#define __otbLeafParametersToProspectLeafOpticalProperties_h


#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "otbLeafParameters.h"
#include "otbLeafOpticalProperties.h"
#include "dataSpec_P5B.h"

namespace otb
{
/** \class LeafParametersToProspectLeafOpticalProperties
   * \brief this class implements the LeafParametersToProspectLeafOpticalProperties lib.
   *
   * \sa itk::DataObject
 */

class ITK_EXPORT LeafParametersToProspectLeafOpticalProperties : public itk::ProcessObject
{
   public:
      /** Standard class typedefs */
      typedef LeafParametersToProspectLeafOpticalProperties Self;
      typedef itk::ProcessObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      typedef otb::LeafParameters        LeafParametersType;
      typedef otb::LeafOpticalProperties LeafOpticalPropertiesType;

      
      /** Standard macros */
      itkNewMacro(Self);
      itkTypeMacro(LeafParametersToProspectLeafOpticalProperties,ProcessObject);
      
      /** Set Input */
      void SetInput(const LeafParametersType *object);
      LeafParametersType * GetInput();
      /** GenerateData */
      virtual void GenerateData();
      
      /** Get Output */
      DataObjectPointer MakeOutput(unsigned int);
      
      virtual LeafOpticalPropertiesType * GetOutput();

      /** compute Transmission of isotropic radiation across an interface between two dielectrics*/
      double Tav(const int theta, double ref);

   protected:
      /** Constructor */
      LeafParametersToProspectLeafOpticalProperties();
      /** Destructor */
      virtual ~LeafParametersToProspectLeafOpticalProperties() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

   private:
      LeafParametersToProspectLeafOpticalProperties(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
      
      LeafParametersType::Pointer m_LeafParameters;

};

}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLeafParametersToProspectLeafOpticalProperties.cxx"
#endif

#endif
