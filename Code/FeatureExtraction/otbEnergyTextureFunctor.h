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
#ifndef __otbEnergyTextureFunctor_h
#define __ootbEnergyTextureFunctor_h

#include "otbMath.h"

namespace otb
{
namespace Functor
  {
    /** \class EnergyTextureFunctor
     *  \brief This functor calculates the local energy of an image
     *
     *   Computes the sqaure gradient which is computed using offset and 
     *   angle pixel of the neighborhood.
     *   TIterInput is an ietrator, TOutput is a PixelType.
     *
     *  \ingroup Functor
     *  \ingroup Statistics
     */
template <class TIterInput1, class TIterInput2, class TOutput>
class EnergyTextureFunctor
 {
 public:
   EnergyTextureFunctor(){ m_Offset.Fill(1); };
   ~EnergyTextureFunctor(){};

   typedef TIterInput1                    IterType1;
   typedef TIterInput2                    IterType2;
   typedef TOutput                        OutputType;
   typedef typename IterType1::OffsetType OffsetType;
   typedef typename IterType1::RadiusType RadiusType;
   typedef typename OutputType::ValueType OutputPixelType;
   
   void SetOffset(OffsetType off){ m_Offset=off; };
    OffsetType GetOffset(){ return m_Offset; };

   inline TOutput operator()(const TIterInput1 &it, const TIterInput2 &itOff)
     {
       RadiusType radius = it.GetRadius();
       unsigned int nbComp = it.GetCenterPixel().GetSize();
       double area = static_cast<double>(radius[0]*radius[1]);
       OutputType outPix;
       outPix.SetSize( nbComp );
       outPix.Fill(0);

       
       OffsetType offset;
       offset.Fill(0);
       OffsetType offsetOff;
       OffsetType offsetOffInit;  
 
       offsetOffInit[0] = -radius[0]+m_Offset[0]-1;
       offsetOffInit[1] = -radius[1]+m_Offset[1]-1;
      
       //std::cout<<offsetOffInit<<std::endl;
 
       double temp = 0.;
       int ll = 0;
       //double dist = 0.;
       double norm = 0.;
       //std::cout<<"START : "<<offsetOff<<std::endl;
       for( unsigned int i=0; i<nbComp; i++ )
	 {
	   //std::cout<<"NEEEEEEEEEEEEW :"<<std::endl;
	   offsetOff = offsetOffInit;
	   temp = 0.;
	   for( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
	     {
	       offsetOff[0]++;
	       offsetOff[1] = offsetOffInit[1];
	      
	       offset[0] = l;
	       ll = l*l;
	       for( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
		 {
		   offsetOff[1]++;
		   offset[1] = k;
		   //std::cout<<"k="<<k<<" : "<<offset<<" -> "<<offsetOff<<std::endl;
		   //dist = 1.;
	
		   norm = vcl_pow(static_cast<double>(itOff.GetPixel(offsetOff)[i]-itOff.GetCenterPixel()[i]), 2);

		   temp += norm;
		 }
	       temp /= area;
	       outPix[i] = static_cast<OutputPixelType>( vcl_pow(temp, 2) );
	     }
	 }
       
     
       return outPix;
     }

 private:
   OffsetType m_Offset;;

 };
 

  } // namespace Functor
} // namespace otb

#endif

