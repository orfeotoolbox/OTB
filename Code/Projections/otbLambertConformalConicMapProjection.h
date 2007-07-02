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
#ifndef __otbLambertConformalConicMapProjection_h
#define __otbLambertConformalConicMapProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimLambertConformalConicProjection.h"
#include "otbMapProjection.h"

namespace otb
{
//Cette classe hérite de otb::MapProjection
class ITK_EXPORT LambertConformalConicMapProjection : public MapProjection<ossimLambertConformalConicProjection> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef LambertConformalConicMapProjection                            Self;
  typedef MapProjection<ossimLambertConformalConicProjection>      Superclass;
  typedef itk::SmartPointer<Self>                    	      		  	Pointer;
  typedef itk::SmartPointer<const Self>              	      		  	ConstPointer;

itkTypeMacro( LambertConformalConicMapProjection, MapProjection );
itkNewMacro( Self );
	  
typedef Superclass::ScalarType  ScalarType;

// typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
// typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType;  
typedef itk::Point<ScalarType,2 >   	      InputPointType;
typedef itk::Point<ScalarType,2 >  	      OutputPointType;      
/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/	    
void SetStandardParallel1 (double degree);

void SetStandardParallel2 (double degree);

void SetStandardParallels (double parallel1Degree,double parallel2Degree);

void SetFalseEasting(double falseEasting);

void SetFalseNorthing(double falseNorthing);

double GetFalseNorthing() const;

double GetFalseEasting() const;

void SetParameters(double parallel1Degree,double parallel2Degree,double falseEasting,double falseNorthing);

void SetDefaults();

protected:
LambertConformalConicMapProjection(); 
virtual ~LambertConformalConicMapProjection();

//Variables protégés:
ossimLambertConformalConicProjection* m_LambertConformalConicProjection;
};

}//Fin header

/*#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLambertConformalConicMapProjection.cxx"
#endif*/

#endif
