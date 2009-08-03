/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToAttributeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorDataToGISTableFilter_h
#define __otbVectorDataToGISTableFilter_h

//#include "itkImageToImageFilter.h"
//#include "itkAttributeLabelObject.h"
#include "otbGISTableSource.h"
//#include "otbGISTable.h"
//#include "otbVectorData.h"
#include <iostream>
#include <sstream>
#include <string>

namespace otb {

/** \class VectorDataToGISTableFilter
 * \brief Convert a LabelMap to a VectorData
 *
   * VectorDataToGISTableFilter converts  a
   * VectorData to GIS Table (PostGIS...).
 *
 * \author Manuel GRIZONNET. CNES, France.
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TVectorData , class TGISTable>
class ITK_EXPORT VectorDataToGISTableFilter : 
    public GISTableSource< TGISTable >
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToGISTableFilter Self;
  typedef GISTableSource< TGISTable >
  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TVectorData InputVectorDataType;
  typedef TGISTable OutputGISTableType;
  typedef typename InputVectorDataType::Pointer         InputVectorDataPointer;
  typedef typename InputVectorDataType::ConstPointer    InputVectorDataConstPointer;
  //typedef typename InputImageType::RegionType      InputImageRegionType;
  //typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputGISTableType::Pointer        OutputGISTablePointer;
  typedef typename OutputGISTableType::ConstPointer   OutputGISTableConstPointer;
  
  typedef typename InputVectorDataType::DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType        ChildrenListType;
  //typedef typename OutputVectorDataType::RegionType     OutputVectorDataRegionType;
  //typedef typename OutputVectorDataType::PixelType      OutputVectorDataPixelType;
  //typedef typename OutputVectorDataType::IndexType      IndexType;
  typedef typename InputVectorDataType::DataNodeType  DataNodeType;
  typedef typename DataNodeType::Pointer         DataNodePointerType;
  
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
  itkSetMacro(CreateGISTable, bool);
  itkGetConstMacro(CreateGISTable, bool);
  
  /**
   * Set/Get the value of the SQL insert command 
   */
  //itkSetMacro(InsertCmd, std::stringstream);
  //itkGetConstMacro(InsertCmd, std::stringstream);

  /** Set/Get the LabelMap input of this process object.  */
  virtual void SetInput( const InputVectorDataType *input);
  virtual void SetInput( unsigned int idx, const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);
  const InputVectorDataType * GetInput(unsigned int idx);
  
protected:
  VectorDataToGISTableFilter();
  ~VectorDataToGISTableFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LabelMapToAttributeImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelMapToAttributeImageFilter will produce the entire output. */
  //void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  
private:
  VectorDataToGISTableFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  
  void ProcessNode(InternalTreeNodeType * source);
  bool m_CreateGISTable;
  
  std::stringstream m_InsertCmd;
  
  
} ; // end of class

} // end namespace otb
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "otbVectorDataToGISTableFilter.txx"
#endif

#endif


