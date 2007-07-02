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
#ifndef __otbEckert4MapProjection_h
#define __otbEckert4MapProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimEckert4Projection.h"
#include "otbMapProjection.h"

namespace otb
{
//Cette classe hérite de otb::MapProjection
class ITK_EXPORT Eckert4MapProjection : public MapProjection<ossimEckert4Projection> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef Eckert4MapProjection                                    Self;
  typedef MapProjection<ossimEckert4Projection>                   Superclass;
  typedef itk::SmartPointer<Self>                    	      	  Pointer;
  typedef itk::SmartPointer<const Self>              	      	  ConstPointer;

itkTypeMacro( Eckert4MapProjection, MapProjection );
itkNewMacro( Self );
	  
  typedef Superclass::ScalarType ScalarType;
/*typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType; */ 
      
typedef itk::Point<ScalarType,2 >   	      InputPointType;
typedef itk::Point<ScalarType,2 >  	      OutputPointType; 
/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/	    
void SetFalseEasting(double falseEasting);

void SetFalseNorthing(double falseNorthing);

double GetFalseNorthing() const;

double GetFalseEasting() const;

void SetDefaults();

protected:
Eckert4MapProjection(); 
virtual ~Eckert4MapProjection();

//Variables protégés:
ossimEckert4Projection* m_Eckert4Projection;
};

}//Fin header

#endif
