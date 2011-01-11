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
#ifndef __otbCanopyParametersTo4SailCanopyBidirectionalReflectance_h
#define __otbCanopyParametersTo4SailCanopyBidirectionalReflectance_h


#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "otbCanopyParameters.h"
#include "otbCanopyBidirectionalReflectance.h"
#include "otbLeafOpticalProperties.h"
#include "dataSpec_P5B.h"

namespace otb
{
/** \class CanopyParametersTo4SailCanopyBidirectionalReflectance
   * \brief this class implements the CanopyParametersTo4SailCanopyBidirectionalReflectance lib.
   *
   * \sa itk::ProcessObject
 */

class ITK_EXPORT CanopyParametersTo4SailCanopyBidirectionalReflectance : public itk::ProcessObject
{
   public:
      /** Standard class typedefs */
      typedef CanopyParametersTo4SailCanopyBidirectionalReflectance Self;
      typedef itk::ProcessObject Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      typedef CanopyParameters        CanopyParametersType;
      typedef CanopyBidirectionalReflectance CanopyBidirectionalReflectanceType;
      typedef LeafOpticalProperties LeafOpticalPropertiesType;

      typedef CanopyBidirectionalReflectanceType::PairType PairType;
      typedef CanopyBidirectionalReflectanceType::VectorPairType VectorPairType;
      
      typedef std::vector<double> VectorType;
      
      /** Standard macros */
      itkNewMacro(Self);
      itkTypeMacro(CanopyParametersTo4SailCanopyBidirectionalReflectance, ProcessObject);
      
      
      /** Set Inputs */
      void SetCanopyParameters(const CanopyParametersType *object);
      CanopyParametersType * GetCanopyParameters();

      void SetLeafOpticalProperties(const LeafOpticalPropertiesType *object);
      LeafOpticalPropertiesType * GetLeafOpticalProperties();
      
      /** GenerateData */
      virtual void GenerateData();
      
      /** Get Output */
      virtual DataObjectPointer MakeOutput(unsigned int);
      
      virtual CanopyBidirectionalReflectanceType * GetOutput();
      
      
      
      /** Compute Leaf Angle Distribution */
      void Calc_LIDF(const double a, VectorType &lidf);
      void Campbell(const double ala, VectorType &freq);
      
      /** J functions */
      double Jfunc1(const double k, const double l, const double t);
      double Jfunc2(const double k, const double l, const double t);
      double Jfunc3(const double k, const double l, const double t);
      /** Volscatt */
      void Volscatt(const double tts, const double tto, const double psi, const double ttl, VectorType &result);


   protected:
      /** Constructor */
      CanopyParametersTo4SailCanopyBidirectionalReflectance();
      /** Destructor */
      virtual ~CanopyParametersTo4SailCanopyBidirectionalReflectance() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

   private:
      CanopyParametersTo4SailCanopyBidirectionalReflectance(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

};

}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbCanopyParametersTo4SailCanopyBidirectionalReflectance.cxx"
#endif

#endif
