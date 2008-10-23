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
#ifndef __otbVectorImageToColorAnaglyphVectorImageFilter_h
#define __otbVectorImageToColorAnaglyphVectorImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
{
namespace Functor
  {
    /** \class ColorAnaglyphFunctor
     *  \brief This functor implements the pixel-wise color anaglyph composition.
     *  The output pixel is a vector pixel of size 3, where the first component contains the
     *  value of the first component from the second input pixel, the second component contains
     *  the value of the second component from the first input pixel, and the third component
     *  contains the value of the third component of the first input pixel.
     *
     *  the channels indices for the composition can be changed to fit the channel order from
     *  a specific sensor.
     */
    template <class TInputPixel1,class TInputPixel2,class TOutputPixel>
      class ColorAnaglyphFunctor
      {
      public:
	/// Constructor
	ColorAnaglyphFunctor()
	  {
	    m_RedChannelIndex = 0;
	    m_GreenChannelIndex = 1;
	    m_BlueChannelIndex = 2;	    
	  };
	/// Destructor 
	~ColorAnaglyphFunctor(){};

	inline TOutputPixel operator()(const TInputPixel1& pixel1, const TInputPixel2& pixel2)
	  {
	    TOutputPixel result(3);
	    result[0]=static_cast<typename TOutputPixel::ValueType>(pixel1[m_RedChannelIndex]);
	    result[1]=static_cast<typename TOutputPixel::ValueType>(pixel2[m_GreenChannelIndex]);
	    result[2]=static_cast<typename TOutputPixel::ValueType>(pixel2[m_BlueChannelIndex]);
	    return result;
	  }
	
	void SetRedChannelIndex(unsigned int index)
	  {
	    m_RedChannelIndex = index;
	  }
	void SetGreenChannelIndex(unsigned int index)
	  {
	    m_GreenChannelIndex = index;
	  }
	void SetBlueChannelIndex(unsigned int index)
	  {
	    m_BlueChannelIndex = index;
	  }
	unsigned int GetRedChannelIndex(void)
	  {
	    return m_RedChannelIndex;
	  }
	unsigned int GetGreenChannelIndex(void)
	  {
	    return m_GreenChannelIndex;
	  }
	unsigned int GetBlueChannelIndex(void)
	  {
	    return m_BlueChannelIndex;
	  }
      private:
	/// Color composition channels indices
	unsigned int m_RedChannelIndex;
	unsigned int m_GreenChannelIndex;
	unsigned int m_BlueChannelIndex;
      };
  }
 /** \class VectorImageToColorAnaglyphVectorImageFilter
  *  \brief This filter implements the synthesis of a grayscale anaglyph image from 
  *  a pair of stereoscopic images. 
  *  The output image is a VectorImage with 3 channels, where the first channel is the first
  *  channel of the second  input image, and the two last channels are the two last channel
  *  of the first input image.
  *
  *  the channels indices for the composition can be changed to fit the channel order from
  *  a specific sensor.
  *
  *  \note TOutputImage is supposed to be of type VectorImage.
  */
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT VectorImageToColorAnaglyphVectorImageFilter
  : public itk::BinaryFunctorImageFilter
  <  TInputImage1,TInputImage2,TOutputImage,
  Functor::ColorAnaglyphFunctor< 
  typename TInputImage1::PixelType,
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType
  > >
  {
 public:
  /** Standard typedefs */
  typedef VectorImageToColorAnaglyphVectorImageFilter  Self;

  typedef itk::BinaryFunctorImageFilter
  <  TInputImage1,TInputImage2,TOutputImage,
    Functor::ColorAnaglyphFunctor< 
    typename TInputImage1::PixelType,
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType
    >  > Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToColorAnaglyphVectorImageFilter, BinaryFunctorImageFilter);
  
  virtual void GenerateOutputInformation(void)
    {
      Superclass::GenerateOutputInformation();
      this->GetOutput()->SetNumberOfComponentsPerPixel(3);
    }

  itkSetMacro(RedChannelIndex,unsigned int);
  itkGetMacro(RedChannelIndex,unsigned int);
  itkSetMacro(GreenChannelIndex,unsigned int);
  itkGetMacro(GreenChannelIndex,unsigned int);
  itkSetMacro(BlueChannelIndex,unsigned int);
  itkGetMacro(BlueChannelIndex,unsigned int);

protected:
  /** Constructor */
  VectorImageToColorAnaglyphVectorImageFilter()
    {
      m_RedChannelIndex = 0;
      m_GreenChannelIndex = 1;
      m_BlueChannelIndex = 2;
    };

  virtual void BeforeThreadedGenerateData(void)
    {
      this->GetFunctor().SetRedChannelIndex(m_RedChannelIndex);
      this->GetFunctor().SetGreenChannelIndex(m_GreenChannelIndex);
      this->GetFunctor().SetBlueChannelIndex(m_BlueChannelIndex);
    };

  /** Destructor */
  virtual ~VectorImageToColorAnaglyphVectorImageFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    };

private:
  VectorImageToColorAnaglyphVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /// Color composition channels indices
  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;
};
}// End namespace otb
#endif
