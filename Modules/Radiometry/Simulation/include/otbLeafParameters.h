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
#ifndef __otbLeafParameters_h
#define __otbLeafParameters_h


#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"


namespace otb
{
/** \class LeafParameters
   * \brief Data class containing Leaf Parameters.
   *
   * \sa itk::DataObject
 */

class ITK_EXPORT LeafParameters : public itk::DataObject
{
   public:
      /** Standard class typedefs */
      typedef LeafParameters Self;
      typedef itk::DataObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      /** Standard macros */
      itkTypeMacro(LeafParameters, DataObject);
      itkNewMacro(Self);

      /** Set/Get Chlorophyll content (ug/cm2)*/
      itkSetMacro(Cab, double);
      itkGetMacro(Cab, double);

      /** Set/Get Carotenoid content (ug/cm2)*/
      itkSetMacro(Car, double);
      itkGetMacro(Car, double);

      /** Set/Get Brown pigment content (arbitrary unit)*/
      itkSetMacro(CBrown, double);
      itkGetMacro(CBrown, double);

      /** Set/Get equivalent leaf water thickness, EWT (cm)*/
      itkSetMacro(Cw, double);
      itkGetMacro(Cw, double);

      /** Set/Get leaf dry matter, LMA (g/cm2)*/
      itkSetMacro(Cm, double);
      itkGetMacro(Cm, double);

      /** Set/Get leaf structure coef*/
      itkSetMacro(N, double);
      itkGetMacro(N, double);

   protected:
      /** Constructor */
      LeafParameters();
      /** Destructor */
      ~LeafParameters();
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

   private:
      LeafParameters(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      double m_Cab; //Chlorophyll content
      double m_Car; //Carotenoid content
      double m_CBrown; //Brown pigment content
      double m_Cw; //EWT
      double m_Cm; //LMA
      double m_N; //structure coef

};
}// end namespace otb

#endif
