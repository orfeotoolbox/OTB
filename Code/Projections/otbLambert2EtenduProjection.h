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
#ifndef __otbLambert2EtenduProjection_h
#define __otbLambert2EtenduProjection_h


#include "projection/ossimMapProjection.h"
#include "projection/ossimLambertConformalConicProjection.h"
#include "otbMapProjection.h"

namespace otb
{
   /** \class Lambert2EtenduProjection
   * \brief This class implements the Lambert 2 Etendu map projection.
   * It converts coordinates in longitude,latitude (WGS84) to Lambert 2 Etendu map coordinates.
   *
    */
  template <InverseOrForwardTransformationEnum transform>
      class ITK_EXPORT Lambert2EtenduProjection : public LambertConformalConicMapProjection<transform> 
  {
    public :
      /** Standard class typedefs. */
      typedef Lambert2EtenduProjection                  Self;
      typedef LambertConformalConicMapProjection<transform>   Superclass;
      typedef itk::SmartPointer<Self>                    	    Pointer;
      typedef itk::SmartPointer<const Self>              	    ConstPointer;
      
      typedef typename Superclass::ScalarType                 ScalarType;
      typedef itk::Point<ScalarType,2 >   	                  InputPointType;
      typedef itk::Point<ScalarType,2 >              	        OutputPointType;
      
      /** Method for creation through the object factory. */
      itkNewMacro( Self );
      
      /** Run-time type information (and related methods). */
      itkTypeMacro( Lambert2EtenduProjection, MapProjection );

        
    protected:
      Lambert2EtenduProjection(); 
      virtual ~Lambert2EtenduProjection();
	
    private :
      Lambert2EtenduProjection(const Self&); //purposely not implemented
      void operator=(const Self&);                       //purposely not implemented
  };
  
} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLambert2EtenduProjection.txx"
#endif

#endif
