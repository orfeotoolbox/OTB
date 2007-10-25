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
  /** \class LambertConformalConicMapProjection
   * \brief This class implements the Lambert Conformatl Conic map projection.
   * It converts coordinates in longitude,latitude to Lambert Conformal Conic map coordinates.
   *
   *
   */
class ITK_EXPORT LambertConformalConicMapProjection : public MapProjection<ossimLambertConformalConicProjection> 
{
public :

  /** Standard class typedefs. */
  typedef LambertConformalConicMapProjection                        Self;
  typedef MapProjection<ossimLambertConformalConicProjection>       Superclass;
  typedef itk::SmartPointer<Self>                    	      		  	Pointer;
  typedef itk::SmartPointer<const Self>              	      		  	ConstPointer;

	typedef Superclass::ScalarType  ScalarType;
	typedef itk::Point<ScalarType,2 >   	InputPointType;
	typedef itk::Point<ScalarType,2 >  	OutputPointType;

	/** Method for creation through the object factory. */
	itkNewMacro( Self );

	/** Run-time type information (and related methods). */
	itkTypeMacro( LambertConformalConicMapProjection, MapProjection );
      
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

	ossimLambertConformalConicProjection* m_LambertConformalConicProjection;
	
private :

  LambertConformalConicMapProjection(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // namespace otb


#endif
