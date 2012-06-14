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
#ifndef __otbGenericRSTransform_h
#define __otbGenericRSTransform_h

#include "otbTransform.h"
#include "itkMacro.h"
#include "otbCompositeTransform.h"

namespace otb
{
namespace Projection
{
enum TransformAccuracy {UNKNOWN, ESTIMATE, PRECISE};
}
/** \class GenericRSTransform
 *  \brief This is the class to handle generic remote sensing transform
 *
 * The transformation will transform the coordinate from any map projection or sensor
 * model to any map projection or sensor model.
 *
 * If one of the projection (output or input) is a map projection, it can be
 * specified using the WKT or the EPSG code.
 *
 * \ingroup Projection
 *
 **/

template <class TScalarType = double,
    unsigned int NInputDimensions = 2,
    unsigned int NOutputDimensions = 2>
class ITK_EXPORT GenericRSTransform : public otb::Transform<TScalarType,          // Data type for scalars
      NInputDimensions,                                                       // Number of dimensions in the input space
      NOutputDimensions>                                                       // Number of dimensions in the output space
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef otb::Transform<TScalarType,
          NInputDimensions,
          NOutputDimensions>                Superclass;
  typedef GenericRSTransform            Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  typedef typename Superclass::ScalarType           ScalarType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  typedef itk::Transform<double, NInputDimensions, NOutputDimensions>         GenericTransformType;
  typedef typename GenericTransformType::Pointer                              GenericTransformPointerType;
  typedef otb::CompositeTransform<GenericTransformType, GenericTransformType> TransformType;
  typedef typename TransformType::Pointer                                     TransformPointerType;

  typedef typename Superclass::InverseTransformBasePointer InverseTransformBasePointer;

  typedef std::pair<InputPointType,InputPointType>  InputTiePointType;
  typedef std::pair<OutputPointType,InputPointType> OutputTiePointType;
  typedef std::vector<InputTiePointType>            InputTiePointContainerType;
  typedef std::vector<OutputTiePointType>           OutputTiePointContainerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GenericRSTransform, Transform);

  typedef TransformDirection::TransformationDirection DirectionOfMappingEnumType;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions * (NInputDimensions + 1));

  /** Set/Get for input and output projections.  */
  itkSetStringMacro(InputProjectionRef);
  itkGetStringMacro(InputProjectionRef);

  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  itkSetStringMacro(DEMDirectory);
  itkGetStringMacro(DEMDirectory);

  itkSetStringMacro(GeoidFile);
  itkGetStringMacro(GeoidFile);

  itkSetMacro(AverageElevation, double);
  itkGetMacro(AverageElevation, double);

  itkSetMacro(OptimizeInputTransform,bool);
  itkGetMacro(OptimizeInputTransform,bool);
  itkBooleanMacro(OptimizeInputTransform);

  itkSetMacro(OptimizeOutputTransform,bool);
  itkGetMacro(OptimizeOutputTransform,bool);
  itkBooleanMacro(OptimizeOutputTransform);

  void SetInputTiePoints(const InputTiePointContainerType & tiePoints)
  {
    m_InputTiePoints = tiePoints;
    this->Modified();
  }

  const InputTiePointContainerType& GetInputTiePoinst() const
  {
    return m_InputTiePoints;
  }

  void SetOutputTiePoints(const OutputTiePointContainerType & tiePoints)
  {
    m_OutputTiePoints = tiePoints;
    this->Modified();
  }

  const OutputTiePointContainerType& GetOutputTiePoinst() const
  {
    return m_OutputTiePoints;
  }

  void AddInputTiePoint(const InputPointType & inputPoint, const InputPointType & wgs84Point);

  void AddOutputTiePoint(const OutputPointType & outputPoint, const InputPointType & wgs84Point);

  void ClearInputTiePoints();

  void ClearOutputTiePoints();

  /** Set/Get Dictionary*/
  const itk::MetaDataDictionary& GetInputDictionary() const
  {
    return m_InputDictionary;
  }

  void SetInputDictionary(const itk::MetaDataDictionary& dictionary)
  {
    m_InputDictionary = dictionary;
    this->Modified();
  }

  const itk::MetaDataDictionary& GetOutputDictionary() const
  {
    return m_OutputDictionary;
  }

  void SetOutputDictionary(const itk::MetaDataDictionary& dictionary)
  {
    m_OutputDictionary = dictionary;
    this->Modified();
  }

  /** Set/Get Keywordlist*/

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
  itkGetConstReferenceMacro(InputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
    * \sa GetSpacing() */
  itkSetMacro(InputSpacing, SpacingType);
  itkGetConstReferenceMacro(InputSpacing, SpacingType);

  /** Set the origin of the vector data.
  * \sa GetOrigin() */
  itkSetMacro(OutputOrigin, OriginType);
  itkGetConstReferenceMacro(OutputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
  * \sa GetSpacing() */
  itkSetMacro(OutputSpacing, SpacingType);
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  /** Check if the transform is up to date */
  virtual bool IsUpToDate()
  {
    return m_TransformUpToDate;
  }

  /** Get Transform accuracy */
  itkGetMacro(TransformAccuracy, Projection::TransformAccuracy);

  /** Methods prototypes */
  virtual const TransformType * GetTransform() const;

  OutputPointType TransformPoint(const InputPointType& point) const;

  virtual void  InstanciateTransform();

  // Get inverse methods
  bool GetInverse(Self * inverseTransform) const;
  virtual InverseTransformBasePointer GetInverseTransform() const;

  // Dummy set parameter method
  virtual void SetParameters(const typename Superclass::ParametersType &)
  {}

protected:
  GenericRSTransform();
  virtual ~GenericRSTransform() {}

  virtual void Modified() const
  {
    this->Superclass::Modified();
    m_TransformUpToDate = false;
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  GenericRSTransform(const Self &);    //purposely not implemented
  void operator =(const Self&);    //purposely not implemented

  ImageKeywordlist m_InputKeywordList;
  ImageKeywordlist m_OutputKeywordList;

  itk::MetaDataDictionary m_InputDictionary;
  itk::MetaDataDictionary m_OutputDictionary;

  std::string m_InputProjectionRef;
  std::string m_OutputProjectionRef;
  std::string m_DEMDirectory;
  std::string m_GeoidFile;
  double      m_AverageElevation;

  SpacingType m_InputSpacing;
  OriginType  m_InputOrigin;
  SpacingType m_OutputSpacing;
  OriginType  m_OutputOrigin;

  TransformPointerType m_Transform;

  GenericTransformPointerType   m_InputTransform;
  GenericTransformPointerType   m_OutputTransform;
  mutable bool                  m_TransformUpToDate;
  Projection::TransformAccuracy m_TransformAccuracy;

  bool                          m_OptimizeInputTransform;
  bool                          m_OptimizeOutputTransform;
  InputTiePointContainerType    m_InputTiePoints;
  OutputTiePointContainerType   m_OutputTiePoints;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericRSTransform.txx"
#endif

#endif
