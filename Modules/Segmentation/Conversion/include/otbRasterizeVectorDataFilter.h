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
#ifndef __otbRasterizeVectorDataFilter_h
#define __otbRasterizeVectorDataFilter_h

#include "itkImageToImageFilter.h"
#include "itkCastImageFilter.h"
#include "otbMacro.h"

#include "otbVectorData.h"

#include "gdal.h"
#include "gdal_alg.h"

#include <ogrsf_frmts.h>
#include "ogr_api.h"
#include "ogr_srs_api.h"

namespace otb {

/** \class RasterizeVectorDataFilter
 *  \brief Burn geometries from the specified VectorData into raster
 *
 *  This class handles burning several input VectorDatas into the
 *  input raster. It handles producing the output in any pixel type
 *  supported by OTB since this class inherits itk::CastImageFilter.
 *
 *  An explicit list of burn values for each layer for each band must
 *  be passed in, if not, the burn vector set by the user will be
 *  duplicated to fit this condition.
 *
 *  The user can set an OutputPixelType as a burn vector via
 *  AddBurnValue(OutputImagePixelType burnValuesPix). To have a
 *  different color for each VectorData, use this method with
 *  differents burnValuesPix as many times as VectorDatas set.
 *  Again, the color will be duplicated if only one burnValuesPix
 *  is set.
 *
 *  Note : The VectorDatas and the input raster must be in the same
 *  projectionRef. Nothing is done in this class to reproject the
 *  VectorData into the image coordinate system.
 */
template <class TVectorData, class TInputImage, class TOutputImage = TInputImage  >
class  ITK_EXPORT RasterizeVectorDataFilter :
    public itk::CastImageFilter< TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef RasterizeVectorDataFilter                           Self;
  typedef itk::CastImageFilter<TInputImage, TOutputImage>     Superclass;
  typedef itk::SmartPointer< Self >                           Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Run-time type information (and related methods). */
  //itkTypeMacro(RasterizeVectorDataFilter, itk::InPlaceImageFilter);
  itkTypeMacro(RasterizeVectorDataFilter, itk::CastImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TInputImage                            InputImageType;
  typedef typename  InputImageType::ConstPointer InputImagePointer;
  typedef typename  InputImageType::RegionType   InputImageRegionType;
  typedef typename  InputImageType::PixelType    InputImagePixelType;
  typedef typename  InputImageType::IndexType    InputIndexType;
  typedef typename  InputImageType::PointType    InputPointType;

  typedef TOutputImage    OutputImageType;
  typedef typename  OutputImageType::Pointer     OutputImagePointer;
  typedef typename  OutputImageType::RegionType  OutputImageRegionType;
  typedef typename  OutputImageType::PixelType   OutputImagePixelType;
  typedef typename  OutputImageType::InternalPixelType   OutputImageInternalPixelType;

  /** VectorData typedefs*/
  typedef TVectorData                            VectorDataType;
  typedef typename VectorDataType::DataTreeType  DataTreeType;
  typedef typename DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename DataTreeType::Pointer         DataTreePointerType;
  typedef typename DataTreeType::ConstPointer    DataTreeConstPointerType;

  typedef itk::DataObject                        DataObjectType;

  const InputImageType * GetInput();
  const DataObjectType* GetInput(unsigned int idx);

  /** Add VectorData Input */
  virtual void AddVectorData(const VectorDataType* vd);

  /**
   * Set the burn values using the output pixel type; all the bands
   * will be burned
   **/
  void AddColor(const OutputImagePixelType& burnValuesPix)
  {
    // checking : If used several times, the user must always set
    // colors with the same size.
    unsigned int previousBandVectorSize = m_BandsToBurn.size();
    if (previousBandVectorSize != 0)
      {
      if (burnValuesPix.Size() != previousBandVectorSize)
        {
        itkExceptionMacro(<< "The color added does not have the same number of elements than the previous "
                          << "added one." << "( Previous color size : "<< previousBandVectorSize
                          << ", new one size :"<< burnValuesPix.Size() << ")");
        }
      }

    // Add the value stored in the output image pixel type to
    // the burn values vector
    // Add all the bands to be burned and clear previous bands.
    m_BandsToBurn.clear();
    for (unsigned int idx = 0; idx < burnValuesPix.Size(); ++idx)
      {
      m_BandsToBurn.push_back(idx + 1 ); // Gdal bands count begins from 1
      m_BurnValues.push_back(static_cast<double>(burnValuesPix.GetElement(idx)));
      }
  }

protected:
  virtual void GenerateData();

  RasterizeVectorDataFilter();
  virtual ~RasterizeVectorDataFilter()
  {
    if (m_OGRDataSourcePointer != NULL)
      {
      OGRDataSource::DestroyDataSource(m_OGRDataSourcePointer);
      }
  }

  virtual void GenerateOutputInformation();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RasterizeVectorDataFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OGRDataSource*              m_OGRDataSourcePointer;

  // Vector Of LayersH
  std::vector< OGRLayerH >    m_SrcDataSetLayers;

  std::vector<double>         m_BurnValues;
  std::vector<double>         m_FullBurnValues;
  std::vector<int>            m_BandsToBurn;

}; // end of class RasterizeVectorDataFilter

} // end of namespace otb


#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbRasterizeVectorDataFilter.txx"
#endif

#endif
