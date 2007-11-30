 /*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  
  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_h
#define __otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_h

#include "itkNumericTraits.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include <vector>
#include "itkConstNeighborhoodIterator.h"
#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include "itkVariableSizeMatrix.h"


namespace otb
{
 namespace Functor
    {
       /** \class ComputeNeighborhoodContributionFunctor
       *  \brief Unary neighborhood functor to compute the value of a pixel which is a sum 
       *   of the surrounding pixels value ponderated by a coefficient.
       *  
       *  \ingroup Functor
       */
      template <class TNeighIter, class TOutput>
	class ComputeNeighborhoodContributionFunctor
	{
	public:
	  ComputeNeighborhoodContributionFunctor() {};
	  ~ComputeNeighborhoodContributionFunctor() {};
	  
	  typedef itk::VariableSizeMatrix<double>               PonderationMatrixType;
	  typedef typename std::vector<PonderationMatrixType>   PonderationValuesContainerType;
	  typedef typename TOutput::RealValueType               RealValueType;

	  void SetPonderationValues(const PonderationValuesContainerType & cont){ m_PonderationValues = cont; };
	  void SetUpwardTransmissionRatio(double upwardTransmissionRatio){ m_UpwardTransmissionRatio = upwardTransmissionRatio;};
	  void SetDiffusRatio(double diffusRatio){ m_DiffusRatio = diffusRatio;};
	  PonderationValuesContainerType GetPonderationValues(){ return m_PonderationValues;};
	  double GetUpwardTransmissionRatio(){ return m_UpwardTransmissionRatio;};
	  double GetDiffusRatio(){ return m_DiffusRatio;};

	  inline TOutput operator()(const TNeighIter & it)
	    {	 
	      unsigned int neighborhoodSize = it.Size();
	      double contribution = 0.;
	      double temp = 0.;
	      TOutput outPixel = it.GetPixel(0);

	      // Loop over each component
	      for (unsigned int j=0; j<outPixel.GetSize(); j++)
		{
		  contribution = 0;
		  // Load the current channel ponderation value matrix
		  PonderationMatrixType TempChannelPonderation = m_PonderationValues[j];
		  // Loop over the neighborhood
		  for (unsigned int i = 0; i < neighborhoodSize; ++i)
		    { 
		      // Current neighborhood pixel index calculation
		      unsigned int RowIdx = 0;
		      unsigned int ColIdx = 0;
		      RowIdx = i/TempChannelPonderation.Cols();
		      if(RowIdx != 0)
			{
			  ColIdx = (i+1)-RowIdx*TempChannelPonderation.Cols()-1;
			}
		      else
			{
			  ColIdx = i;
			}
		      // Extract the current neighborhood pixel ponderation
		      double idVal = TempChannelPonderation(RowIdx, ColIdx);
		      // Extract the current neighborhood pixel value
		      TOutput tempPix = it.GetPixel(i);
		      
		      contribution += static_cast<double>( tempPix[j] )*idVal;

		    }
		  temp = 0.; 
		  temp = static_cast<double>(it.GetCenterPixel()[j])*m_UpwardTransmissionRatio + contribution*m_DiffusRatio; 
		  outPixel[j] = static_cast<RealValueType>(temp);
		}
	      return outPixel;
	    }

	private:
	  PonderationValuesContainerType m_PonderationValues;
	  double m_UpwardTransmissionRatio;
	  double m_DiffusRatio;
	};
  
    }

  /** \class SurfaceAdjencyEffect6SCorrectionSchemeFilter
   *  \brief Correct the scheme taking care of the surrounding pixels. 
   *   
   *   The SurfaceAdjencyEffect6SCorrectionSchemeFilter class allows to introduce a neighbor correction to the 
   *   reflectance estimation. The satelite signal is considered as to be a combinaison of the signal coming from 
   *   the target pixel and a weighting of the siganls coming from the neighbor pixels.
   *
   */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT SurfaceAdjencyEffect6SCorrectionSchemeFilter : 
 public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage,
                                                   TOutputImage,
                                                   ITK_TYPENAME Functor::ComputeNeighborhoodContributionFunctor< itk::ConstNeighborhoodIterator<TInputImage>, 
                                                                                                                 ITK_TYPENAME TOutputImage::PixelType >       >
{
public:
  /** "typedef" to simplify the variables definition and the declaration. */
  typedef Functor::ComputeNeighborhoodContributionFunctor<itk::ConstNeighborhoodIterator<TInputImage>, 
                                                                   ITK_TYPENAME TOutputImage::PixelType> FunctorType;

  /** "typedef" for standard classes. */
  typedef SurfaceAdjencyEffect6SCorrectionSchemeFilter                                           Self;
  typedef UnaryImageFunctorWithVectorImageFilter< TInputImage, TOutputImage, FunctorType > Superclass;  
  typedef itk::SmartPointer<Self>                                                                Pointer;
  typedef itk::SmartPointer<const Self>                                                          ConstPointer;

  typedef typename Superclass::InputImageType         InputImageType;
  typedef typename Superclass::OutputImageType        OutputImageType;


  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(SurfaceAdjencyEffect6SCorrectionSchemeFilter, UnaryFunctorNeighborhoodVectorImageFilter);
  
/** 	Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType                           InputPixelType;
  typedef typename InputImageType::InternalPixelType                   InputInternalPixelType;
  typedef typename InputImageType::RegionType                          InputImageRegionType;
  typedef typename InputImageType::SizeType                            SizeType;
  typedef typename OutputImageType::PixelType                          OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                  OutputInternalPixelType;
  typedef typename OutputImageType::RegionType                         OutputImageRegionType;


  /** Storage ponderation values types*/
  typedef itk::VariableSizeMatrix<double>                               PonderationMatrixType;
  typedef typename std::vector<PonderationMatrixType>                   PonderationValuesContainerType;



  /** typedef for calculation*/
  typedef typename itk::ConstNeighborhoodIterator<InputImageType>          NeighborIterType;
 
   /** Set/Get the Size of the neighbor window. */
  void SetWindowRadius(unsigned int rad)
    {
      this->SetRadius(rad);
      m_WindowRadius = rad;
      this->Modified();
    }
  itkGetConstReferenceMacro(WindowRadius, unsigned int);
   /** Set/Get the ponderation values container. */
  void SetPonderationValues(const PonderationValuesContainerType & val)
    {
      m_PonderationValues = val;
      this->Modified();
    }
  PonderationValuesContainerType GetPonderationValues() { return m_PonderationValues; };


 /** Set/Get the upward transmission.*/
  itkSetMacro(UpwardTransmission, double);
  itkGetConstReferenceMacro(UpwardTransmission, double);
  /** Set/Get the upward direct transmission.*/
  itkSetMacro(UpwardDirectTransmission, double);
  itkGetConstReferenceMacro(UpwardDirectTransmission, double);
  /** Set/Get the upward diffus transmission. */
  itkSetMacro(UpwardDiffusTransmission, double);
  itkGetConstReferenceMacro(UpwardDiffusTransmission, double);



 protected:
  SurfaceAdjencyEffect6SCorrectionSchemeFilter();
  virtual ~SurfaceAdjencyEffect6SCorrectionSchemeFilter(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

 /** Initialize some accumulators before the threads run. */
  virtual void BeforeThreadedGenerateData ();

                        
private:

  /** Size of the window. */
  unsigned int m_WindowRadius;
  /** Ponderation values for the neighbor pixels.*/
  PonderationValuesContainerType m_PonderationValues;
  /** Upward Transmission.*/
  double m_UpwardTransmission;
  /** Upward Direct Transmission.*/
  double m_UpwardDirectTransmission;
  /** Diffus Transmission. */
  double m_UpwardDiffusTransmission;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.txx"
#endif

#endif
