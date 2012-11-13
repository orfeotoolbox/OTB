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
#ifndef __otbVectorDataProjectionFilter_h
#define __otbVectorDataProjectionFilter_h

#include "otbVectorDataToVectorDataFilter.h"
#include "itkTransform.h"
#include "otbGenericRSTransform.h"
#include "otbImageKeywordlist.h"

namespace otb
{

/** \class VectorDataProjectionFilter
  * \brief Reproject vector data in a different coordinate system
  *
  * This class is used to reproject vector data into a different coordinate system.
  * the input and output coordinate system can be a map projection, a raw image from a
  * sensor (the sensor model will be used), or the local coordinate system of an image.
  *
  * This filter works on otb::VectorData as input and output.
  *
  * The process goes as follow:
  * - offset/scaling of the input coordinates
  * - transform to get the data in geographic coordinates (lon/lat)
  * - transform from geographic coordinates
  * - offset/scaling of the output coordinates
  *
  * Each of this step is optional and would default to an identity transform if nothing
  * is specified.
  *
  * The offset/scaling steps are necessary only when working with the local coordinate
  * system of the image (origin on the top left). The value need to be provided by the
  * SetInputSpacing, SetInputOrigin, SetOutputSpacing and SetOutputOrigin methods.
  *
  * The two transforms derived from itk::Transform and will be instanciated as
  * otb::GenericMapProjection or otb::InverseSensorModel or otb::ForwardSensorModel
  * (according to the available information).
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  *
  * \example Projections/VectorDataProjectionExample.cxx
  *
  */

template <class TInputVectorData, class TOutputVectorData>
class ITK_EXPORT VectorDataProjectionFilter :
  public otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataProjectionFilter                                             Self;
  typedef otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>                                                Pointer;
  typedef itk::SmartPointer<const Self>                                          ConstPointer;

  typedef TInputVectorData                        InputVectorDataType;
  typedef TOutputVectorData                       OutputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;
  typedef typename TOutputVectorData::Pointer     OutputVectorDataPointer;

  /** Some typedefs. */
  typedef itk::Transform<double, 2, 2>           GenericTransformType;
  typedef typename GenericTransformType::Pointer GenericTransformPointerType;
//   typedef otb::CompositeTransform<GenericTransformType, GenericTransformType, double, 2, 2> InternalTransformType;
  typedef otb::GenericRSTransform<double, 2, 2>   InternalTransformType;
  typedef typename InternalTransformType::Pointer InternalTransformPointerType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  typedef typename OutputVectorDataType::DataNodeType        OutputDataNodeType;
  typedef typename InputVectorDataType::DataNodeType         InputDataNodeType;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType  InputInternalTreeNodeType;
  typedef typename OutputVectorDataType::DataTreeType::TreeNodeType OutputInternalTreeNodeType;

  typedef typename InputDataNodeType::PointType        InputPointType;
  typedef typename InputDataNodeType::LineType         InputLineType;
  typedef typename InputDataNodeType::PolygonType      InputPolygonType;
  typedef typename InputDataNodeType::PolygonListType  InputPolygonListType;

  typedef typename InputLineType::Pointer         InputLinePointerType;
  typedef typename InputPolygonType::Pointer      InputPolygonPointerType;
  typedef typename InputPolygonListType::Pointer  InputPolygonListPointerType;

  typedef typename OutputDataNodeType::PointType       OutputPointType;
  typedef typename OutputDataNodeType::LineType        OutputLineType;
  typedef typename OutputDataNodeType::PolygonType     OutputPolygonType;
  typedef typename OutputDataNodeType::PolygonListType OutputPolygonListType;

  typedef typename OutputLineType::Pointer         OutputLinePointerType;
  typedef typename OutputPolygonType::Pointer      OutputPolygonPointerType;
  typedef typename OutputPolygonListType::Pointer  OutputPolygonListPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataProjectionFilter, VectorDataToVectorDataFilter);

  /** Set/Get for input and output projections.  */
  itkSetStringMacro(InputProjectionRef);
  itkGetStringMacro(InputProjectionRef);

  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  itkGetMacro(InputKeywordList, ImageKeywordlist);
  void SetInputKeywordList(const ImageKeywordlist& kwl)
  {
    this->m_InputKeywordList = kwl;
    this->Modified();
  }

  itkGetMacro(OutputKeywordList, ImageKeywordlist);
  void SetOutputKeywordList(const ImageKeywordlist& kwl)
  {
    this->m_OutputKeywordList = kwl;
    this->Modified();
  }

  /** Set the origin of the vector data.
   * \sa GetOrigin() */
  itkSetMacro(InputOrigin, OriginType);
  virtual void SetInputOrigin(const double origin[2]);
  virtual void SetInputOrigin(const float origin[2]);

  itkGetConstReferenceMacro(InputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
   * \sa GetSpacing() */
  virtual void SetInputSpacing(const SpacingType& spacing);
  virtual void SetInputSpacing(const double spacing[2]);
  virtual void SetInputSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(InputSpacing, SpacingType);

  /** Set the origin of the vector data.
   * \sa GetOrigin() */
  itkSetMacro(OutputOrigin, OriginType);
  virtual void SetOutputOrigin(const double origin[2]);
  virtual void SetOutputOrigin(const float origin[2]);

  itkGetConstReferenceMacro(OutputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
   * \sa GetSpacing() */
  virtual void SetOutputSpacing(const SpacingType& spacing);
  virtual void SetOutputSpacing(const double spacing[2]);
  virtual void SetOutputSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  // Deprecated calls to elevation setters
  otbLegacyElevationMacro();

protected:
  VectorDataProjectionFilter();
  virtual ~VectorDataProjectionFilter() {}

  virtual OutputPointType ProcessPoint(InputPointType point) const;
  virtual OutputLinePointerType ProcessLine(InputLinePointerType line) const;
  virtual OutputPolygonPointerType ProcessPolygon(InputPolygonPointerType polygon) const;
  virtual OutputPolygonListPointerType ProcessPolygonList(InputPolygonListPointerType polygonList) const;

  virtual void InstanciateTransform(void);

  virtual void GenerateOutputInformation(void);
  virtual void GenerateData(void);

private:
  VectorDataProjectionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InternalTransformPointerType m_Transform;
  GenericTransformPointerType  m_InputTransform;
  GenericTransformPointerType  m_OutputTransform;
  std::string                  m_InputProjectionRef;
  std::string                  m_OutputProjectionRef;
  ImageKeywordlist             m_InputKeywordList;
  ImageKeywordlist             m_OutputKeywordList;

  SpacingType m_InputSpacing;
  OriginType  m_InputOrigin;
  SpacingType m_OutputSpacing;
  OriginType  m_OutputOrigin;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataProjectionFilter.txx"
#endif

#endif
