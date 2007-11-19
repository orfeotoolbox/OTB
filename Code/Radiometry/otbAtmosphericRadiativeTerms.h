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
#ifndef _otbAtmosphericRadiativeTerms_h
#define _otbAtmosphericRadiativeTerms_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"

namespace otb 
{
/** \class AtmosphericRadiativeTerms
 *  \brief This class contain all atmospheric radiative terms.
 *
 * Each value can be deducted from the atmospheric correction parameters (using 6S) or directly set by the user.
 */

class ITK_EXPORT AtmosphericRadiativeTerms : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericRadiativeTerms     Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(AtmosphericRadiativeTerms,DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);


  /**
   * Set/Get the intrinsic atmospheric reflectance.
   */
  itkSetMacro(IntrinsicAtmosphericReflectance,double); 
  itkGetMacro(IntrinsicAtmosphericReflectance,double); 


  /**
   * Set/Get the spherical albedo of the atmosphere.
   */
  itkSetMacro(SphericalAlbedo,double); 
  itkGetMacro(SphericalAlbedo,double); 
  /**
   * Set/Get the ozone transmission.
   */
  itkSetMacro(OzoneTransmission,double); 
  itkGetMacro(OzoneTransmission,double); 

  /**
   * Set/Get the ozygen transmission.
   */
  itkSetMacro(OxygenTransmission,double); 
  itkGetMacro(OxygenTransmission,double); 

  /**
   * Set/Get the water vapor transmission.
   */
  itkSetMacro(WaterVaporTransmission,double); 
  itkGetMacro(WaterVaporTransmission,double); 
 
  /**
   * Set/Get the total transmittance of the atmosphere.
   */
  itkSetMacro(TotalTransmittance,double); 
  itkGetMacro(TotalTransmittance,double); 

 

protected:
  /** Constructor */
  AtmosphericRadiativeTerms(){};
  /** Destructor */
  ~AtmosphericRadiativeTerms(){};
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AtmosphericRadiativeTerms(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented


  /** The intrinsic atmospheric reflectance. */
  double m_IntrinsicAtmosphericReflectance; 
  
  /** The spherical albedo of the atmosphere.*/
  double m_SphericalAlbedo; 

  /** The ozone transmission. */
  double m_OzoneTransmission; 

  /** The ozygen transmission. */
  double m_OxygenTransmission; 

  /** The water vapor transmission. */
  double m_WaterVaporTransmission; 

  /** The total transmittance of the atmosphere. */
  double m_TotalTransmittance; 


};

} // end namespace otb

#endif


