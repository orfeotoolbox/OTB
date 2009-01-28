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
#ifndef __otbPointSetFunction_h
#define __otbPointSetFunction_h

#include "itkSpatialFunction.h"
#include "itkPoint.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 * \class PointSetFunction
 * \brief Calculate the density in the neighborhood of a pixel
 *
 * \ingroup SpatialFunctions
 */
template <class TPointSet, class  TOutput>
 class ITK_EXPORT PointSetFunction :
  public itk::SpatialFunction<TOutput >
{
public:
  /** Standard class typedefs. */
  typedef PointSetFunction                    Self;
  typedef itk::SpatialFunction< TOutput >             Superclass;
    
  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetFunction, itk::SpatialFunction);
  
  /** PointSet Type typedef Support*/
  typedef TPointSet                            PointSetType;
  typedef typename  PointSetType::Pointer      PointSetPointerType;
  
  /** TOutput typedef suppoty*/
  typedef TOutput           OutputType;
  
  /** Set the input image (reimplemented since we need to set the detector input) */
  virtual void SetPointSet(PointSetType* PointSet);
  virtual PointSetType * GetPointSet();

 
protected:
  PointSetFunction();
  ~PointSetFunction(){};

  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  

private:
  PointSetFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  PointSetPointerType  m_PointSet;

};

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION 
#include "otbPointSetFunction.txx"
#endif

#endif

