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
#ifndef __otbLeafOpticalProperties_h
#define __otbLeafOpticalProperties_h


#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"


namespace otb
{
/** \class LeafOpticalProperties
   * \brief this class implements the LeafOpticalProperties lib.
   *
   * \sa itk::DataObject
 */
 
class ITK_EXPORT LeafOpticalProperties : public itk::DataObject
{
   public:
      /** Standard class typedefs */
      typedef LeafOpticalProperties Self;
      typedef itk::DataObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      typedef std::pair<double,double> PairType;
      typedef std::vector<PairType> VectorPairType;
      
      /** Standard macros */
      itkTypeMacro(LeafOpticalProperties, DataObject);
      itkNewMacro(Self);


      /** Set/Get Reflectance*/
//       itkSetObjectMacro(Reflectance, VectorPairType);
//       itkGetObjectMacro(Reflectance, VectorPairType);
      VectorPairType & GetReflectance()
      {
         return m_Reflectance;
      }
      void SetReflectance(const VectorPairType & refl)
      {
         m_Reflectance=refl;
      }
      
      /** Set/Get Transmitance*/
//       itkSetMacro(Transmitance, VectorPairType);
//       itkGetMacro(Transmitance, VectorPairType);
      VectorPairType & GetTransmitance()
      {
         return m_Transmitance;
      }
      void SetTransmitance(const VectorPairType & trans)
      {
         m_Transmitance=trans;
      }


   protected:
      /** Constructor */
      LeafOpticalProperties();
      /** Destructor */
      virtual ~LeafOpticalProperties() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

      
   private:
      LeafOpticalProperties(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
      
      VectorPairType m_Reflectance;
      VectorPairType m_Transmitance;

};
}// end namespace otb

#endif
