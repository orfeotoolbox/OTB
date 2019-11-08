/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbGeometriesSource_h
#define otbGeometriesSource_h

#include <boost/noncopyable.hpp>
#include "itkProcessObject.h"
#include "itkMacro.h"

#include "OTBGdalAdaptersExport.h"

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

/**\defgroup GeometriesFilters Filters of geometries sets
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
 *
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT GeometriesSource : public itk::ProcessObject, boost::noncopyable
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesSource                                   Self;
  typedef itk::ProcessObject                                 Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
  //@}

  /**\name I/O typedefs */
  //@{
  typedef GeometriesSet OutputGeometriesType;
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
  virtual OutputGeometriesType* GetOutput(DataObjectPointerArraySizeType idx);

  /**
   * Output mutator.
   * \param[in,out] output  \c GeometriesSource where output result will be
   * written.
   * \param[in] idx  index of the output assigned to the \c GeometriesSource.
   *
   * \throw std::bad_alloc if no new output can be added.
   */
  virtual void SetOutput(OutputGeometriesType* output, DataObjectPointerArraySizeType idx = 0);
  using Superclass::SetOutput;
  //@}

  /** \c ProcessObject hook Specialized for \c GeometriesSource.
   * This function makes sure the output(s) is (/are) allocated before
   * initializing them.
   * \post <tt>GetOutput() != NULL</tt>
   */
  void PrepareOutputs() override;

protected:
  /** Default constructor.
   * \post Required number of outputs == 1
   */
  GeometriesSource();
  /** Destructor.
   * Does nothing.
   */
  ~GeometriesSource() override;

  /** Ensures that the output geometries are allocated before processing.
   * If the output hasn't been set, at this point, the default output geometries
   * set will be an <em>in-memory</em> \c ogr::DataSource.
   * \post <tt>GetOutput() != NULL</tt>
   */
  virtual void DoAllocateOutputs();
};
} // end namespace otb

#endif // otbGeometriesSource_h
