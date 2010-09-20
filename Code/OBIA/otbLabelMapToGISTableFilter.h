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
#ifndef __otbLabelMapToGISTableFilter_h
#define __otbLabelMapToGISTableFilter_h

#include "otbGISTableSource.h"
#include "otbGISConnectionImplementation.h"
#include "otbLabelObjectToPolygonFunctor.h"

#include "otbCorrectPolygonFunctor.h"
#include "itkLabelMap.h"
#include <iostream>
#include <sstream>
#include <string>

namespace otb {

/** \class LabelMapToGISTableFilter
 * \brief Convert a LabelMap to a GIS table
 *
 * LabelMapToGISTableFilter converts  a
 * LabelMap to GIS Table (PostGIS...).
 * Create first a PostGIS table (by default name=labelmap_to_gis_sample)
 * The table is not drop if it already exist
 * Structure of the PostGIS table:
   * labelmap_to_gis_sample (id serial, the_geom geometry)
 * with srid = -1
 * polygons are handle yet
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TLabelMap, class TGISTable>
class ITK_EXPORT LabelMapToGISTableFilter :
  public GISTableSource<TGISTable>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToGISTableFilter      Self;
  typedef GISTableSource<TGISTable>     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TLabelMap                                 InputLabelMapType;
  typedef TGISTable                                 OutputGISTableType;
  typedef typename InputLabelMapType::Pointer       InputLabelMapPointer;
  typedef typename InputLabelMapType::ConstPointer  InputLabelMapConstPointer;
  typedef typename OutputGISTableType::Pointer      OutputGISTablePointer;
  typedef typename OutputGISTableType::ConstPointer OutputGISTableConstPointer;

  //typedef typename InputLabelMapType::DataTreeType::TreeNodeType    InternalTreeNodeType;
  //typedef typename InternalTreeNodeType::ChildrenListType        ChildrenListType;
  //typedef typename InputLabelMapType::DataNodeType  DataNodeType;
  //typedef typename DataNodeType::Pointer         DataNodePointerType;
  typedef typename InputLabelMapType::LabelObjectType LabelObjectType;

  typedef typename OutputGISTableType::ConnectionType InputGISConnectionType;
  typedef typename InputGISConnectionType::Pointer    InputGISConnectionPointerType;

  typedef typename OutputGISTableType::PolygonType PolygonType;
  typedef typename PolygonType::Pointer            PolygonPointerType;

  typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType, PolygonType> FunctorType;
  typedef otb::SimplifyPathFunctor<PolygonType, PolygonType>                      SimplifyFunctorType;
  typedef ClosePathFunctor <PolygonType, PolygonType>                             CloseFunctorType;
  typedef CorrectPolygonFunctor <PolygonType>                                     CorrectFunctorType;
  /** ImageDimension constants */
  //itkStaticConstMacro(InputImageDimension, unsigned int,
  //                    TInputImage::ImageDimension);
  //itkStaticConstMacro(OutputImageDimension, unsigned int,
  //                    TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  //itkTypeMacro(LabelMapToAttributeImageFilter,
  //           ImageToImageFilter);

  /**
   * Set/Get the boolean value if you do not want to create the GIS table
   */
  itkSetMacro(DropExistingGISTable, bool);
  itkGetConstMacro(DropExistingGISTable, bool);

  itkSetMacro(GISTableName, std::string);
  itkGetConstMacro(GISTableName, std::string);

  itkGetObjectMacro(InputGISConnection, InputGISConnectionType);
  itkSetObjectMacro(InputGISConnection, InputGISConnectionType);

  //void setConnection

  /** Set/Get the LabelMap input of this process object.  */
  virtual void SetInput(const InputLabelMapType *input);
  virtual void SetInput(unsigned int idx, const InputLabelMapType *input);
  const InputLabelMapType * GetInput(void);
  const InputLabelMapType * GetInput(unsigned int idx);

protected:
  LabelMapToGISTableFilter();
  virtual ~LabelMapToGISTableFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LabelMapToAttributeImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** LabelMapToAttributeImageFilter will produce the entire output. */
  //void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();

private:
  LabelMapToGISTableFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Remove table before insertion if true  */
  bool m_DropExistingGISTable;

  /** Connection parameters to the db  */
  InputGISConnectionPointerType m_InputGISConnection;
  /** GIS table name  */
  std::string m_GISTableName;
}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapToGISTableFilter.txx"
#endif

#endif
