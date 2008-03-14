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
#ifndef _otbRationalQuotientResampleImageFilter_h
#define _otbRationalQuotientResampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkExpandImageFilter.h"
#include "itkShrinkImageFilter.h"

namespace otb
{
  /** \class RationalQuotientResampleImageFilter
   *  \brief This class is a composite filter, allowing user to resample
   *         an input image by rational quotient factor, for example *1.5
   *
   *  Combining an expand filter, then a shrink filter,
   *  denominator is applied as expand factor, whereas
   *  numerator is applied as shrink factor.
   */
  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT RationalQuotientResampleImageFilter
    : public itk::ImageToImageFilter<TInputImage,TOutputImage>
    {
    public:
      /** Standard typedefs */
      typedef RationalQuotientResampleImageFilter Self;
      typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      /** Creation through object factory macro */
      itkNewMacro(Self);
      
      /** Type macro */
      itkTypeMacro(RationalQuotientResampleImageFilter, ImageToImageFilter);

      itkStaticConstMacro(ImageDimension, unsigned int,
			  TInputImage::ImageDimension);
      itkStaticConstMacro(OutputImageDimension, unsigned int,
			  TOutputImage::ImageDimension );
      
      /** Display */
      void PrintSelf( std::ostream& os, itk::Indent indent ) const;

      /** Template parameters typedefs */
      typedef TInputImage InputImageType;
      typedef typename InputImageType::Pointer InputImagePointerType;
      typedef TOutputImage OutputImageType;
      typedef typename OutputImageType::Pointer OutputImagePointerType;
      
      /** Set numerator factors
       *  \param numerator The factor for expand filter, same for each dimension
       */
      virtual void SetNumeratorFactors(const unsigned int numerator);
      
      /** Set numerator factors
       *  \param numerators Tab of expand filter factors, along each dimension
       */
      virtual void SetNumeratorFactors( const unsigned int* numerators);

      /** Set numerator factors
       *  \param i Indice of dimension
       *  \param numerator The factor for expand filter
       */
      virtual void SetNumeratorFactors( const unsigned int i,
					const unsigned int numerator);
      
      /** Get numerator factors
       *  \return tab of numerator factors
       */
      virtual const unsigned int* GetNumeratorFactors() const
	{ return m_Numerators;}
      
      /** Set denominator factors
       *  \param denominator The factor for shrink filter, same along each dimension
       */
      virtual void SetDenominatorFactors(const unsigned int denominator);
      
      /** Set denominator factors
        *  \param denominators Tab of shrink filter factors, along each dimension
	*/
      virtual void SetDenominatorFactors(const unsigned int* denominators);

      /** Set denominator factors
       *  \param i Indice of dimension
       *  \param denominator The factor for shrink filter
       */
      virtual void SetDenominatorFactors(const unsigned int i,
					 const unsigned int denominator);
      
      /** Get denominator factors
        * \return tab of denominator factors
	*/
      virtual const unsigned int* GetDenominatorFactors() const
	{ return m_Denominators;}
      
    protected:

      /** Internal filters typedefs */
      typedef itk::ShrinkImageFilter<InputImageType, OutputImageType>
	ShrinkFilterType;
      typedef typename ShrinkFilterType::Pointer ShrinkFilterPointerType;
      
      typedef itk::ExpandImageFilter<InputImageType, OutputImageType>
	ExpandFilterType;
      typedef typename ExpandFilterType::Pointer ExpandFilterPointerType;
      
      /** Actually process the input */
      virtual void GenerateData();
      
      /** Constructor */
      RationalQuotientResampleImageFilter();
      
      /** Expand filter */
      ExpandFilterPointerType m_ExpandFilter;
      
      /** Shrink filter */
      ShrinkFilterPointerType m_ShrinkFilter;
      
      /** Destructor */
      virtual ~RationalQuotientResampleImageFilter() {}

      /** Numerator factors*/
      unsigned int m_Numerators[ImageDimension];
      
      /** Denominator factors*/
      unsigned int m_Denominators[ImageDimension];
      
    private:

      RationalQuotientResampleImageFilter(Self&);   // intentionally not implemented
      void operator=(const Self&);          // intentionally not implemented
      
    };
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRationalQuotientResampleImageFilter.txx"
#endif

#endif
