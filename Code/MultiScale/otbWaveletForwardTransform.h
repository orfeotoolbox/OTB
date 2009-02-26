/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletForwardTransform__h
#define __otbWaveletForwardTransform__h

#include "itkImageToImageFilter.h"

namespace otb {

/** \class WaveletForwardTransform
 * \brief Wavelet transformation framework
 *
 * This class defines the Wavelet Packet transformation of an image
 * by using a (templated) elementary wavelet transform.
 *
 * Here the number of decomposition is given. The output is then given 
 * in the GetOutputs() array.
 *
 * Output image organization depends on the number of Dim on the image
 * In the OuputList, the first image is always the low passed one. Next, 
 * follow the high Passed images of the lowest decomposition (ask
 * GetFilter(0)->GetNumberOfOutputs() to know the length) and so on until 
 * the high passed images of the highest decomposition.
 *
 * \sa StationaryFilterBank
 * \sa WaveletPacketForwardTransform
 *
 */
template < class TInputImage, class TOutputImage, class TFilter >
class ITK_EXPORT WaveletForwardTransform
  : public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard typedefs */
  typedef WaveletForwardTransform Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Type macro */
	itkNewMacro(Self);

	/** Creation through object factory macro */
	itkTypeMacro(WaveletForwardTransform,ImageToImageFilter);

  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointer;
  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointer;

  typedef TFilter FilterType;
  typedef typename FilterType::Pointer FilterPointerType;
  typedef ObjectList< FilterType > FilterListType;
  typedef typename FilterListType::Pointer FilterListPointerType;

  itkGetObjectMacro(FilterList,FilterListType);
  FilterType * GetFilter ( unsigned int idx ) const
  {
    if ( idx >= m_NumberOfDecompositions )
      return NULL;
    return static_cast< FilterType* >( m_FilterList->GetNthElement( idx ) );
  }

  itkGetMacro(NumberOfDecompositions,unsigned int);
  void SetNumberOfDecompositions( unsigned int num ) ;

  itkStaticConstMacro(InputImageDimension, unsigned int,TInputImage::ImageDimension);

protected:
  WaveletForwardTransform();
  virtual ~WaveletForwardTransform() {}

  /** Generate data redefinition */
  virtual void GenerateData ();

private:
  WaveletForwardTransform ( const Self &);
  void operator= ( const Self & );

  unsigned int m_NumberOfDecompositions;
  FilterListPointerType m_FilterList;

  /*
   * At this level, m_UseSubSampleImage is set to FALSE to perform
   * multiscale analysis only
   */
  bool m_UseSubSampleImage;

}; // end of class

} // end of namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletForwardTransform.txx"
#endif


#endif


