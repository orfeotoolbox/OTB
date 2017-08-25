
#ifndef otbCoeffGuidedBoxImageFilter_txx
#define otbCoeffGuidedBoxImageFilter_txx


#include "otbCoeffGuidedBoxImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConstNeighborhoodIterator.h"


#include "itkImageToImageFilter.h" 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include "itkMatrix.h"
#include "vnl/vnl_matrix.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

namespace otb
{	
	
template <class TInputImage,  class TOutputImage >
CoeffGuidedBoxImageFilter<TInputImage,TOutputImage>
::CoeffGuidedBoxImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(4);


  // Set the outputs
  this->SetNumberOfRequiredOutputs(0);
  this->SetNthOutput(0,TOutputImage::New());
  // this->SetNumberOfThreads(1);

  // Default parameters
  m_Epsilon = 0.00001*255*255;

}

template <class TInputImage,  class TOutputImage >
CoeffGuidedBoxImageFilter<TInputImage,TOutputImage>
::~CoeffGuidedBoxImageFilter()
{}
// ========================================================== Seters ==================================

template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::SetCostVolumeImageInput(const TInputImage * image)
{
 
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::SetMeanCostVolumeImageInput(const TInputImage * image)
{

  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::SetMeanImageInput(const TInputImage * image)
{

  this->SetNthInput(2, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::SetCovarianceImageInput(const TInputImage * image)
{

  this->SetNthInput(3, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::SetLeftImageInput(const TInputImage * image)
{

  this->SetNthInput(4, const_cast<TInputImage *>( image ));
}

template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::SetCovIm1CostImageInput(const TInputImage * image)
{

  this->SetNthInput(5, const_cast<TInputImage *>( image ));
}
//========================================================== Geters ======================================
template <class TInputImage,  class TOutputImage >
const TInputImage *
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GetCostVolumeImageInput() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage,  class TOutputImage >
const TInputImage *
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GetMeanCostVolumeImageInput() const
{
  if (this->GetNumberOfInputs()<2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage,  class TOutputImage >
const TInputImage *
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GetMeanImageInput() const
{
  if(this->GetNumberOfInputs()<3)
    {
    return ITK_NULLPTR;
    }

  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TInputImage,  class TOutputImage >
const TInputImage *
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GetCovarianceImageInput() const
{
  if(this->GetNumberOfInputs()<4)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TInputImage,  class TOutputImage >
const TInputImage *
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GetLeftImageInput() const
{
  if(this->GetNumberOfInputs()<5)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TInputImage,  class TOutputImage >
TOutputImage *
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GetOutputImage()
{
  if (this->GetNumberOfOutputs()<1)
    {
    return ITK_NULLPTR;
    }
  return static_cast< TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputImage,  class TOutputImage >
const TInputImage *
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GetCovIm1CostImageInput() const
{
  if(this->GetNumberOfInputs()<6)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(5));
}

//================================================= GenerateOutputInformation  ===================================================
template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage,  TOutputImage >
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();
  
  
  this->GetOutput()->SetNumberOfComponentsPerPixel(4);
 
}

//================================================ ThreadedGenerateData ====================================
template <class TInputImage,  class TOutputImage >
void
CoeffGuidedBoxImageFilter<TInputImage, TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{	

//Iterator 

ConstNeighborhoodIteratorType InputImageIt( this->GetRadius(), this->GetLeftImageInput(), outputRegionForThread   );
InputImageIt.GoToBegin(); 

ConstNeighborhoodIteratorType MeanInputIt( this->GetRadius(), this->GetMeanImageInput(),outputRegionForThread);  
MeanInputIt.GoToBegin(); 

ConstNeighborhoodIteratorType CostVolumeImageIt( this->GetRadius(), this->GetCostVolumeImageInput(),outputRegionForThread );      
CostVolumeImageIt.GoToBegin();

ConstNeighborhoodIteratorType MeanCostVolumeImageIt( this->GetRadius(), this->GetMeanCostVolumeImageInput(),outputRegionForThread);  
MeanCostVolumeImageIt.GoToBegin();

ConstNeighborhoodIteratorType CovarianceInputIt( this->GetRadius(), this->GetCovarianceImageInput(),outputRegionForThread);  
CovarianceInputIt.GoToBegin();

ConstNeighborhoodIteratorType CovIm1CostIt( this->GetRadius(), this->GetCovIm1CostImageInput(),outputRegionForThread);  
CovIm1CostIt.GoToBegin();

itk::ImageRegionIterator<TOutputImage> outputIt ( this->GetOutputImage(), outputRegionForThread );
outputIt.GoToBegin(); 

 
// Guided filter computation    
  
itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
      
typename TOutputImage::PixelType OutPixel(4);
OutPixel.Fill(0);
typename TOutputImage::PixelType a(3);
a.Fill(0);
typename TOutputImage::PixelType b(1);
b.Fill(0);


std::vector<double> Matrice_cov;
Matrice_cov.resize(9,0);

std::vector<double> Matrice_covInv;
Matrice_covInv.resize(9,0);



while ( !outputIt.IsAtEnd() && !InputImageIt.IsAtEnd() && !MeanInputIt.IsAtEnd() && !CovarianceInputIt.IsAtEnd()){
		
				
						    Matrice_cov[0] = CovarianceInputIt.GetCenterPixel()[0] + m_Epsilon;
							Matrice_cov[4] = CovarianceInputIt.GetCenterPixel()[4] + m_Epsilon;
							Matrice_cov[8] = CovarianceInputIt.GetCenterPixel()[8] + m_Epsilon;
							Matrice_cov[1] = CovarianceInputIt.GetCenterPixel()[1];
							Matrice_cov[2] = CovarianceInputIt.GetCenterPixel()[2];
							Matrice_cov[3] = CovarianceInputIt.GetCenterPixel()[3];
							Matrice_cov[5] = CovarianceInputIt.GetCenterPixel()[5];
							Matrice_cov[6] = CovarianceInputIt.GetCenterPixel()[6];
							Matrice_cov[7] = CovarianceInputIt.GetCenterPixel()[7];		
					
							
//=================================  ineversion of the matrix

							Matrice_covInv[0] = Matrice_cov[0]* Matrice_cov[8]- Matrice_cov[5]*Matrice_cov[7];
													
							Matrice_covInv[1] = Matrice_cov[2]*Matrice_cov[7] - Matrice_cov[1]*Matrice_cov[8];
													
							Matrice_covInv[2] = Matrice_cov[1]*Matrice_cov[5]- Matrice_cov[2]*Matrice_cov[4];
							
							
							float det = Matrice_cov[0]*Matrice_covInv[0]
										+Matrice_cov[3]*Matrice_covInv[1]
										+Matrice_cov[6]*Matrice_covInv[2];
										
							det = 1/det;
							Matrice_covInv[0] *= det;
							Matrice_covInv[1] *= det;
							Matrice_covInv[2] *= det;
							
							Matrice_covInv[3] = Matrice_covInv[1];
							Matrice_covInv[4] = (Matrice_cov[0] *Matrice_cov[8]  
												- Matrice_cov[2]*Matrice_cov[6]) * det;
												
							Matrice_covInv[5] = (Matrice_cov[2]*Matrice_cov[3]
													- Matrice_cov[0]*Matrice_cov[5]) * det;
													
							Matrice_covInv[6]=  Matrice_covInv[2];
							Matrice_covInv[7] = Matrice_covInv[5];
							Matrice_covInv[8] = (Matrice_cov[0] *Matrice_cov[4]	- Matrice_cov[1]*Matrice_cov[3]) * det;
    
//===== 
    
// equation (19) Compute ak


OutPixel[0] = CovIm1CostIt.GetCenterPixel()[0]*Matrice_covInv[0] + CovIm1CostIt.GetCenterPixel()[1]* Matrice_covInv[1] + CovIm1CostIt.GetCenterPixel()[2]* Matrice_covInv[2];
OutPixel[1] = CovIm1CostIt.GetCenterPixel()[0]*Matrice_covInv[3] + CovIm1CostIt.GetCenterPixel()[1]* Matrice_covInv[4] + CovIm1CostIt.GetCenterPixel()[2]* Matrice_covInv[5];												 
OutPixel[2] = CovIm1CostIt.GetCenterPixel()[0]*Matrice_covInv[6] + CovIm1CostIt.GetCenterPixel()[1]* Matrice_covInv[7] + CovIm1CostIt.GetCenterPixel()[2]* Matrice_covInv[8];

// Compute b
    OutPixel[3] = (MeanCostVolumeImageIt.GetCenterPixel()[0]
				   -OutPixel[0]*MeanInputIt.GetCenterPixel()[0]
				   -OutPixel[1]*MeanInputIt.GetCenterPixel()[1]
				   -OutPixel[2]*MeanInputIt.GetCenterPixel()[2]); 
				    // .boxFilter(r); will be implemented in next filtre   
					// b += aR.boxFilter(r)*im1R+aG.boxFilter(r)*im1G+aB.boxFilter(r)*im1B;												 


		outputIt.Set( OutPixel);
		
 
        ++InputImageIt;
        ++MeanInputIt;
        ++CostVolumeImageIt;
        ++MeanCostVolumeImageIt;
        
        ++CovarianceInputIt;
        ++CovIm1CostIt;
        ++outputIt;
        
        progress.CompletedPixel();
      }  // end of while 
        
} //end of threaded generate data





} // End namespace otb

#endif
