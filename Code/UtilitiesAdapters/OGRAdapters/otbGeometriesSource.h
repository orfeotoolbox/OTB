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
#ifndef __otbGeometriesSource_h
#define __otbGeometriesSource_h

#include <boost/noncopyable.hpp>
#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"

// Forward declarations
namespace otb
{
namespace ogr
{
class DataSource;
class Layer;
} // ogr namespace
class GeometriesSet;
} // otb namespace


namespace otb
{

// todo: disable graft
class ITK_EXPORT GeometriesSource : public itk::ProcessObject, boost::noncopyable
  {
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesSource              Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  /**\name I/O typedefs */
  //@{
  typedef GeometriesSet                        OutputGeometriesType;
  // typedef GeometriesSet::Pointer               OutputGeometriesPointer;
  //@}


  /**\name Standard macros */
  //@{
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataSource, itk::ProcessObject);
  //@}

  /** Overriding \c GetOutput() functions */
  virtual OutputGeometriesType* GetOutput(void);
  /** Overriding \c GetOutput() functions */
  virtual OutputGeometriesType* GetOutput(unsigned int idx);

  virtual void SetOutput(OutputGeometriesType* output, unsigned int idx = 0);
  virtual void PrepareOutputs();

protected:
  GeometriesSource();
  virtual ~GeometriesSource();

  /** Ensures that the output vector data are cleared before processing. */
  virtual void  AllocateOutputs();
  };
} // end namespace otb

// #ifndef OTB_MANUAL_INSTANTIATION
// #include "otbGeometriesSource.txx"
// #endif

#endif // __otbGeometriesSource_h
