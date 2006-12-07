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
#ifndef _otbImageMultiSegmentationToRCC8GraphFilter_h
#define _otbImageMultiSegmentationToRCC8GraphFilter_h

#include "otbImageListToRCC8GraphFilter.h"

namespace otb
{
/**
 * \class ImageMultiSegmentationToRCC8GraphFilter
 * \brief This class takes a list of labelled segmentation images 
 * and build the RCC8 graph of the set of regions it represents.
 */
template <class TInputImage, class TOutputGraph>
class ImageMultiSegmentationToRCC8GraphFilter 
  : public ImageListToRCC8GraphFilter<TInputImage,TOutputGraph>
{
public:
  /** Standard class typedefs. */
  typedef ImageMultiSegmentationToRCC8GraphFilter               Self;
  typedef ImageListToRCC8GraphFilter<TInputImage, TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMultiSegmentationToRCC8GraphFilter,ImageListToRCC8GraphFilter);
  /** Input related typedefs */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  /** Output related typedefs */
  typedef TOutputGraph OutputGraphType;
  typedef typename OutputGraphType::Pointer OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType VertexType;
  typedef typename VertexType::Pointer VertexPointerType;
  
/*   /// Get The statistics for the different relations */
/*   itkGetConstMacro(TotalNumberOfRegions,int); */
/*   itkGetConstMacro(NumberOfRelations,int); */
/*   /// Get the number of regions by segmentation image */
/*   std::vector<int> GetNumberOfRegions(void); */
/*   /// Set a filter to not take in account relations whose index is */
/*   /// under the threshold */
/*   itkGetConstMacro(RelationFilter,int); */
/*   itkSetMacro(RelationFilter,int);       */          

protected:
  /** Constructor */
  ImageMultiSegmentationToRCC8GraphFilter();
  /** Destructor */
  ~ImageMultiSegmentationToRCC8GraphFilter();
  /** Main computation method */
  virtual void GenerateData();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
 /*  int m_NumberOfRelations; */
/*   int m_TotalNumberOfRegions; */
/*   std::vector<unsigned int> m_NumberOfRegions; */
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageMultiSegmentationToRCC8GraphFilter.txx"
#endif

#endif
