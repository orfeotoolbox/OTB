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
#ifndef __otbGenericRSTransform_txx
#define __otbGenericRSTransform_txx

#include "otbGenericRSTransform.h"
#include "otbMacro.h"
// #include "projection/ossimMapProjectionFactory.h"
// #include "projection/ossimMapProjection.h"
#include "itkMetaDataObject.h"

//TODO OTB wrapper around the WKT/ossimProjection/isGeographic, etc.
#include "ogr_spatialref.h"

namespace otb
{

template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::GenericRSTransform() : Superclass(SpaceDimension,ParametersDimension)
{
  m_InputProjectionRef.clear();
  m_OutputProjectionRef.clear();
  m_InputKeywordList.Clear();
  m_OutputKeywordList.Clear();
  m_InputSpacing.Fill(1);
  m_InputOrigin.Fill(0);
  m_OutputSpacing.Fill(1);
  m_OutputOrigin.Fill(0);

  m_Transform = NULL;
  m_InputTransform = NULL;
  m_OutputTransform = NULL;
  m_TransformUpToDate = false;
  m_TransformAccuracy = Projection::UNKNOWN;
}



//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetInputSpacing(const SpacingType & spacing )
{
  itkDebugMacro("setting Spacing to " << spacing);
  if ( this->m_InputSpacing != spacing )
  {
    this->m_InputSpacing = spacing;
    this->Modified();
  }
}


//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetInputSpacing(const double spacing[2] )
{
  SpacingType s(spacing);
  this->SetInputSpacing(s);
}


//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetInputSpacing(const float spacing[2] )
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom( sf );
  this->SetInputSpacing(s);
}

//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetInputOrigin(const double origin[2] )
{
  OriginType p(origin);
  this->SetInputOrigin( p );
}


//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetInputOrigin(const float origin[2] )
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom( of );
  this->SetInputOrigin( p );
}



//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetOutputSpacing(const SpacingType & spacing )
{
  itkDebugMacro("setting Spacing to " << spacing);
  if ( this->m_OutputSpacing != spacing )
  {
    this->m_OutputSpacing = spacing;
    this->Modified();
  }
}


//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetOutputSpacing(const double spacing[2] )
{
  SpacingType s(spacing);
  this->SetOutputSpacing(s);
}


//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetOutputSpacing(const float spacing[2] )
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom( sf );
  this->SetOutputSpacing(s);
}

//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetOutputOrigin(const double origin[2] )
{
  OriginType p(origin);
  this->SetOutputOrigin( p );
}


//----------------------------------------------------------------------------
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    void
        GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetOutputOrigin(const float origin[2] )
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom( of );
  this->SetOutputOrigin( p );
}


template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    const typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::TransformType*
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::GetTransform() const
{
  itkDebugMacro("returning MapProjection address " << this->m_Transform );
  if ((!m_TransformUpToDate) || (m_Transform.IsNull()))
  {
    itkExceptionMacro(<<"m_Transform not up-to-date, call InstanciateTransform() first");
  }

  return this->m_Transform;
}





/**
 * Instanciate the transformation according to informations
 */
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::InstanciateTransform(void)
{
  m_Transform = TransformType::New();

  if (m_InputKeywordList.GetSize()  == 0)
  {
    itk::ExposeMetaData<ImageKeywordlist>(m_InputDictionary, MetaDataKey::OSSIMKeywordlistKey, m_InputKeywordList );
  }
  if (m_InputProjectionRef.empty())
  {
    itk::ExposeMetaData<std::string>(m_InputDictionary, MetaDataKey::ProjectionRefKey, m_InputProjectionRef );
  }

  otbMsgDevMacro(<< "Information to instanciate transform: ");
  otbMsgDevMacro(<< " * Input Origin: " << m_InputOrigin);
  otbMsgDevMacro(<< " * Input Spacing: " << m_InputSpacing);
  otbMsgDevMacro(<< " * Input keyword list: "
      << ((m_InputKeywordList.GetSize() == 0)?"Empty":"Full"));
  otbMsgDevMacro(<< " * Input projection: " << m_InputProjectionRef);
  otbMsgDevMacro(<< " * Output keyword list: "
      << ((m_OutputKeywordList.GetSize() == 0)?"Empty":"Full"));
  otbMsgDevMacro(<< " * Output projection: " << m_OutputProjectionRef);
  otbMsgDevMacro(<< " * Output Origin: " << m_OutputOrigin);
  otbMsgDevMacro(<< " * Output Spacing: " << m_OutputSpacing);

  //Make sure that the state is clean:
  m_InputTransform = NULL;
  m_OutputTransform = NULL;

  bool firstTransformGiveGeo = true;
  bool inputTransformIsSensor = false;
  bool inputTransformIsMap = false;
  bool outputTransformIsSensor = false;
  bool outputTransformIsMap = false;

  //*****************************
  //Set the input transformation
  //*****************************
  try
    {
    if (m_InputKeywordList.GetSize()  > 0)
      {
      typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
      ForwardSensorModelType::Pointer sensorModel = ForwardSensorModelType::New();
      sensorModel->SetImageGeometry(m_InputKeywordList);
      if ( !m_DEMDirectory.empty())
        {
        sensorModel->SetDEMDirectory(m_DEMDirectory);
        }
      m_InputTransform = sensorModel.GetPointer();
      inputTransformIsSensor = true;
      otbMsgDevMacro(<< "Input projection set to sensor model.");
      }
    }
  catch(itk::ExceptionObject &)
    {
    otbMsgDevMacro(<<" Input keyword list does not describe a sensor model.");
    }


  if ((m_InputTransform.IsNull()) && ( !m_InputProjectionRef.empty() ))//map projection
  {
    typedef otb::GenericMapProjection<otb::INVERSE> InverseMapProjectionType;
    InverseMapProjectionType::Pointer mapTransform = InverseMapProjectionType::New();
    mapTransform->SetWkt(m_InputProjectionRef);
    if (mapTransform->GetMapProjection() != NULL)
    {
      m_InputTransform = mapTransform.GetPointer();
      inputTransformIsMap = true;
      otbMsgDevMacro(<< "Input projection set to map transform: " << m_InputTransform);
    }

  }

  if (m_InputTransform.IsNull())//default if we didn't manage to instantiate it before
  {
    m_InputTransform = itk::IdentityTransform< double, 2 >::New();
//     firstTransformGiveGeo = false;

    OGRSpatialReferenceH  hSRS = NULL;
    hSRS = OSRNewSpatialReference(NULL);
    const char * wktString = m_InputProjectionRef.c_str();
    if( OSRImportFromWkt( hSRS, (char **) &wktString ) != OGRERR_NONE )
    {
      OSRDestroySpatialReference( hSRS );
      firstTransformGiveGeo = false;
    }

    else if (static_cast<OGRSpatialReference *>(hSRS)->IsGeographic())
    {
      OSRDestroySpatialReference( hSRS );
      firstTransformGiveGeo = true;
    }
    else
    {
      OSRDestroySpatialReference( hSRS );
      firstTransformGiveGeo = false;
    }

    otbMsgDevMacro(<< "Input projection set to identity")
  }

  //*****************************
  //Set the output transformation
  //*****************************
  try
    {
    if (m_OutputKeywordList.GetSize()  > 0)
      {
      typedef otb::InverseSensorModel<double> InverseSensorModelType;
      InverseSensorModelType::Pointer sensorModel = InverseSensorModelType::New();
      sensorModel->SetImageGeometry(m_OutputKeywordList);
      if ( !m_DEMDirectory.empty())
        {
        sensorModel->SetDEMDirectory(m_DEMDirectory);
        }
      m_OutputTransform = sensorModel.GetPointer();
      outputTransformIsSensor = true;
      otbMsgDevMacro(<< "Output projection set to sensor model");
      }
    }
  catch(itk::ExceptionObject &)
    {
    otbMsgDevMacro(<<" Output keyword list does not describe a sensor model.");
    }


  if ((m_OutputTransform.IsNull()) && ( !m_OutputProjectionRef.empty() ))//map projection
  {
    typedef otb::GenericMapProjection<otb::FORWARD> ForwardMapProjectionType;
    ForwardMapProjectionType::Pointer mapTransform = ForwardMapProjectionType::New();
    mapTransform->SetWkt(m_OutputProjectionRef);
    if (mapTransform->GetMapProjection() != NULL)
    {
      m_OutputTransform = mapTransform.GetPointer();
      outputTransformIsMap = true;
      otbMsgDevMacro(<< "Output projection set to map transform: " << m_OutputTransform);
    }

  }

  if (m_OutputTransform.IsNull())//default if we didn't manage to instantiate it before
  {
    m_OutputTransform = itk::IdentityTransform< double, 2 >::New();
    if (firstTransformGiveGeo)
    {
      m_OutputProjectionRef = "GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]";
    }
    otbMsgDevMacro(<< "Output projection set to identity");
  }


  m_Transform->SetFirstTransform(m_InputTransform);
  m_Transform->SetSecondTransform(m_OutputTransform);
  m_TransformUpToDate = true;
  //The acurracy information is a simplistic model for now and should be refined
  if ((inputTransformIsSensor || outputTransformIsSensor) && (m_DEMDirectory.empty()))
  {
    //Sensor model without DEM
    m_TransformAccuracy = Projection::ESTIMATE;
  }
  else if (!inputTransformIsSensor && !outputTransformIsSensor && !inputTransformIsMap && !outputTransformIsMap)
  {
    //no transform
    m_TransformAccuracy = Projection::UNKNOWN;
  }
  else
  {

    m_TransformAccuracy = Projection::PRECISE;
  }

}


template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType & point) const
{
  InputPointType inputPoint = point;

  // Apply input origin/spacing
  inputPoint[0] = inputPoint[0] * m_InputSpacing[0] + m_InputOrigin[0];
  inputPoint[1] = inputPoint[1] * m_InputSpacing[1] + m_InputOrigin[1];

  // Transform point
  OutputPointType outputPoint;
  outputPoint = this->GetTransform()->TransformPoint(inputPoint);

  // Apply output origin/spacing
  outputPoint[0] = (outputPoint[0] - m_OutputOrigin[0]) / m_OutputSpacing[0];
  outputPoint[1] = (outputPoint[1] - m_OutputOrigin[1]) / m_OutputSpacing[1];

  return outputPoint;
}

  //TODO
// template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
// void
// GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
// ::PrintSelf() const
// {
//
//   std::cout << m_Transform->print(std::cout);
// }



} // namespace otb

#endif
