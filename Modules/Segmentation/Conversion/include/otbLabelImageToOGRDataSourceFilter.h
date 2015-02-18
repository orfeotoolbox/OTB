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
#ifndef __otbLabelImageToOGRDataSourceFilter_h
#define __otbLabelImageToOGRDataSourceFilter_h

#include "itkProcessObject.h"
#include "otbOGRDataSourceWrapper.h"

namespace otb
{

/** \class LabelImageToOGRDataSourceFilter
 *  this class uses \c GDALPolygonize() method to transform a Label image into
 *  a "memory" \c OGRDataSource. The layer in the DataSource is named "Layer".
 *  Each Feature of the layer has a Integer Field which name is specified by \c SetFieldName() (default is "DN").
 *  Label of the input image are written into this field.
 *  An optional input mask can be used to exclude pixels from vectorization process.
 *  All pixels with a value of 0 in the input mask image will not be suitable for vectorization.
 * \Note The Use8Connected parameter can be turn on and it will be used in \c GDALPolygonize(). But be carreful, it
 * can create cross polygons !
 * \Note It is a non-streamed version.
 * \ingroup OBIA
 *
 */

template <class TInputImage>
class ITK_EXPORT LabelImageToOGRDataSourceFilter :
    public itk::ProcessObject
{
public:

   /** typedef for the classes standards. */
  typedef LabelImageToOGRDataSourceFilter                 Self;
  typedef itk::ProcessObject                              Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LabelImageToOGRDataSourceFilter, ProcessObject);

  /** Definition of the input image */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef typename InputImageType::IndexType    InputIndexType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::RegionType   RegionType;
  typedef typename InputImageType::SpacingType  SpacingType;
  typedef typename InputImageType::PointType    OriginType;
  typedef typename InputImageType::IndexType    IndexType;

  typedef ogr::DataSource                            OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer        OGRDataSourcePointerType;
  typedef ogr::Layer                                 OGRLayerType;

  /** Set/Get the input image of this process object.  */
  virtual void SetInput(const InputImageType *input);
  virtual const InputImageType * GetInput(void);

  /** Set the input mask image.
   * All pixels in the mask with a value of 0 will not be considered
   * suitable for vectorization.
   */
  virtual void SetInputMask(const InputImageType *input);
  virtual const InputImageType * GetInputMask(void);

  /** Set the Field Name in which labels will be written. (default is "DN")
   * A field "FieldName" of type integer is created in the output memory layer.
   */
  itkSetMacro(FieldName, std::string);
  /**
   * Return the Field name in which labels have been written.
   */
  itkGetMacro(FieldName, std::string);

  /**
   * Set the value of 8-connected neighborhood option used in \c GDALPolygonize
   */
  itkSetMacro(Use8Connected, bool);
  /**
   * Get the value of 8-connected neighborhood option used in \c GDALPolygonize
   */
  itkGetMacro(Use8Connected, bool);

  /**
   * Get the output \c ogr::DataSource which is a "memory" datasource.
   */
  const OGRDataSourceType * GetOutput();

protected:
  LabelImageToOGRDataSourceFilter();
  virtual ~LabelImageToOGRDataSourceFilter() {}

  virtual void GenerateInputRequestedRegion();

  /** Generate Data method*/
  virtual void GenerateData();

  /** DataObject pointer */
  typedef itk::DataObject::Pointer DataObjectPointer;

  virtual DataObjectPointer MakeOutput(unsigned int idx);

private:
  LabelImageToOGRDataSourceFilter(const Self &);  //purposely not implemented
  void operator =(const Self&);      //purposely not implemented

  std::string m_FieldName;
  bool m_Use8Connected;


};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageToOGRDataSourceFilter.txx"
#endif

#endif
