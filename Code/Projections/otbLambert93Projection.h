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
#ifndef __otbLambert93Projection_h
#define __otbLambert93Projection_h


#include "projection/ossimMapProjection.h"
#include "projection/ossimLambertConformalConicProjection.h"
#include "otbMapProjection.h"

namespace otb
{
   /** \class Lambert93Projection
   * \brief This class implements the Lambert93 map projection (RGF93 french geodesic system).
   * It converts coordinates in longitude,latitude (WGS84) to Lambert 93 map coordinates.
   *
    */
  template <InverseOrForwardTransformationEnum transform>
      class ITK_EXPORT Lambert93Projection : public LambertConformalConicMapProjection<transform> 
  {
    public :
      /** Standard class typedefs. */
      typedef Lambert93Projection                  Self;
      typedef LambertConformalConicMapProjection<transform>   Superclass;
      typedef itk::SmartPointer<Self>                    	    Pointer;
      typedef itk::SmartPointer<const Self>              	    ConstPointer;
      
      typedef typename Superclass::ScalarType                 ScalarType;
      typedef itk::Point<ScalarType,2 >   	                  InputPointType;
      typedef itk::Point<ScalarType,2 >              	        OutputPointType;
      
      /** Method for creation through the object factory. */
      itkNewMacro( Self );
      
      /** Run-time type information (and related methods). */
      itkTypeMacro( Lambert93Projection, MapProjection );

        
    protected:
      Lambert93Projection(); 
      virtual ~Lambert93Projection();
	
    private :
      Lambert93Projection(const Self&); //purposely not implemented
      void operator=(const Self&);                       //purposely not implemented
  };
  
} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLambert93Projection.txx"
#endif

#endif
