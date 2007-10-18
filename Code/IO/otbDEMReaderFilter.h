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
#ifndef __otbDEMReaderFilter_h
#define __otbDEMReaderFilter_h

#include "itkIndent.h"
#include "itkImageSource.h"
#include "otbImage.h"
#include <iostream>
#include <stdio.h>
#include "elevation/ossimElevManager.h"
#include "base/ossimFilename.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkIndent.h"

namespace otb
{
/** \class DEMReaderFilter
 *
 * \brief Class for Reading a DEM data
 *
 * This class is based on ossimElevManager. It takes in input the UL and LR geographic coordinates and the spacing.
 * Handle DTED and SRTM formats.
 * \ingroup Images
 *
 */
template <class TDEMImage>
class ITK_EXPORT DEMReaderFilter: 
public itk::ImageSource<Image<typename TDEMImage::PixelType,2, 0> >
{
public :
/** Standard class typedefs. */
  typedef itk::Indent														Indent;
  typedef TDEMImage					      									DEMImageType;
  typedef typename DEMImageType::Pointer									DEMImagePointerType;
  typedef typename DEMImageType::PixelType                 					PixelType;
  
  typedef DEMReaderFilter                                	                       	Self;
  typedef itk::ImageSource<Image<typename DEMImageType::PixelType,2, 0> >	Superclass;
  typedef itk::SmartPointer<Self>                    	      				Pointer;
  typedef itk::SmartPointer<const Self>              	      				ConstPointer;
  typedef Image<PixelType,2>           						   				OutputImageType; 

  typedef typename Superclass::Pointer    					    			OutputImagePointer;
  typedef typename OutputImageType::SpacingType 							SpacingType;
  typedef typename OutputImageType::SizeType 								SizeType;
  typedef typename OutputImageType::PointType 								PointType;
  typedef typename OutputImageType::IndexType 								IndexType;
  typedef typename Superclass::OutputImageRegionType 				        OutputImageRegionType;
  typedef itk::ImageRegionIteratorWithIndex< Image<PixelType,2, 0> >      	ImageIteratorType;
  
	/** Method for creation through the object factory. */
  itkNewMacro(Self);
  
	/** Run-time type information (and related methods). */
  itkTypeMacro(DEMReader,ImageSource);
  	                                      
	/** Set the spacing. */
	itkSetMacro(Spacing,SpacingType);   
	itkGetConstReferenceMacro(Spacing,SpacingType);
	
	/** Set the Upper Left coordinates. */
	itkSetMacro(Ul,PointType);
	itkGetConstReferenceMacro(Ul,PointType);

	/** Set the Lower Right coordinates. */
	itkSetMacro(Lr,PointType);
	itkGetConstReferenceMacro(Lr,PointType);

  /** Set the spacing. */    
	void SetSpacing(const double* spacing); 

	/** Try to open the DEM directory. */
	bool OpenDEMDirectory(char* &DEMDirectory);         

	/** Compute the height above MSL(Mean Sea Level) of the point. */
	virtual double GetHeightAboveMSL(const PointType& worldPoint);     

protected:
	DEMReaderFilter();
	~DEMReaderFilter();

	void PrintSelf(std::ostream& os, Indent indent) const;
	void GenerateData();
	virtual void GenerateOutputInformation();

	ossimElevManager* m_ElevManager;
	//DEMImagePointerType  m_DEMImage;
	SpacingType m_Spacing;
	PointType m_Ul;
	PointType m_Lr;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDEMReaderFilter.txx"
#endif

#endif
