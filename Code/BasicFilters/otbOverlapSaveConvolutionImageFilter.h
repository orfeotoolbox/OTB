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
#ifndef __otbOverlapSaveConvolutionImageFilter_h
#define __otbOverlapSaveConvolutionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkFFTWCommon.h"


/*NB: This program gives a texture image by using gabor filter. Suddently, this program implements a block convolution procedure in the aim to deal huge image. This procedure is called "overlap-save method".*/

namespace otb
{
template <class TInputImage,class TOutputImage> class ITK_EXPORT OverlapSaveConvolutionImageFilter : public itk::ImageToImageFilter< TInputImage, TOutputImage >
{


public:
	/** Extract dimension from input and output image. */
	itkStaticConstMacro(InputImageDimension, unsigned int,TInputImage::ImageDimension);
	itkStaticConstMacro(OutputImageDimension, unsigned int,TOutputImage::ImageDimension);

	/** Convenient typedefs for simplifying declarations. */
	typedef TInputImage InputImageType;
	typedef TOutputImage OutputImageType;

	/** Standard class typedefs. */
	typedef OverlapSaveConvolutionImageFilter Self;
  	typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self>  ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro(Self);

	/** Run-time type information (and related methods). */
	itkTypeMacro(OverlapSaveConvolutionImageFilter, ImageToImageFilter);
  
	/** Image typedef support. */
	typedef typename InputImageType::PixelType InputPixelType;
	typedef typename OutputImageType::PixelType OutputPixelType;
	typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;

	typedef typename InputImageType::RegionType InputImageRegionType;
	typedef typename OutputImageType::RegionType OutputImageRegionType;

	typedef typename InputImageType::SizeType InputSizeType;

	typedef typename itk::Array<InputRealType> ArrayType;
  
	/** Set the radius of the neighborhood used to compute the mean. */
	virtual void SetRadius (const InputSizeType rad) 
	{ 
		itkDebugMacro("setting radius to " << rad); 
		if (this->m_Radius != rad) 
		{ 
			this->m_Radius = rad;
			unsigned int arraySize=1;
			for (unsigned int i=0; i<m_Radius.GetSizeDimension(); i++)
			{
			arraySize *= 2*this->m_Radius[i]+1;
			}
		this->m_Filter.SetSize(arraySize);
		this->m_Filter.Fill(1);
		this->Modified(); 
		} 
	}

	/** Get the radius of the neighborhood used to compute the mean */
	itkGetConstReferenceMacro(Radius, InputSizeType);
  
	/** Set the input filter */
	void SetFilter(ArrayType filter)
	{
		if((filter.Size()!= m_Filter.Size()))
		{
			itkExceptionMacro("Error in SetFilter, invalid filter size:"<< filter.Size()<<" instead of 2*(m_Radius[0]+1)*(2*m_Radius[1]+1): "<<m_Filter.Size());
		}
		else
		{
			m_Filter = filter;
		}
      		this->Modified();
    	}	
	/** Get the filter */
	itkGetConstReferenceMacro(Filter,ArrayType);
 
	/** Set/Get methods for the normalization of the filter */
	itkSetMacro(NormalizeFilter, bool);
	itkGetMacro(NormalizeFilter, bool);
	itkBooleanMacro(NormalizeFilter);
  
	virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

#ifdef ITK_USE_CONCEPT_CHECKING
	/** Begin concept checking */
	itkConceptMacro(InputHasNumericTraitsCheck,(itk::Concept::HasNumericTraits<InputPixelType>));
	/** End concept checking */
#endif 

protected:
	OverlapSaveConvolutionImageFilter();
	virtual ~OverlapSaveConvolutionImageFilter() {}
	void PrintSelf(std::ostream& os, itk::Indent indent) const;
	void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,int threadId);
	/** Process a square piece */
	void ProcessRegion(const OutputImageRegionType& region,const OutputImageRegionType& outputRegionForThread);


private:
	OverlapSaveConvolutionImageFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	InputSizeType m_Radius;
	ArrayType m_Filter;
	bool m_NormalizeFilter; //Flag for normalisation

};
  
} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbOverlapSaveConvolutionImageFilter.txx"
#endif

#endif
