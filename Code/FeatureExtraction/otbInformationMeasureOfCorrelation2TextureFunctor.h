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
#ifndef __otbInformationMeasureOfCorrelation2TextureFunctor_h
#define __otbInformationMeasureOfCorrelation2TextureFunctor_h

#include "otbEntropyTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class InformationMeasureOfCorrelation2TextureFunctor
 *  \brief This functor calculates the inverse difference moment of an image
 *
 *   Computes joint histogram (neighborhood and offset neighborhood) 
 *   which bins are computing using Scott formula.
 *   Computes the probabiltiy p for each pair of pixel.
 *   InverseDifferenceMoment  is the sum 1/(1+(pi-poff)²)*p over the neighborhood.
 *   TIterInput is an ietrator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup 
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT InformationMeasureOfCorrelation2TextureFunctor : 
public EntropyTextureFunctor<TIterInput1, TIterInput2, TOutput>
{
public:
  InformationMeasureOfCorrelation2TextureFunctor(){};
  ~InformationMeasureOfCorrelation2TextureFunctor(){};

  typedef TIterInput1                           IterType1;
  typedef TIterInput2                           IterType2;
  typedef TOutput                               OutputType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;
  typedef EntropyTextureFunctor<TIterInput1, TIterInput2, TOutput> Superclass;

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    double HXY = Superclass::ComputeOverSingleChannel(neigh, neighOff);

    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    double out = 0.;
    // Stores marginal proba values
    std::vector<double> PxVector;
    std::vector<double> PyVector;
    
    // Computes HX
    for (unsigned r = 0; r<this->GetHisto()[0].size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto().size(); s++)
	  {
	    sumTemp += this->GetHisto()[s][r]*areaInv;
	  }
	PxVector.push_back( sumTemp );
      }

    // Computes HY
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    sumTemp += this->GetHisto()[r][s]*areaInv;
	  }
	PyVector.push_back( sumTemp );
      }
 
    // Computes HXY2
    double HXY2 = 0.;
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    double PxPy = PyVector[r]*PxVector[s];
	    if( PxPy != 0. )
	      {
		HXY2 += PxPy * vcl_log( PxPy );
	      }
	    
	  }
      }
    if ( HXY2 != 0. )
      HXY2 = -HXY2;
    
    out = vcl_sqrt( vcl_abs(1-vcl_exp( -2.*(HXY2-HXY))) );

    return out;  
  }
  
};
 
 
} // namespace Functor
} // namespace otb

#endif

