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
#ifndef __otbVectorDataExtractROI_h
#define __otbVectorDataExtractROI_h

#include "otbVectorDataToVectorDataFilter.h"
#include "itkMacro.h"

namespace otb
{

/** \class VectorDataExtractROI
 * \brief Extract a subset of a Vector Data.
 *
 * \note Parameter to this class for input and outputs are vectorData
 *
 */
template <class TVectorData>
class ITK_EXPORT VectorDataExtractROI:
      public VectorDataToVectorDataFilter<TVectorData ,TVectorData >
{
public:
  /** Standard class typedefs. */
  typedef VectorDataExtractROI                                     Self;
  typedef VectorDataToVectorDataFilter<TVectorData ,TVectorData >  Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataExtractROI,VectorDataToVectorDataFilter);

  /** Image type information. */
  typedef TVectorData                                        VectorDataType;
  typedef typename VectorDataType::DataTreeType               DataTreeType;

  /** Get/Set Macro for ROI Column size */
  itkGetMacro(SizeX, unsigned int);
  itkSetMacro(SizeX, unsigned int);

  /** Get/Set Macro for ROI Lines size */
  itkGetMacro(SizeY, unsigned int);
  itkSetMacro(SizeY, unsigned int);

  /** Get/Set Macro for ROI Start Point Coordinate */
  itkGetMacro(StartX, unsigned int);
  itkSetMacro(StartX, unsigned int);

  /** Get/Set Macro for  ROI Start Point Abcisse */
  itkGetMacro(StartY, unsigned int);
  itkSetMacro(StartY, unsigned int);


  /** Prototype of the generate data method*/
  void GenerateData(void );




protected:
  VectorDataExtractROI();
  ~VectorDataExtractROI() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** VectorDataExtractROI
   *
   * \sa VectorDataExtractROIBase::GenerateOutputInformaton()  */
  //virtual void GenerateOutputInformation();

private:
  VectorDataExtractROI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_SizeX;
  unsigned int m_SizeY;
  unsigned int m_StartX;
  unsigned int m_StartY;


};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataExtractROI.txx"
#endif

#endif
