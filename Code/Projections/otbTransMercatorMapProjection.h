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
#ifndef __otbTransMercatorMapProjection_h
#define __otbTransMercatorMapProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimTransMercatorProjection.h"
#include "otbMapProjection.h"

namespace otb
{

class ITK_EXPORT TransMercatorMapProjection : public MapProjection<ossimTransMercatorProjection> 
{
public :

  /** Standard class typedefs. */
 	typedef TransMercatorMapProjection                            Self;
  typedef MapProjection<ossimTransMercatorProjection>           Superclass;
  typedef itk::SmartPointer<Self>                    	      	  Pointer;
  typedef itk::SmartPointer<const Self>              	      	  ConstPointer;

	typedef Superclass::ScalarType  															ScalarType;
	typedef itk::Point<ScalarType,2>   	      										InputPointType;
	typedef itk::Point<ScalarType,2>  	      										OutputPointType;     

	/** Method for creation through the object factory. */
	itkNewMacro( Self );

	/** Run-time type information (and related methods). */
	itkTypeMacro( TransMercatorMapProjection, MapProjection );
	
	virtual void SetEllipsoid();
    
	virtual void SetFalseEasting(double falseEasting);

	virtual void SetFalseNorthing(double falseNorthing);

	virtual void SetScaleFactor(double scaleFactor);

	virtual void SetParameters(double falseEasting,double falseNorthing, double scaleFactor);

	virtual void SetDefaults();

	virtual double GetFalseNorthing() const;
	
	virtual double GetFalseEasting() const;

	virtual double GetScaleFactor() const;

protected:

	TransMercatorMapProjection(); 
	virtual ~TransMercatorMapProjection();

	ossimTransMercatorProjection* m_TransMercatorProjection;
	
private :

  TransMercatorMapProjection(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
 	
};

} // namespace otb

#endif
