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
namespace otb {
namespace ogr {
class DataSource;
class Layer;
} // ogr namespace
class GeometriesSet;
} // otb namespace

/**\defgroup GeometriesFilters
 * \ingroup  gGeometry Filters
 * Filters of geometries sets.
 */

namespace otb
{

/**\ingroup GeometriesFilters
 * \class GeometriesSource
 * Source of geometries & Root of the geometries filters hierarchy.
 * This class provides a kind of \em reader for geometries sets.
 * \todo: disable Graft
 * \since OTB v 3.14.0
 */
class ITK_EXPORT GeometriesSource : public itk::ProcessObject, boost::noncopyable
  {
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesSource                     Self;
  typedef itk::ProcessObject                   Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
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
  itkTypeMacro(GeometriesSource, itk::ProcessObject);
  //@}

  /**\name GeometriesSet Output property. */
  //@{
  /** Overriding \c GetOutput() functions */
  virtual OutputGeometriesType* GetOutput(void);
  /** Overriding \c GetOutput() functions */
  virtual OutputGeometriesType* GetOutput(unsigned int idx);

  virtual void SetOutput(OutputGeometriesType* output, unsigned int idx = 0);
  //@}

  /** Specialized hook for \c GeometriesSource.
   * Makes sure the output(s) is (/are) allocated before initializing them.
   */
  virtual void PrepareOutputs();

protected:
  /** Default constructor.
   * \post Required number of outputs = 1
   */
  GeometriesSource();
  /** Destructor.
   * Does nothing.
   */
  virtual ~GeometriesSource();

  /** Ensures that the output geometries are allocated before processing.
   * If the output hasn't been set, at this point, the default output geometries
   * set will an <em>in-memory</em> \c DataSource.
   * \post <tt>GetOutput() != NULL</tt>
   */
  virtual void  DoAllocateOutputs();
  };
} // end namespace otb

#endif // __otbGeometriesSource_h
