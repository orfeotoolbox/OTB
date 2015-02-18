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
#ifndef __otbDataNodeVectorDataFunction_h
#define __otbDataNodeVectorDataFunction_h

#include "otbDataNodeFunctionBase.h"
#include "otbVectorDataProperties.h"


namespace otb
{
/** \class DataNodeVectorDataFunction
  * \brief TODO
  *
  *
  *
  * \ingroup Functions
  * \sa
 */

template <
class TOutput,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT DataNodeVectorDataFunction :
    public DataNodeFunctionBase<DataNode<TCoordRep,
                                         2,
                                         TPrecision>,
                                TOutput>
{
public:
   /** Standard class typedefs. */
  typedef DataNodeVectorDataFunction               Self;
  typedef DataNodeFunctionBase<DataNode<TCoordRep,
                                        2,
                                        TPrecision>,
                               TOutput>            Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DataNodeVetorDataFunction, DataNodeFunctionBase);

  /** Some typedefs. */
  typedef VectorData<TCoordRep, 2, TPrecision>    VectorDataType;
  typedef typename VectorDataType::ConstPointer   VectorDataConstPointerType;
  typedef TOutput                                 OutputType;

  /** CoordRepType typedef support. */
  typedef TCoordRep CoordRepType;

  /** PrecisionType typedef support. */
  typedef TPrecision PrecisionType;

  /** DataNode Type */
  typedef typename Superclass::DataNodeType       DataNodeType;

  /** Point Type */
  typedef typename DataNodeType::PointType        PointType;

  /** Set the input VetorData. */
  virtual void SetInputVectorData( const VectorDataType * ptr );

  /** Get the input VectorData. */
  const VectorDataType * GetInputVectorData() const
    { return m_VectorData.GetPointer(); }

  /** Evaluate the function at specified DataNode position.
   * Subclasses must provide this method. */
  virtual TOutput Evaluate( const DataNodeType& node ) const = 0;

protected:
  DataNodeVectorDataFunction();
  ~DataNodeVectorDataFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Const pointer to the input VectorData. */
  VectorDataConstPointerType  m_VectorData;

private:
  DataNodeVectorDataFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDataNodeVectorDataFunction.txx"
#endif

#endif
