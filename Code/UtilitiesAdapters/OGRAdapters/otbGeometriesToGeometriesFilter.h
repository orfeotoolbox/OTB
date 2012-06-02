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
#ifndef __otbGeometriesToGeometriesFilter_h
#define __otbGeometriesToGeometriesFilter_h

#include "otbGeometriesSource.h"
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "otbOGRFeatureWrapper.h"
#include "otbOGRLayerWrapper.h"

struct ProcessVisitor;
namespace otb
{
class ITK_EXPORT GeometriesToGeometriesFilter : public GeometriesSource
  {
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesToGeometriesFilter  Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  /**\name I/O typedefs */
  //@{
  typedef GeometriesSet                        InputGeometriesType;
  // typedef GeometriesSet::Pointer               InputGeometriesPointer;
  typedef GeometriesSet                        OutputGeometriesType;
  // typedef GeometriesSet::Pointer               OutputGeometriesPointer;
  //@}

  /**\name Standard macros */
  //@{
  /** Method for creation through the object factory. */
  // itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometriesToGeometriesFilter, GeometriesSource);
  //@}

  virtual void SetInput(const InputGeometriesType *input);
  const InputGeometriesType * GetInput(void);

protected:
  /** Constructor */
  GeometriesToGeometriesFilter();
  /** Destructor */
  virtual ~GeometriesToGeometriesFilter();

  virtual void GenerateOutputInformation(void);
  virtual void GenerateData(void);

private:
  void Process(
    InputGeometriesType const& source, OutputGeometriesType &destination);
  virtual void DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination) const=0;
  friend struct ::ProcessVisitor;
  };


template <class TransformationFunctor, class TransformedElementType>
struct TransformationFunctorDispatcher
  {
  };

template <class TransformationFunctor>
struct TransformationFunctorDispatcher<TransformationFunctor, ogr::Layer>
  {
  typedef typename TransformationFunctor::TransformedElementType TransformedElementType;
  BOOST_MPL_ASSERT((boost::is_same<ogr::Layer, TransformedElementType>));
  TransformationFunctorDispatcher(TransformationFunctor functor) : m_functor(functor){ }
  void operator()(ogr::Layer const& in, ogr::Layer & out) const
    {
    m_functor(in, out);
    }
private:
  TransformationFunctor m_functor;
  };


template <class TransformationFunctor>
struct TransformationFunctorDispatcher<TransformationFunctor, OGRGeometry>
  {
  typedef typename TransformationFunctor::TransformedElementType TransformedElementType;
  BOOST_MPL_ASSERT((boost::is_same<OGRGeometry, TransformedElementType>));
  TransformationFunctorDispatcher(TransformationFunctor functor) : m_functor(functor){ }
  void operator()(ogr::Layer const& in, ogr::Layer & out) const
    {
    OGRFeatureDefn & defn = out.GetLayerDefn();
    for (ogr::Layer::const_iterator b = in.begin(), e = in.end()
    ; b != e
    ; ++b
    )
      {
      ogr::Feature const feat = *b;
      ogr::UniqueGeometryPtr g = m_functor(feat.GetGeometry());
      // TODO: field transformations...
      ogr::Feature dest(defn);
      dest.SetGeometryDirectly(boost::move(g));
      out.CreateFeature(dest);
      }
    }
private:
  TransformationFunctor m_functor;
  };


template <class TransformationFunctor>
class ITK_EXPORT DefaultGeometriesToGeometriesFilter : public GeometriesToGeometriesFilter
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef DefaultGeometriesToGeometriesFilter                    Self;
  typedef GeometriesToGeometriesFilter                           Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;
  //@}
  /**\name Class typedefs */
  //@{
  typedef TransformationFunctor                                  TransformationFunctorType;
  typedef typename TransformationFunctor::TransformedElementType TransformedElementType;
  typedef TransformationFunctorDispatcher<TransformationFunctorType, TransformedElementType>
                                                                 TransformationFunctorDispatcherType;
  //@}

  /**\name Standard macros */
  //@{
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DefaultGeometriesToGeometriesFilter, GeometriesToGeometriesFilter);
  //@}

protected:
  /** Default constructor. */
  DefaultGeometriesToGeometriesFilter();
  /** Destructor. */
  virtual ~DefaultGeometriesToGeometriesFilter();

  /** Prints self to stream. */
  // void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination) const
    {
    m_TransformationFunctor(source, destination); // if TransformedElementType == layer
    };
private:
  TransformationFunctorDispatcherType m_TransformationFunctor;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeometriesToGeometriesFilter.txx"
#endif

#endif // __otbGeometriesToGeometriesFilter_h
