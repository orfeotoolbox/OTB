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
 *
 * Unlike others filters, the \em output has to be built from outside and given
 * to the \c GeometriesSource. If no \em output has been defined at the moment
 * the filter begins processing, by default the \c ogr::DataSource will have
 * have an \em in-memory storage.
 *
 * \internal
 * This particular behaviour is the consequence that, to exist, a \c
 * ogr::DataSource needs to know how it is stored. We could have permitted to
 * work on an \em in-memory \c ogr::DataSource, and then store the result into
 * another \em on-disk \c ogr::DataSource. This wouldn't have been efficient at
 * all. Hence the design adopted.
 *
 * \note As OGR data sources don't support multiple re-rentrant access, a \c
 * ogr::DataSource can't be grafted, and as a consequence, \c GeometriesSource
 * subtypes can't be grafted as well.
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

  /**
   * Output mutator.
   * \param[in,out] output  \c GeometriesSource where output result will be
   * written.
   * \param[in] idx  index of the output assigned to the \c GeometriesSource.
   *
   * \throw std::bad_alloc if no new output can be added.
   */
  virtual void SetOutput(OutputGeometriesType* output, unsigned int idx = 0);
  //@}

  /** \c ProcessObject hook Specialized for \c GeometriesSource.
   * This function makes sure the output(s) is (/are) allocated before
   * initializing them.
   * \post <tt>GetOutput() != NULL</tt>
   */
  virtual void PrepareOutputs();

protected:
  /** Default constructor.
   * \post Required number of outputs == 1
   */
  GeometriesSource();
  /** Destructor.
   * Does nothing.
   */
  virtual ~GeometriesSource();

  /** Ensures that the output geometries are allocated before processing.
   * If the output hasn't been set, at this point, the default output geometries
   * set will be an <em>in-memory</em> \c ogr::DataSource.
   * \post <tt>GetOutput() != NULL</tt>
   */
  virtual void  DoAllocateOutputs();
  };
} // end namespace otb

#endif // __otbGeometriesSource_h
