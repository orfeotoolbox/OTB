/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorDataToLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/02/18 20:03:03 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkVectorDataToLabelMapFilter_h
#define __itkVectorDataToLabelMapFilter_h

#include "itkLabelObject.h"
#include "itkProgressReporter.h"
#include "itkBarrier.h"
#include "itkConceptChecking.h"
//#include "itkDataObject.h"

#include "otbLabelMapSource.h"
#include "otbVectorData.h"

#include <vector>
#include <map>

// #include "itkImageToImageFilter.h"


namespace otb
{

/**
 * \class VectorDataToLabelMapFilter
 * \brief Convert a vector data and produce a collection of label objects for each node
 *
 * VectorDataToLabelMapFilter convert a vector data in a LabelMap.
 * Each distinct object is assigned a unique label. 
 * The final object labels start with 1 and are consecutive. Objects
 * that are reached earlier by a raster order scan have a lower
 * label.
 *
 * \author Manuel GRIZONNET, CNES, France.
 *
 * \sa LabelMapSource
 */

template <class TVectorData, class TLabelMap >
class ITK_EXPORT VectorDataToLabelMapFilter : 
    public LabelMapSource < TLabelMap >
//     public itk::ImageToImageFilter<TLabelMap, TLabelMap>

{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef VectorDataToLabelMapFilter                   Self;
  typedef LabelMapSource < TLabelMap > Superclass;
//   typedef itk::ImageToImageFilter<TLabelMap, TLabelMap> Superclass;
  
  /** Some convenient typedefs. */
  typedef TVectorData InputVectorDataType;
  typedef TLabelMap OutputLabelMapType;
  typedef typename InputVectorDataType::Pointer                       InputVectorDataPointer;
  typedef typename InputVectorDataType::ConstPointer                  InputVectorDataConstPointer;
  
  typedef typename OutputLabelMapType::Pointer                        OutputLabelMapPointer;
  typedef typename OutputLabelMapType::ConstPointer                   OutputLabelMapConstPointer;
  
  typedef typename InputVectorDataType::DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType             ChildrenListType;
  typedef typename InputVectorDataType::DataNodeType                  DataNodeType;
  typedef typename DataNodeType::Pointer                              DataNodePointerType;
  
  typedef typename OutputLabelMapType::LabelType                               LabelType;
  
  //typedef typename InputVectorDataType::VDimension                             VDimension;
  typedef typename OutputLabelMapType::IndexType                               IndexType;
  typedef typename OutputLabelMapType::PixelType       OutputLabelMapPixelType;
  typedef typename OutputLabelMapType::PointType       OriginType;
  
  /** Number of dimensions. */
  itkStaticConstMacro(VectorDataDimension, unsigned int,
                      TVectorData::Dimension);
  
  /** Image size typedef. */
  typedef itk::Size<itkGetStaticConstMacro(VectorDataDimension)> SizeType;
  //typedef typename IndexType::Pointer                                 IndexPointerType;
  
  typedef typename InputVectorDataType::PointType                     PointType;
  //typedef typename PointType::Pointer                                 PointPointerType
  /** 
   * Smart pointer typedef support 
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  
  /** Set the size of the output image. */
  itkSetMacro( Size, SizeType );
  
  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(VectorDataToLabelMapFilter, ImageToImageFilter);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputLabelMapPixelType);
  itkGetConstMacro(BackgroundValue, OutputLabelMapPixelType);
  
  
  /** Set/Get the Vector data input of this process object.  */
  virtual void SetInput( const InputVectorDataType *input);
  virtual void SetInput( unsigned int idx, const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);
  const InputVectorDataType * GetInput(unsigned int idx);
  
  virtual void GenerateOutputInformation();
  
protected:
  VectorDataToLabelMapFilter(); 
  ~VectorDataToLabelMapFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /**
   * Standard pipeline method. 
   */
  
  
  void GenerateData();

  /** VectorDataToLabelMapFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** VectorDataToLabelMapFilter will produce all of the output.
   * Therefore it must provide an implementation of
   * EnlargeOutputRequestedRegion().
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  //void EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output));

  
  
private:
  VectorDataToLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void ProcessNode(InternalTreeNodeType * source);
  
  /** Current label value incremented during vectorization */
  LabelType m_lab;
  
  //OriginType          m_Origin;
  SizeType            m_Size;
  IndexType           m_StartIndex;
  
  /** Background value, not use presently background value=itk::NumericTraits<LabelType>::max()*/
  OutputLabelMapPixelType m_BackgroundValue;
  };
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#if !defined(CABLE_CONFIGURATION)
#include "otbVectorDataToLabelMapFilter.txx"
#endif
#endif

#endif
