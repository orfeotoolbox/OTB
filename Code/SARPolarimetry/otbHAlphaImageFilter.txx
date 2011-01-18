/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: otbHALphaImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/01/11 19:43:31 $
  Version:   $Revision: 1.14 $

=========================================================================*/
#ifndef __HAlphaImageFilter_txx
#define __HAlphaImageFilter_txx

#include "otbHAlphaImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace otb
{	
  static double abs(double x) { return x < 0.0 ? -x : x; }

  template <class TPixel>
  HAlphaImageFilter<TPixel>::HAlphaImageFilter()
  {
    m_Radius.Fill(1);
    m_Eigenvalues.Fill(0);
    //m_Eigenvectors.Fill(0);
    m_Entropie=0.;
    m_Alpha=0.;
    m_Anisotropie=0.;
  }

  template <class TPixel>
  void 
  HAlphaImageFilter<TPixel>
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

    // pad the input requested region by the operator radius
    // inputRequestedRegion.PadByRadius( m_Radius );

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
  HAlphaImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {
    unsigned int i;
    IndexType index;
    OutputVectorType vectorValue;  
    HermitianAnalysisType HermitianAnalysis;
    // Définition de la matrice T
    CoherencyMatrixType T;
    EigenvalueType eigenValues;
    EigenMatrixType eigenVectors;
    
    // Initialisation de eigenValues et eigenVectors
    for (int i =0;i<3;i++)
      {
	eigenValues=0.;
	for (int j=0; j<3; j++)
	  {
	    eigenVectors[i][2*j]=0.;
	    eigenVectors[i][2*j+1]=0.;
	  }
      }  
    m_Entropie=0.;
    

    std::cout << "entrée dans ThreadedGenerateData" << std::endl;
    

    itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;
    
    itk::ConstNeighborhoodIterator<InputImageType> bit;
    itk::ImageRegionIterator<OutputImageType> it;
  
    // Allocate output
    typename OutputImageType::Pointer     output = this->GetOutput();
    typename InputImageType::ConstPointer input  = this->GetInput();
  
    // Find the data-set boundary "faces"
    typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType 		faceList;
    typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator 	fit;
    
    itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
    faceList = bC(input, outputRegionForThread, m_Radius);


    // support progress methods/callbacks
    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  

    // Process each of the boundary faces.  These are N-d regions which border
    // the edge of the buffer.
    for (fit=faceList.begin(); fit != faceList.end(); ++fit)
      { 
	bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
	unsigned int neighborhoodSize = bit.Size();
	it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
	bit.OverrideBoundaryCondition(&nbc);
	bit.GoToBegin();

	while ( ! bit.IsAtEnd() )
	  {
	    T[0]= 0.;
	    T[1]= 0.;
	    T[2]= 0.;
	    T[3]= 0.;
	    T[4]= 0.;
	    T[5]= 0.;
	    T[6]= 0.;
	    T[7]= 0.;  
	    T[8]= 0.;
	    //Parcours du voisinage
	    for (int i = 0; i < neighborhoodSize; ++i)
	      {
		T[0]+=bit.GetPixel(i)[0];
		T[1]+=bit.GetPixel(i)[1];
		T[2]+=bit.GetPixel(i)[2];
		T[3]+=bit.GetPixel(i)[3];
		T[4]+=bit.GetPixel(i)[4]; 
		T[5]+=bit.GetPixel(i)[5];
		T[6]+=bit.GetPixel(i)[6];
		T[7]+=bit.GetPixel(i)[7];     
		T[8]+=bit.GetPixel(i)[8];
		
	      }
	    T[0]/= double(neighborhoodSize);
	    T[1]/= double(neighborhoodSize);
	    T[2]/= double(neighborhoodSize);
	    T[3]/= double(neighborhoodSize);
	    T[4]/= double(neighborhoodSize);
	    T[5]/= double(neighborhoodSize);
	    T[6]/= double(neighborhoodSize);
	    T[7]/= double(neighborhoodSize);  
	    T[8]/= double(neighborhoodSize);

	    HermitianAnalysis.ComputeEigenValuesAndVectors(T,eigenValues,eigenVectors);

      
	    m_Eigenvalues[0]= eigenValues[0];
	    m_Eigenvalues[1]= eigenValues[1];
	    m_Eigenvalues[2]= eigenValues[2];
	    
	    
	    m_Eigenvectors[0][0]= eigenVectors[0][0];   m_Eigenvectors[0][1]= eigenVectors[0][1];
	    m_Eigenvectors[1][0]= eigenVectors[0][2];   m_Eigenvectors[1][1]= eigenVectors[0][3];
	    m_Eigenvectors[2][0]= eigenVectors[0][4];   m_Eigenvectors[2][1]= eigenVectors[0][5];

	    // Calcul de l'entropie
	    double totalEigenValues=0.;
	    double p[3];

	    totalEigenValues = m_Eigenvalues[0]+m_Eigenvalues[1]+m_Eigenvalues[2];
	    if (totalEigenValues <0.00001)
	      totalEigenValues = 0.0001;
	    
	    
	    for (int k = 0; k < 3; k++) 
	      {
		if (m_Eigenvalues[k] <0.)
		  m_Eigenvalues[k] =0.;
		p[k] = m_Eigenvalues[k] / totalEigenValues;
	      }
	    if ( (p[0]<0.0001) || (p[1]<0.0001) || (p[2]<0.0001) )
	      m_Entropie =0.0;
	    else
	      {
		m_Entropie = p[0]*log(p[0])+p[1]*log(p[1])+p[2]*log(p[2]);
		m_Entropie=-1.*m_Entropie/log(3.);
		//std::cout << "Entropie "<< m_Eigenvalues[0]<< "  "<<m_Eigenvalues[1]<< " "<<m_Eigenvalues[2]<<" "<<m_Entropie << std::endl;
	      }
	
	    
	    //std::cout << T[0] << " "<<T[1] << " "<<T[2]<<" "<<T[3]<<" "<<T[4]<<" "<<T[5]<<" "<<T[6]<<" "<<T[7]<<" "<<T[8]<<std::endl;
	    //std::cout << it.GetIndex()[0] << "  "<<it.GetIndex()[1]<< std::endl;
	    //std::cout << "Entropie "<< m_Eigenvalues[0]<< "  "<<m_Eigenvalues[1]<< " "<<m_Eigenvalues[2]<<" "<<m_Entropie << std::endl;
	    
	    //std::cout << std::endl;
	    
	    // Calcul de alpha
	    double val0, val1, val2;
	    double a0, a1, a2;
	

	    for (int k = 0; k < 3; k++) 
	      {
		p[k] = m_Eigenvalues[k] / totalEigenValues;
		if (p[k] < 0.) p[k] = 0.;
		if (p[k] > 1.) p[k] = 1.;
	      }
	    
	    
	    val0=sqrt(m_Eigenvectors[0][0]*m_Eigenvectors[0][0]+m_Eigenvectors[0][1]*m_Eigenvectors[0][1]);
	    a0=acos(abs(val0))*180./PI;
	    
	    val1=sqrt(m_Eigenvectors[1][0]*m_Eigenvectors[1][0]+m_Eigenvectors[1][1]*m_Eigenvectors[1][1]);
	    a1=acos(abs(val1))*180./PI;
	
	    val2=sqrt(m_Eigenvectors[2][0]*m_Eigenvectors[2][0]+m_Eigenvectors[2][1]*m_Eigenvectors[2][1]);
	    a2=acos(abs(val2))*180./PI;
	    
	    m_Alpha=p[0]*a0 + p[1]*a1 + p[2]*a2;
	
	    if (m_Alpha>90) m_Alpha=0.;

	    // Anisotropie
	    m_Anisotropie=(m_Eigenvalues[1]-m_Eigenvalues[2])/(m_Eigenvalues[1]+m_Eigenvalues[2]+0.000001);
	
	    vectorValue[0]=m_Entropie;
	    vectorValue[1]=m_Alpha;
	    vectorValue[2]=m_Anisotropie;

	    //std::cout << m_Entropie << "   "<<m_Alpha<< "  "<<m_Anisotropie << std::endl;
	    //std::cout << std::endl;

	    it.Set(vectorValue);
	    
	    ++bit;
	    ++it;
	    progress.CompletedPixel();
	  }
      }
    
    std::cout << "Sortie de l'itérateur" << std::endl;
    
  }
    


  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  HAlphaImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    os << indent << "Radius: " << m_Radius << std::endl;

  }



}//end of namespace otb


#endif
