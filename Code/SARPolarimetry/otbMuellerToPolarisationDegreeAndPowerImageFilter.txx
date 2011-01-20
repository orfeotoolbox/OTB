/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $otbMuellerToPolarisationDegreeAndPowerImageFilter.txx$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Version: 1.0 $

=========================================================================*/
#ifndef __MuellerToPolarisationDegreeAndPowerImageFilter_cxx
#define __MuellerToPolarisationDegreeAndPowerImageFilter_cxx

#include "otbMuellerToPolarisationDegreeAndPowerImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace otb
{

  template <class TPixel>
  MuellerToPolarisationDegreeAndPowerImageFilter<TPixel>::MuellerToPolarisationDegreeAndPowerImageFilter()
  {
    // !! TInputImage should be MuellerType and TOutputImage should be CircularType (otherwise fail)
    
  }

  template <class TPixel>
  void 
  MuellerToPolarisationDegreeAndPowerImageFilter<TPixel>
  ::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
  {
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr = 
      const_cast< InputImageType * >( this->GetInput() );
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    
    if ( !inputPtr || !outputPtr )
      {
	return;
      }
    
    // get a copy of the input requested region (should equal the output
    // requested region)
    typename InputImageType::RegionType inputRequestedRegion;
    inputRequestedRegion = inputPtr->GetRequestedRegion();
    
    
    
    // crop the input requested region at the input's largest possible region
    if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
      {
	inputPtr->SetRequestedRegion( inputRequestedRegion );
	return;
      }
    else
      {
	// Couldn't crop the region (requested region is outside the largest
	// possible region).  Throw an exception.
	
	// store what we tried to request (prior to trying to crop)
	inputPtr->SetRequestedRegion( inputRequestedRegion );
	
	// build an exception
	itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
	e.SetLocation(ITK_LOCATION);
	e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
	e.SetDataObject(inputPtr);
	throw e;
      }
  }


  template< class TPixel>
  void
  MuellerToPolarisationDegreeAndPowerImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {
    
    
    
    float M1, M2, M3, M4, M5, M6, M7, M8, M9, M10;
    float Pmin, Pmax, deg_pol_min, deg_pol_max;
    float P, deg_pol;
    
    itk::ImageRegionConstIterator<InputImageType> inIt (this->GetInput(),outputRegionForThread);
    itk::ImageRegionIterator<OutputImageType> outIt (this->GetOutput(),outputRegionForThread);
    DegreeAndPowerType vectorValue;
    StokesVectorType Si;
    StokesVectorType Sr;
    
    
    int numPix =0;
    
    for ( inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt)
      {
	
	//Initialisation des paramètres
	Pmin=100000.;
	Pmax=-100000.;
	deg_pol_min=100000.;
	deg_pol_max=-10000.;
  
	P=0.;
	deg_pol=0.;

	//Récupération de la matrice de Mueller en entrée
	M1 = inIt.Get()[0];
	M2 = inIt.Get()[1];
	M3 = inIt.Get()[2];
	M4 = inIt.Get()[3];
	M5 = inIt.Get()[4];
	M6 = inIt.Get()[5];
	M7 = inIt.Get()[6];
	M8 = inIt.Get()[7];
	M9 = inIt.Get()[8];
	M10= inIt.Get()[9];
	

	int tau = -45;
	int psi = -90;
	while (tau <46)
	  while (psi<91)
	    { 
	      //std::cout << tau << "  "<<psi << std::endl;
	      // Définition du vecteur de Stokes incident
	      Si[0]=1;
	      Si[1]=cos(2*psi*PI/180.)*cos(2*tau*PI/180.);
	      Si[2]=sin(2*psi*PI/180.)*cos(2*tau*PI/180.);
              Si[3]=sin(2*tau*PI/180.);
	      
	      //Définition du vecteur de Stokes réfléchi Sr=M*Si
	      Sr[0]=M1*Si[0]+M5*Si[1]+M6*Si[2]+M7*Si[3];
              Sr[1]=M5*Si[0]+M2*Si[1]+M8*Si[2]+M9*Si[3];
              Sr[2]=M6*Si[0]+M8*Si[1]+M3*Si[2]+M10*Si[3];
              Sr[3]=M7*Si[0]+M9*Si[1]+M10*Si[2]+M4*Si[3];

	      //std::cout << Sr[0] << " "<< Sr[1] << " "<<Sr[2]<< " "<<Sr[3] << std::endl;

	      //Calcul des paramètres P et deg_pol
	      P=Sr[0];
	      
	      if (P<0.00001)
		deg_pol=0.; //Par défaut, à laisser ??
	      else
		deg_pol=sqrt(Sr[1]*Sr[1]+Sr[2]*Sr[2]+Sr[3]*Sr[3])/Sr[0];

	      if (P > Pmax) 
		Pmax = P;
	      if (P < Pmin) 
		Pmin = P;
              if (deg_pol > deg_pol_max) {deg_pol_max = deg_pol;}
	      if (deg_pol < deg_pol_min) {deg_pol_min = deg_pol;}
	      
	      tau=tau+5; // parcours 5° par 5° pour diminuer temps de calcul
	      psi= psi+5;
	    } // end for psi
	

   
	
	vectorValue[0]= Pmin;   
	vectorValue[1]= Pmax;   
	vectorValue[2]= deg_pol_min;
	vectorValue[3]= deg_pol_max;
	

	//std::cout << "------"<<Pmin << "  "<< Pmax << "  "<< deg_pol_min << "  "<< deg_pol_max << std::endl;
	outIt.Set(vectorValue);
	
    
	numPix++;
      }
  }
  

  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  MuellerToPolarisationDegreeAndPowerImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    
  }
  
} // end namespace otb

#endif
