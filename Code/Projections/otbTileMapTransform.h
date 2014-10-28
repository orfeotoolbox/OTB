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
#ifndef __otbTileMapTransform_h
#define __otbTileMapTransform_h


// Only for the enum definition
#include "otbGenericMapProjection.h"

namespace otb
{

/** \class TileMapTransform
 *  \brief to do
 **/

template <TransformDirection::TransformationDirection TTransformDirection,
    class TScalarType = double,
    unsigned int NInputDimensions = 2,
    unsigned int NOutputDimensions = 2>
class ITK_EXPORT TileMapTransform : public Transform<TScalarType,           // Data type for scalars
      NInputDimensions,                                                     // Number of dimensions in the input space
      NOutputDimensions>                                                    // Number of dimensions in the output space
{
public:
  /** Standard class typedefs. */
  typedef TileMapTransform Self;
  typedef Transform<TScalarType,
      NInputDimensions,  NOutputDimensions>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::ScalarType           ScalarType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileMapTransform, Transform);

  static const TransformDirection::TransformationDirection DirectionOfMapping = TTransformDirection;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions * (NInputDimensions + 1));

  itkGetConstMacro(Depth, int);
  itkSetMacro(Depth, int);

  void SetLevel(unsigned int level);
  unsigned int GetLevel() const;

  OutputPointType TransformPoint(const InputPointType& point) const;

  virtual void PrintMap() const;

  /** FIXME with a Mercator projection definition*/
  virtual std::string GetWkt() const
  {
    return "TileMapTransform";
  }

protected:
  TileMapTransform();
  virtual ~TileMapTransform();

private:
  TileMapTransform(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  int m_Depth;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTileMapTransform.txx"
#endif

#endif
