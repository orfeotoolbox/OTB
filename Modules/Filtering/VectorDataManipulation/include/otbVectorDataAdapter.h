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
#ifndef __otbVectorDataAdapter_h
#define __otbVectorDataAdapter_h

#include "otbVectorDataToVectorDataFilter.h"

namespace otb
{

/** \class VectorDataAdapter
  * \brief Helper class to convert the vector data to generic type
  *
 *
 * \ingroup OTBVectorDataManipulation
  */

template <class TInputVectorData, class TOutputVectorData>
class ITK_EXPORT VectorDataAdapter :
    public otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataAdapter                                             Self;
  typedef otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>                                                Pointer;
  typedef itk::SmartPointer<const Self>                                          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataAdapter, VectorDataToVectorDataFilter);

  typedef TInputVectorData                  InputVectorDataType;
  typedef TOutputVectorData                 OutputVectorDataType;

  typedef typename OutputVectorDataType::DataNodeType        OutputDataNodeType;
  typedef typename InputVectorDataType::DataNodeType         InputDataNodeType;

  typedef typename InputDataNodeType::PointType        InputPointType;
  typedef typename InputDataNodeType::LineType         InputLineType;
  typedef typename InputDataNodeType::PolygonType      InputPolygonType;
  typedef typename InputDataNodeType::PolygonListType  InputPolygonListType;

  typedef typename InputLineType::Pointer         InputLinePointerType;
  typedef typename InputPolygonType::Pointer      InputPolygonPointerType;
  typedef typename InputPolygonListType::Pointer  InputPolygonListPointerType;

  typedef typename OutputDataNodeType::PointType       OutputPointType;
  typedef typename OutputDataNodeType::LineType        OutputLineType;
  typedef typename OutputDataNodeType::PolygonType     OutputPolygonType;
  typedef typename OutputDataNodeType::PolygonListType OutputPolygonListType;

  typedef typename OutputLineType::Pointer         OutputLinePointerType;
  typedef typename OutputPolygonType::Pointer      OutputPolygonPointerType;
  typedef typename OutputPolygonListType::Pointer  OutputPolygonListPointerType;


protected:
  VectorDataAdapter() {};
  ~VectorDataAdapter() ITK_OVERRIDE {}

  OutputPointType ProcessPoint(InputPointType point) const ITK_OVERRIDE;
  OutputLinePointerType ProcessLine(InputLinePointerType line) const ITK_OVERRIDE;
  OutputPolygonPointerType ProcessPolygon(InputPolygonPointerType polygon) const ITK_OVERRIDE;
  OutputPolygonListPointerType ProcessPolygonList(InputPolygonListPointerType polygonList) const ITK_OVERRIDE;

private:
  VectorDataAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataAdapter.txx"
#endif

#endif
