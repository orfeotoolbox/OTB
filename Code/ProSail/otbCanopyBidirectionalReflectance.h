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
#ifndef __otbCanopyBidirectionalReflectance_h
#define __otbCanopyBidirectionalReflectance_h


#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"


namespace otb
{
/** \class CanopyBidirectionalReflectance
   * \brief this class implements the CanopyBidirectionalReflectance lib.
   *
   * \sa itk::DataObject
 */
 
class ITK_EXPORT CanopyBidirectionalReflectance : public itk::DataObject
{
   public:
      /** Standard class typedefs */
      typedef CanopyBidirectionalReflectance Self;
      typedef itk::DataObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      typedef std::pair<double,double> PairType;
      typedef std::vector<PairType> VectorPairType;
      
      
      /** Standard macros */
      itkTypeMacro(CanopyBidirectionalReflectance, DataObject);
      itkNewMacro(Self);
      
      /** Set/Get Resh*/
      VectorPairType & GetResh()
      {
         return m_Resh;
      }
      void SetResh(const VectorPairType & resh)
      {
         std::cout<<"set resh"<<std::endl;
         m_Resh=resh;
         std::cout<<"setted"<<std::endl;
      }
      
      /** Set/Get Resv*/
      VectorPairType & GetResv()
      {
         return m_Resv;
      }
      void SetResv(const VectorPairType & resv)
      {
         m_Resv=resv;
      }
      
    


   protected:
      /** Constructor */
      CanopyBidirectionalReflectance();
      /** Destructor */
      virtual ~CanopyBidirectionalReflectance() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

      
   private:
      CanopyBidirectionalReflectance(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      VectorPairType m_Resh;
      VectorPairType m_Resv;

};
}// end namespace otb

#endif
