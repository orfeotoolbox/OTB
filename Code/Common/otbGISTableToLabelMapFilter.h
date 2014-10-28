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
#ifndef __otbGISTableToLabelMapFilter_h
#define __otbGISTableToLabelMapFilter_h

//#include "itkImageToImageFilter.h"
#include "otbLabelMapSource.h"
//include "otbLabelMap.h"
#include "otbVectorData.h"
#include "otbGISTableToVectorDataFilter.h"
#include "otbVectorDataToLabelMapFilter.h"

namespace otb {

/** \class GISTableToLabelMapFilter
 * \brief Convert a LabelMap to a LabelMap
 *
 * GISTableToLabelMapFilter converts a GIS Table (PostGIS...) to a
 * LabelMap.
 * The translation is delegate to the SHPLabelMapIO class (OGRLabelMapIO)
 * which calls the OGR driver to translate GIS layer to a LabelMap structure
 *
 * \sa LabelMapSource
 * \ingroup Common, GeospatialAnalysis
 */
template<class TGISTable, class TLabelMap>
class ITK_EXPORT GISTableToLabelMapFilter :
  public LabelMapSource<TLabelMap>
{
public:
  /** Standard class typedefs. */
  typedef GISTableToLabelMapFilter Self;
  typedef LabelMapSource<TLabelMap>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /**typedefs related to the GISTableToVectorDataFilter*/
  typedef VectorData <typename TGISTable::PrecisionType,  TGISTable::Dimension> TVectorData;
  typedef GISTableToVectorDataFilter<TGISTable, TVectorData>                    GISTableToVectorDataFilterType;
  typedef VectorDataToLabelMapFilter<TVectorData, TLabelMap>                    VectorDataToLabelMapFilterType;
  /** Some convenient typedefs. */
  typedef TGISTable                                 InputGISTableType;
  typedef TLabelMap                                 OutputLabelMapType;
  typedef typename OutputLabelMapType::Pointer      OutputLabelMapPointer;
  typedef typename OutputLabelMapType::ConstPointer OutputLabelMapConstPointer;

  typedef typename InputGISTableType::ConnectionType        ConnectionType;
  typedef typename InputGISTableType::ConnectionPointerType ConnectionPointerType;

  typedef typename OutputLabelMapType::PixelType OutputLabelMapPixelType;
  typedef typename OutputLabelMapType::IndexType IndexType;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(GISTableToLabelMapFilter,
               LabelMapSource);

  /** Image size typedef. */
  typedef itk::Size<TGISTable::Dimension> SizeType;
  //typedef typename IndexType::Pointer                                 IndexPointerType;

  typedef typename InputGISTableType::PointType PointType;

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputLabelMapPixelType);
  itkGetConstMacro(BackgroundValue, OutputLabelMapPixelType);

  /** Set the size of the output image. */
  itkSetMacro(Size, SizeType);

  /** Set the starting index of the output image. */
  itkSetMacro(StartIndex, IndexType);

  /** Set/Get the GISTable input of this process object.  */
  virtual void SetInput(const InputGISTableType *input);
  virtual void SetInput(unsigned int idx, const InputGISTableType *input);
  const InputGISTableType * GetInput(void);
  const InputGISTableType * GetInput(unsigned int idx);

  virtual void GenerateOutputInformation();

protected:
  GISTableToLabelMapFilter();
  virtual ~GISTableToLabelMapFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** GISTableToLabelMapFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** Single-threaded version of GenerateData.  This filter delegates
   * to the OGR driver. */
  void GenerateData();

private:
  GISTableToLabelMapFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename GISTableToVectorDataFilterType::Pointer m_GISTableToVectorDataFilter;
  typename VectorDataToLabelMapFilterType::Pointer m_VectorDataToLabelMapFilter;

  OutputLabelMapPixelType m_BackgroundValue;

  SizeType  m_Size;
  IndexType m_StartIndex;
}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGISTableToLabelMapFilter.txx"
#endif

#endif
