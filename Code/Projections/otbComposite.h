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
#ifndef __otbComposite_h
#define __otbComposite_h

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "itkTransform.h"
#include "itkMacro.h"
#include "otbMapProjection.h"

namespace otb
{

//On définit le template avec un type générique.
const unsigned int NInputDimensions = 2;
const unsigned int NOutputDimensions = 2;
typedef double TScalarType;

/**Template otbComposite.txx
* Cette classe permet de passer d'une MapProjection à une autre en passant par un point géographique. 
* Ceci pour éviter TOUTES les combinaisons possibles de passage d'une proj. carto à une autre.
* Template: Pour vérifier les types.
**/
template <
class TInputMapProjection, 
class TOutputMapProjection,
class TScalarType=double, 
unsigned int NInputDimensions=2,
unsigned int NOutputDimensions=2>
class ITK_EXPORT Composite: public itk::Transform<
TScalarType,         // Data type for scalars 
NInputDimensions,  // Number of dimensions in the input space
NOutputDimensions> // Number of dimensions in the output space
{
public :
  
//Déclaration des types utilisés:

  typedef itk::Transform< TScalarType,
                   NInputDimensions,
                   NOutputDimensions >                        Superclass;
  typedef Composite                                	          Self;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;
  typedef TInputMapProjection                                 InputMapProjectionType;
  typedef TOutputMapProjection                                OutputMapProjectionType;
  
itkTypeMacro( Composite, itk::Transform );
itkNewMacro( Self );

   itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
   itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
   itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
   itkStaticConstMacro(ParametersDimension, unsigned int,NInputDimensions*(NInputDimensions+1));

  typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
  typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType;        
	                                      
//Déclaration des méthodes:
itkSetObjectMacro(InputMapProjection,InputMapProjectionType); // SetMacro pour la 1è MapProjection

itkSetObjectMacro(OutputMapProjection,OutputMapProjectionType); // SetMacro pour la 2è MapProjection

// itkGetObjectMacro(InputMapProjection,InputMapProjectionType);
//  
// itkGetObjectMacro(OutputMapProjection,OutputMapProjectionType);

OutputPointType ComputeProjection1ToProjection2(const InputPointType &point1);

InputPointType ComputeProjection2ToProjection1(const OutputPointType &point2);

protected:
Composite();
~Composite();

TInputMapProjection* m_InputMapProjection;
TOutputMapProjection* m_OutputMapProjection;  
};
}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComposite.txx"
#endif

#endif
