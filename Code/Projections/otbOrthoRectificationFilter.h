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
#include "otbInverseSensorModel.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbCompositeTransform.h"
#include "otbInverseSensorModel.h"

namespace otb
{
  /** \class OrthoRectificationFilter
   *
   * \brief Class for Orthorectifying an image
   *
   * This class is used to apply map projection and sensor model transformation
   * to orthorectify an image, with or without DEM.
   *
   * \example Projections/OrthoRectificationExample.cxx
   *
   */

  template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision=double>
      class ITK_EXPORT OrthoRectificationFilter :
          public StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>
  {
    public :
      /** Standard class typedefs */
      typedef StreamingResampleImageFilter<TInputImage,
      TOutputImage,
      TInterpolatorPrecision>  Superclass;
      typedef OrthoRectificationFilter    			    Self;
      typedef itk::SmartPointer<Self>              		    Pointer;
      typedef itk::SmartPointer<const Self>        		    ConstPointer;

      typedef typename TInputImage::IndexType 	  IndexType;
      typedef typename TInputImage::SizeType  	  SizeType;
      typedef typename TInputImage::SpacingType   SpacingType;
      typedef typename TInputImage::PointType	  PointType;
      typedef typename TInputImage::RegionType    RegionType;

      typedef typename TOutputImage::PixelType    OutputPixelType;

      typedef TMapProjection			  MapProjectionType;
      typedef typename TMapProjection::Pointer    MapProjectionPointerType;

      typedef InverseSensorModel<double>          SensorModelType;
      typedef typename SensorModelType::Pointer   SensorModelPointerType;

      typedef CompositeTransform< MapProjectionType,SensorModelType> CompositeTransformType;
      typedef typename CompositeTransformType::Pointer               CompositeTransformPointerType;

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

      /** Specify where are DEM files, and load useful ones */
      virtual void SetDEMDirectory(const std::string& directory)
      {
        m_SensorModel->SetDEMDirectory(directory);
        this->Modified();
      }

      /** Methods to enable DEM */
      virtual void EnableDEM()
      {
        m_SensorModel->EnableDEM();
        this->Modified();
      }

      /** Methods to desable DEM */
      virtual void DisableDEM()
      {
        m_SensorModel->DisableDEM();
        this->Modified();
      }


      /** Specify average elevation.*/
      virtual void SetAverageElevation(double elevation)
      {
        m_SensorModel->SetAverageElevation(elevation);
        this->Modified();
      }




    protected:
      OrthoRectificationFilter();
      ~OrthoRectificationFilter();
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

      virtual void GenerateInputRequestedRegion();


    private:
      OrthoRectificationFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      /** Calculate transformation model from sensor model and map projection	composition */
      void ComputeResampleTransformationModel();

      /** Boolean used to know if transformation model computation is needed */
      bool m_IsComputed;

      /** Sensor Model used to transform geographic coordinates in image sensor	index */
      SensorModelPointerType m_SensorModel;

      /** Map Projection used to transform cartographic coordinates in geographic	coordinates */
      MapProjectionPointerType m_MapProjection;

      /** Composite Transform of Sensor Model and Map Projection, used for Resampler */
      CompositeTransformPointerType m_CompositeTransform;

  };

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOrthoRectificationFilter.txx"
#endif

#endif
