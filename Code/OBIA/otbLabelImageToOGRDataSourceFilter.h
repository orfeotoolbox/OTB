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
#include "itkDataObjectDecorator.h"
#include "itkDataObject.h"

namespace otb
{

class OGRDataSourceWrapper;

/** \class LabelImageToOGRDataSourceFilter
 *  \brief this class uses GDALPolygonize method to transform a Label image into
 *         a OGRDataSource. It is a non-streamed version.
 *
 *
 *  \sa StreamingLabelImageToOGRDataSourceFilter (streamed version)
 *  \ingroup OBIA
 *
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
  
  typedef itk::DataObjectDecorator<OGRDataSourceWrapper> OGRDataSourceObjectType;
  
  /** Set/Get the input image of this process object.  */
  virtual void SetInput(const InputImageType *input);
  virtual const InputImageType * GetInput(void);
  
  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);
  
  const OGRDataSourceObjectType * GetOutput();
  
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


};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageToOGRDataSourceFilter.txx"
#endif

#endif
