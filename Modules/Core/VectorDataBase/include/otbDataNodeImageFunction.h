/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbDataNodeImageFunction_h
#define otbDataNodeImageFunction_h

#include "otbDataNodeFunctionBase.h"
#include "otbDataNode.h"
#include "itkContinuousIndex.h"
#include "itkImageBase.h"

namespace otb
{
/** \class DataNodeImageFunction
  * \brief TODO
  *
  *
  *
  * \ingroup Functions
  * \sa NDVIDataNodeFeatureFunction
 *
 * \ingroup OTBVectorDataBase
 */

template <
class TImage,
class TOutput,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT DataNodeImageFunction :
    public DataNodeFunctionBase<DataNode<TCoordRep,
                                         TImage::ImageDimension,
                                         TPrecision>,
                                TOutput>
{
public:
  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard class typedefs. */
  typedef DataNodeImageFunction                    Self;
  typedef DataNodeFunctionBase<DataNode<TCoordRep,
                                        itkGetStaticConstMacro(ImageDimension),
                                        TPrecision>,
                               TOutput>            Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DataNodeImageFunction, DataNodeFunctionBase);

  /** Some typedefs. */
  typedef TImage                                  InputImageType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;

  typedef TOutput                                 OutputType;

  /** CoordRepType typedef support. */
  typedef TCoordRep CoordRepType;

  /** PrecisionType typedef support. */
  typedef TPrecision PrecisionType;

  /** Index Type. */
  typedef typename InputImageType::IndexType      IndexType;
  typedef typename InputImageType::IndexValueType IndexValueType;

  /** ContinuousIndex Type. */
  typedef itk::ContinuousIndex<CoordRepType, itkGetStaticConstMacro(ImageDimension)>
                                                  ContinuousIndexType;

  /** DataNode Type */
  typedef typename Superclass::DataNodeType       DataNodeType;

  /** Point Type */
  typedef typename DataNodeType::PointType        PointType;

  /** Set the input image.
   * \warning this method caches BufferedRegion information.
   * If the BufferedRegion has changed, user must call
   * SetInputImage again to update cached values. */
  virtual void SetInputImage( const InputImageType * ptr );

  /** Get the input image. */
  const InputImageType * GetInputImage() const
    { return m_Image.GetPointer(); }

  /** Evaluate the function at specified DataNode position.
   * Subclasses must provide this method. */
  TOutput Evaluate( const DataNodeType& node ) const override = 0;

  /** Check if an index is inside the image buffer.
   * we take into account the fact that each voxel has its
   * center at the integer coordinate and extends half way
   * to the next integer coordinate.
   * \warning For efficiency, no validity checking of
   * the input image is done. */
  virtual bool IsInsideBuffer( const IndexType & index ) const
    {
    for( unsigned int j = 0; j < ImageDimension; ++j )
      {
      if( index[j] < m_StartIndex[j] )
        {
        return false;
        }
      if( index[j] > m_EndIndex[j] )
        {
        return false;
        }
      }
    return true;
    }

  /** Check if a continuous index is inside the image buffer.
   * \warning For efficiency, no validity checking of
   * the input image is done. */
  virtual bool IsInsideBuffer( const ContinuousIndexType & index ) const
    {
    for( unsigned int j = 0; j < ImageDimension; ++j )
      {
      if( index[j] < m_StartContinuousIndex[j] )
        {
        return false;
        }
      if( index[j] >= m_EndContinuousIndex[j] )
       //Comment this instruction after itkv4 migration (correct
       //usage of centered-pixel coordinates)
       //if( index[j] > m_EndContinuousIndex[j] )
       {
        return false;
        }
      }
    return true;
    }

  /** Check if a point is inside the image buffer.
   * \warning For efficiency, no validity checking of
   * the input image pointer is done. */
  virtual bool IsInsideBuffer( const PointType & point ) const
    {
    ContinuousIndexType index;
    m_Image->TransformPhysicalPointToContinuousIndex( point, index );
    return this->IsInsideBuffer( index );
    }

  /** Convert point to continuous index */
  void ConvertPointToContinuousIndex( const PointType & point,
    ContinuousIndexType & cindex ) const
    {
    m_Image->TransformPhysicalPointToContinuousIndex( point, cindex );
    }

  /** Convert continuous index to nearest index. */
  inline void ConvertContinuousIndexToNearestIndex( const ContinuousIndexType & cindex,
    IndexType & index ) const
    {
    index.CopyWithRound( cindex );
    }

  /** Set/Get methods */
  itkGetConstReferenceMacro(StartIndex, IndexType);
  itkGetConstReferenceMacro(EndIndex, IndexType);

  itkGetConstReferenceMacro(StartContinuousIndex, ContinuousIndexType);
  itkGetConstReferenceMacro(EndContinuousIndex, ContinuousIndexType);

protected:
  DataNodeImageFunction();
  ~DataNodeImageFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Const pointer to the input image. */
  InputImageConstPointer  m_Image;

  /** Cache some values for testing if indices are inside buffered region. */
  IndexType               m_StartIndex;
  IndexType               m_EndIndex;
  ContinuousIndexType     m_StartContinuousIndex;
  ContinuousIndexType     m_EndContinuousIndex;

private:
  DataNodeImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDataNodeImageFunction.hxx"
#endif

#endif
