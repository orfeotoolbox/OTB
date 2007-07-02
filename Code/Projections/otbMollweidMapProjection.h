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
#ifndef __otbMollweidMapProjection_h
#define __otbMollweidMapProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimMollweidProjection.h"
#include "otbMapProjection.h"

namespace otb
{
//Cette classe hérite de otb::MapProjection
class ITK_EXPORT MollweidMapProjection : public MapProjection<ossimMollweidProjection> 
{
public :

/******************************************/
/*  Déclaration des types utilisés:       */
/******************************************/
  typedef MollweidMapProjection                                   Self;
  typedef MapProjection<ossimMollweidProjection>                  Superclass;
  typedef itk::SmartPointer<Self>                    	      	  Pointer;
  typedef itk::SmartPointer<const Self>              	      	  ConstPointer;

itkTypeMacro( MollweidMapProjection, MapProjection );
itkNewMacro( Self );
	  
/*typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType; */ 

typedef Superclass::ScalarType  ScalarType;

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
MollweidMapProjection(); 
virtual ~MollweidMapProjection();

//Variables protégés:
ossimMollweidProjection* m_MollweidProjection;
};

}//Fin header

/*#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMollweidMapProjection.cxx"
#endif*/

#endif
