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
#ifndef __otbGenericMapProjection_h
#define __otbGenericMapProjection_h

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "projection/ossimProjection.h"
#include "itkTransform.h"
#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "base/ossimGpt.h"
#include "base/ossimDpt.h"
#include "projection/ossimProjection.h"
#include "base/ossimEllipsoid.h"
#include "base/ossimEllipsoidFactory.h"
#include "base/ossimString.h"
#include "ossimOgcWktTranslator.h"

namespace otb
{

  /** \class GenericMapProjection
   *  \brief This is the base class for generic map projection transformation
   *
   *
   * All derived class assume that the latitude and longitude are given according to the
   * WGS84 ellipsoid model.
   **/

  typedef enum {FORWARD=0, INVERSE=1} InverseOrForwardTransformationEnum;

  template <InverseOrForwardTransformationEnum Transform,
            class TScalarType = double,
            unsigned int NInputDimensions=2,
            unsigned int NOutputDimensions=2>
    class ITK_EXPORT GenericMapProjection: public itk::Transform<TScalarType,       // Data type for scalars
                                                          NInputDimensions,  // Number of dimensions in the input space
                                                          NOutputDimensions> // Number of dimensions in the output space
  {
    public :
      /** Standard class typedefs. */
      typedef itk::Transform< TScalarType,
                              NInputDimensions,
                              NOutputDimensions >       Superclass;
      typedef GenericMapProjection                      Self;
      typedef itk::SmartPointer<Self>                   Pointer;
      typedef itk::SmartPointer<const Self>             ConstPointer;

      typedef typename Superclass::ScalarType           ScalarType;
      typedef ossimProjection                           OssimMapProjectionType;
      typedef itk::Point<ScalarType,NInputDimensions >  InputPointType;
      typedef itk::Point<ScalarType,NOutputDimensions > OutputPointType;

      /** Method for creation through the object factory. */
      itkNewMacro( Self );

      /** Run-time type information (and related methods). */
      itkTypeMacro( GenericMapProjection, Transform );

      itkSetObjectMacro(MapProjection,OssimMapProjectionType);
      virtual OssimMapProjectionType* GetMapProjection ();

      typedef InverseOrForwardTransformationEnum DirectionOfMappingEnumType;

      itkStaticConstMacro(DirectionOfMapping,DirectionOfMappingEnumType,Transform);
      itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
      itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
      itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
      itkStaticConstMacro(ParametersDimension, unsigned int,NInputDimensions*(NInputDimensions+1));


      /** Return the Wkt representation of the projection*/
      virtual std::string GetWkt();

      /** Instanciate the projection according to the Wkt specification*/
      virtual void SetWkt(std::string projectionRefWkt);

      virtual void PrintMap() const;

      OutputPointType TransformPoint(const InputPointType &point);

    protected:
      GenericMapProjection();
      virtual ~GenericMapProjection();
      OssimMapProjectionType* m_MapProjection;
      std::string m_ProjectionRefWkt;
      virtual void InstanciateProjection();
      bool reinstanciateProjection;

    private :
      GenericMapProjection(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

  };



} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericMapProjection.txx"
#endif

#endif
