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
#ifndef __otbSinusoidalMapProjection_h
#define __otbSinusoidalMapProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimSinusoidalProjection.h"
#include "otbMapProjection.h"

namespace otb
{
//Cette classe hérite de otb::MapProjection
class ITK_EXPORT SinusoidalMapProjection : public otb::MapProjection<ossimSinusoidalProjection> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef SinusoidalMapProjection                                    Self;
  typedef otb::MapProjection<ossimSinusoidalProjection>              Superclass;
  typedef itk::SmartPointer<Self>                    	      	  Pointer;
  typedef itk::SmartPointer<const Self>              	      	  ConstPointer;

itkTypeMacro( SinusoidalMapProjection, MapProjection );
itkNewMacro( Self );
	  
typedef Superclass::ScalarType  ScalarType;
/*typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType; */   
typedef itk::Point<ScalarType,2>   	      InputPointType;
typedef itk::Point<ScalarType,2>  	      OutputPointType;     
/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/	    
void SetFalseEasting(double falseEasting);

void SetFalseNorthing(double falseNorthing);

double GetFalseNorthing() const;

double GetFalseEasting() const;

void SetDefaults();

protected:
SinusoidalMapProjection(); 
virtual ~SinusoidalMapProjection();

//Variables protégés:
ossimSinusoidalProjection* m_SinusoidalProjection;
};

}//Fin header

#endif
