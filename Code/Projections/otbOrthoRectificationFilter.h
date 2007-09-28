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
#ifndef __otbOrthoRectificationFilter_h
#define __otbOrthoRectificationFilter_h

#include <iostream>
#include <sstream>
#include <stdio.h>

#include "otbMapProjection.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbCompositeTransform.h"

namespace otb
{

/** Macro used to access Resampler class */
/*#define otbSetResampleMacro(name, type)\
	virtual void SetResample##name( const type & _arg)\
	{\
		if (m_Resampler->Get##name() != _arg)\
		{\
			m_Resampler->Set##name(_arg);\
			this->Modified();\
		}\
	}\*/

/**Template otbOrthoRectificationFilter.txx
* Cette classe permet de passer d'une MapProjection à une autre en passant par un point géographique. 
* Ceci pour éviter TOUTES les combinaisons possibles de passage d'une proj. carto à une autre.
* Template: Pour vérifier les types.
**/
template <class TInputImage,
					class TOutputImage,
					class TMapProjection,
					class TInterpolatorPrecision=double>
class ITK_EXPORT OrthoRectificationFilter : public StreamingResampleImageFilter<TInputImage, TOutputImage,TInterpolatorPrecision>  
{
public :
  
	/** Standard class typedefs */
  typedef StreamingResampleImageFilter<TInputImage, TOutputImage,TInterpolatorPrecision>  			Superclass;
  typedef OrthoRectificationFilter    										Self;
  typedef itk::SmartPointer<Self>              										Pointer;
  typedef itk::SmartPointer<const Self>        										ConstPointer;
	
	 
//  typedef otb::StreamingResampleImageFilter< TInputImage, TOutputImage  >          ResamplerType;
//	typedef typename ResamplerType::Pointer     ResamplerPointerType;
	typedef typename TInputImage::IndexType 	  IndexType;
	typedef typename TInputImage::SizeType  		SizeType;
  typedef typename TInputImage::SpacingType  	SpacingType;
  typedef typename TInputImage::PointType			PointType;
  typedef typename TInputImage::RegionType    RegionType;

	typedef typename TOutputImage::PixelType    OutputPixelType;

	typedef TMapProjection											MapProjectionType;
	typedef typename TMapProjection::Pointer    MapProjectionPointerType;

  typedef InverseSensorModel<double> SensorModelType;
	typedef typename SensorModelType::Pointer SensorModelPointerType;
	
	typedef CompositeTransform<SensorModelType,MapProjectionType> CompositeTransformType; 
	typedef typename CompositeTransformType::Pointer CompositeTransformPointerType; 
	
	typedef typename Superclass::InterpolatorType InterpolatorType;
//	typedef typename InterpolatorType::Pointer InterpolatorPointerType;

  /** Method for creation through the object factory. */
	itkNewMacro( Self );
	
	/** Run-time type information (and related methods). */
	itkTypeMacro( OrthoRectificationFilter, StreamingResampleImageFilter );
	
	/** Accessors */
	itkSetMacro(MapProjection, MapProjectionPointerType);
	itkGetMacro(MapProjection, MapProjectionPointerType);
	
protected:
	OrthoRectificationFilter();
	~OrthoRectificationFilter();
	void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateInputRequestedRegion();

	
private:

  OrthoRectificationFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

	/** Calculate transformation model from sensor model & map projection	composition */	
	void ComputeResampleTransformationModel();
	
	void Modified();
	
	/** Boolean used to know if transformation model computation is needed */
	bool m_IsComputed;	
	SensorModelPointerType m_SensorModel;
	MapProjectionPointerType m_MapProjection;
	CompositeTransformPointerType m_CompositeTransform;
	
	
	
};





} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOrthoRectificationFilter.txx"
#endif

#endif
