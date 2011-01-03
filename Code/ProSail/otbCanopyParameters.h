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
#ifndef __otbCanopyParameters_h
#define __otbCanopyParameters_h


#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"


namespace otb
{
/** \class CanopyParameters
   * \brief this class implements the CanopyParameters lib.
   *
   * \sa itk::DataObject
 */
 
class ITK_EXPORT CanopyParameters : public itk::DataObject
{
   public:
      /** Standard class typedefs */
      typedef CanopyParameters Self;
      typedef itk::DataObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      /** Standard macros */
      itkTypeMacro(CanopyParameters, DataObject);
      itkNewMacro(Self);
      
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


   protected:
      /** Constructor */
      CanopyParameters();
      /** Destructor */
      virtual ~CanopyParameters() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

      
   private:
      CanopyParameters(const Self&); //purposely not implemented
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

#endif
