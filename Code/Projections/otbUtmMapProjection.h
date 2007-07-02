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
#ifndef __otbUtmMapProjection_h
#define __otbUtmMapProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimUtmProjection.h"
#include "otbMapProjection.h"

namespace otb
{ 
//Cette classe hérite de otb::MapProjection
class ITK_EXPORT UtmMapProjection : public MapProjection<ossimUtmProjection> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef UtmMapProjection                                    Self;
  typedef MapProjection<ossimUtmProjection>                   Superclass;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;

itkTypeMacro( UtmMapProjection, MapProjection );
itkNewMacro( Self );
	  
typedef Superclass::ScalarType  ScalarType;

/*typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType; */     
typedef itk::Point<ScalarType,2>   	      InputPointType;
typedef itk::Point<ScalarType,2>  	      OutputPointType;     
        
/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/	    
void SetZone(long zone);

void SetZone(const InputPointType &ground); 

void SetHemisphere(char hemisphere);

long GetZone();

const char GetHemisphere() const;
protected:
UtmMapProjection(); 
virtual ~UtmMapProjection();

//Variables protégés:
ossimUtmProjection* m_utmprojection;
};

}//Fin header


#endif
