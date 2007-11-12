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
  /** \class UtmMapProjection
   *  \brief This class implements the UTM map projection.
   * It converts coordinates in longitude,latitude to UTM map coordinates.
   */
	template <InverseOrForwardTransformationEnum transform>
  class ITK_EXPORT UtmMapProjection : public MapProjection<ossimUtmProjection,transform> 
    {
      public :
	
			/** Standard class typedefs. */
			typedef UtmMapProjection                Self;
      typedef MapProjection<ossimUtmProjection,transform> Superclass;
      typedef itk::SmartPointer<Self>           Pointer;
      typedef itk::SmartPointer<const Self>     ConstPointer;
      
      typedef typename Superclass::ScalarType 		ScalarType;
      typedef itk::Point<ScalarType,2>   	InputPointType;
      typedef itk::Point<ScalarType,2>  	OutputPointType;     

      /** Method for creation through the object factory. */
      itkNewMacro( Self );
      
      /** Run-time type information (and related methods). */
      itkTypeMacro( UtmMapProjection, MapProjection );
      
      virtual void SetZone(long zone);
      virtual void SetZone(const InputPointType &ground); 
      virtual void SetHemisphere(char hemisphere);
      virtual long GetZone();
      virtual const char GetHemisphere() const;

			virtual void SetZoneAndHemisphereFromGeoPoint(const InputPointType &geoPoint);
//			virtual void SetZoneAndHemisphereFromCartoPoint(const OutputPointType &cartoPoint);
			virtual int GetZoneFromGeoPoint(const InputPointType& geoPoint);

//      virtual void Initialize(const InputPointType& middlePoint);
			
    protected:
      UtmMapProjection(); 
      virtual ~UtmMapProjection();
			
        
    private:
      UtmMapProjection(const Self&); //purposely not implemented
      void operator=(const Self&);   //purposely not implemented
     };

} // namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUtmMapProjection.txx"
#endif

#endif
