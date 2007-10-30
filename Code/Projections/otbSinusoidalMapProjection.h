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

  /** \class SinusoidalMapProjection
   *  \brief This class implements the Sinusoidal map projection.
   *  It converts coordinates in longitude,latitude to Sinusoidal map coordinates. 
   */
	template <InverseOrForwardTransformationEnum transform> 
  class ITK_EXPORT SinusoidalMapProjection : public	MapProjection<ossimSinusoidalProjection, transform> 
    {
      public :
	
	/** Standard class typedefs. */
	typedef SinusoidalMapProjection                      Self;
      typedef MapProjection<ossimSinusoidalProjection, transform>  Superclass;
      typedef itk::SmartPointer<Self>                        Pointer;
      typedef itk::SmartPointer<const Self>                  ConstPointer;
      
      typedef typename Superclass::ScalarType  			     ScalarType;
      typedef itk::Point<ScalarType,2>   	      	     InputPointType;
      typedef itk::Point<ScalarType,2>  	      	     OutputPointType;     

      /** Method for creation through the object factory. */
      itkNewMacro( Self );
      
      /** Run-time type information (and related methods). */
      itkTypeMacro( SinusoidalMapProjection, MapProjection );
      
      virtual void SetFalseEasting(double falseEasting);
      virtual void SetFalseNorthing(double falseNorthing);
      virtual double GetFalseNorthing() const;
      virtual double GetFalseEasting() const;
      virtual void SetParameters(double falseEasting,double falseNorthing);
      virtual void SetDefaults();
      
    protected:
      SinusoidalMapProjection(); 
      virtual ~SinusoidalMapProjection();
      ossimSinusoidalProjection* m_SinusoidalProjection;
      
      private :
	SinusoidalMapProjection(const Self&); //purposely not implemented
      void operator=(const Self&);            //purposely not implemented
      
    };

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSinusoidalMapProjection.txx"
#endif

} // namespace otb

#endif
