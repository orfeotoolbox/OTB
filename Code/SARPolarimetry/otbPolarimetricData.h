/*=================g=======================================================

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
#ifndef  __otb_PolarimetricData_h
#define  __otb_PolarimetricData_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "otbMacro.h"

namespace otb
{
/**
 * This enumeration describes the different architectures we can find in polarimetry.
 */
 typedef enum {
 HH_HV_VH_VV=0,
 HH_HV_VV=1,
 HH_VH_VV=2, 
 HH_HV=3,
 VH_VV=4,
 HH_VV=5,
 UNKNOWN=6} ArchitectureType;  
 
 
 /** \class PolarimetricData
 *  \brief This class allows to determine the type of architecture we get.
 *
 *
 * \sa 
 */

class ITK_EXPORT PolarimetricData : public itk::DataObject
{
  public:
 
  /** Standard typedefs */
  typedef PolarimetricData                           Self;
  typedef itk::DataObject                            Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Type macro */
  itkTypeMacro(PolarimetricData,DataObject);
    
  /** Creation through object factory macro */
  itkNewMacro(Self);
  
  /** Determine the kind of architecture of the input */
  void DetermineArchitecture(bool *PresentInputImages);
  void DetermineArchitecture(int NumberOfImages, bool EmissionH,bool EmissionV);

  /** Set/Get ArchitectureType */
  itkSetMacro(ArchitectureType,ArchitectureType);
  itkGetMacro(ArchitectureType,ArchitectureType);     
  
  protected:
  /** Constructor */
  PolarimetricData();
  /** Destructor */
  ~PolarimetricData(){};  
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;      

  
  private:
  PolarimetricData(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
   
  /** Architecture Type */
  ArchitectureType m_ArchitectureType;

};

} // end namespace otb

#endif
