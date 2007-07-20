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

class ITK_EXPORT UtmMapProjection : public MapProjection<ossimUtmProjection> 
{
public :

  /** Standard class typedefs. */
  typedef UtmMapProjection                                  Self;
  typedef MapProjection<ossimUtmProjection>                 Superclass;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;

	typedef Superclass::ScalarType 														ScalarType;
	typedef itk::Point<ScalarType,2>   	      								InputPointType;
	typedef itk::Point<ScalarType,2>  	      								OutputPointType;     

	/** Method for creation through the object factory. */
	itkNewMacro( Self );

	/** Run-time type information (and related methods). */
	itkTypeMacro( UtmMapProjection, MapProjection );

	void SetZone(long zone);

	void SetZone(const InputPointType &ground); 

	void SetHemisphere(char hemisphere);

	long GetZone();

	const char GetHemisphere() const;
	
protected:

	UtmMapProjection(); 
	virtual ~UtmMapProjection();

	ossimUtmProjection* m_utmprojection;

private:
	
  UtmMapProjection(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
 	
};

} // namespace otb


#endif
