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
#include "otbInverseSensorModel.h"

namespace otb
{



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
	
	typedef CompositeTransform< MapProjectionType,SensorModelType> CompositeTransformType; 
	typedef typename CompositeTransformType::Pointer CompositeTransformPointerType; 
	
  /** Method for creation through the object factory. */
	itkNewMacro( Self );
	
	/** Run-time type information (and related methods). */
	itkTypeMacro( OrthoRectificationFilter, StreamingResampleImageFilter );
	
	/** Accessors */
	virtual void SetMapProjection (MapProjectionType* _arg) 
  { 
    if (this->m_MapProjection != _arg) 
      { 
      this->m_MapProjection = _arg; 
      m_CompositeTransform->SetFirstTransform(_arg);
			m_IsComputed = false;
			this->Modified(); 
      } 
  } 

	itkGetObjectMacro(MapProjection, MapProjectionType);
	
	virtual void SetDEMDirectory(const std::string& directory)
	{
		m_SensorModel->SetDEMDirectory(directory);
		this->Modified();
	}	
	
	virtual void ActiveDEM()
	{
		m_SensorModel->ActiveDEM();
	}
	
	virtual void DesactiveDEM()
	{
		m_SensorModel->DesactiveDEM();
	}
	
	
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
