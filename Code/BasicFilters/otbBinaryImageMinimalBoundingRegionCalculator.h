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
#ifndef _otbBinaryImageMinimalBoundingRegionCalculator_h
#define _otbBinaryImageMinimalBoundingRegionCalculator_h

#include "itkImageToImageFilter.h"
#include "itkObjectFactory.h"

namespace otb
{
/**
 * \class BinaryImageMinimalBoundingRegionCalculator
 * \brief This class compute the smallest region of the image
 * containing every pixel with the foreground value. 
 *
 * This class is used for instance in the RCC8 calculator filter,
 * where the input region used for computation has to be the smallest possible
 * for costs reasons. The Pad flag allows the user to get a region 1 pixel larger
 * at each bound in case a security margin has to be kept. 
 *
 * \sa ImageToImageRCC8Calculator
 */
template <class TInputImage>
class BinaryImageMinimalBoundingRegionCalculator
  : public itk::ImageToImageFilter<TInputImage,TInputImage>
{
public:
	/** Standard typedefs */
	typedef BinaryImageMinimalBoundingRegionCalculator Self;
	typedef itk::ImageToImageFilter<TInputImage,TInputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	/** Creation through object factory macro */
	itkNewMacro(Self);
	/** Type macro */
	itkTypeMacro(BinaryImageMinimalBoundingRegionCalculator, ImageToImageFilter);

	typedef TInputImage InputImageType;
	typedef typename InputImageType::PixelType PixelType;
	typedef typename InputImageType::RegionType RegionType;
	typedef typename InputImageType::Pointer InputImagePointerType;
	/** Toogle the pad option */
	itkBooleanMacro(Pad);
	itkGetMacro(Region,RegionType);
	itkSetMacro(InsideValue,PixelType);
	itkGetMacro(InsideValue,PixelType);
	itkSetMacro(Pad,bool);
	itkGetMacro(Pad,bool);
	
protected:
	/** Constructor */
	BinaryImageMinimalBoundingRegionCalculator();
	/** Destructor */
	virtual ~BinaryImageMinimalBoundingRegionCalculator() {};
	/** Main computation method */
	void GenerateData(void);
	/** PrintSelf method */
	void PrintSelf(std::ostream& os, itk::Indent indent) const;
private:
	BinaryImageMinimalBoundingRegionCalculator(const Self&); // purposely not implemented
	void operator=(const Self&); // purposely not implemented
	/** The computed region */
	RegionType m_Region;
	/** Toogle if pad wanted */
	bool m_Pad;
	/** Inside value */
	PixelType m_InsideValue;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryImageMinimalBoundingRegionCalculator.txx"
#endif

#endif
