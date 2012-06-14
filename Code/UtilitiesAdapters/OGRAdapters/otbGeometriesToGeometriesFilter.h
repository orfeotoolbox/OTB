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
  void Process(OutputGeometriesType &inout);
  void Process(InputGeometriesType const& source, OutputGeometriesType &destination);
  virtual void DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination) const=0;
  virtual OGRSpatialReference*     DoDefineNewLayerSpatialReference(ogr::Layer const& source) const { return 0; }
  virtual OGRwkbGeometryType       DoDefineNewLayerGeometryType(ogr::Layer const& source) const { return source.GetGeomType(); }
  virtual std::vector<std::string> DoDefineNewLayerOptions(ogr::Layer const& source) const { return std::vector<std::string>(); }
  virtual void FinalizeInitialisation() {}
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
  TransformationFunctorDispatcher() { }
  TransformationFunctorDispatcher(TransformationFunctor functor) : m_functor(functor){ }
  void operator()(ogr::Layer const& in, ogr::Layer & out) const
    {
    m_functor(in, out);
    }
  void operator()(ogr::Layer & inout) const
    {
    m_functor(inout);
    }
  TransformationFunctor * operator->() { return &m_functor; }
private:
  TransformationFunctor m_functor;
  };


template <class TransformationFunctor>
struct TransformationFunctorDispatcher<TransformationFunctor, OGRGeometry>
  {
  typedef typename TransformationFunctor::TransformedElementType TransformedElementType;
  BOOST_MPL_ASSERT((boost::is_same<OGRGeometry, TransformedElementType>));
  TransformationFunctorDispatcher() { }
  TransformationFunctorDispatcher(TransformationFunctor functor) : m_functor(functor){ }

  void operator()(ogr::Layer const& in, ogr::Layer & out) const
    {
    // std::cout << "Converting layer " << in.GetName() << " -> " << out.GetName() << "\n";
    OGRFeatureDefn & defn = out.GetLayerDefn();
    for (ogr::Layer::const_iterator b = in.begin(), e = in.end(); b != e; ++b)
      {
      ogr::Feature const feat = *b;
      // TODO: field transformations...
      ogr::UniqueGeometryPtr g = m_functor(feat.GetGeometry());
      ogr::Feature dest(defn);
      dest.SetGeometryDirectly(boost::move(g));
      out.CreateFeature(dest);
      }
    }

  void operator()(ogr::Layer & inout) const
    {
    // std::cout << "Converting layer " << inout.GetName() << "\n";
    OGRFeatureDefn & defn = inout.GetLayerDefn();
    // NB: We can't iterate with begin()/end() as SetFeature may invalidate the
    // iterators depending of the underlying drivers
    // => we use start_at(), i.e. SetNextByIndex()
    for (int i=0, N=inout.GetFeatureCount(true); i!=N; ++i)
      {
      ogr::Feature feat = *inout.start_at(i);
      // TODO: field transformations...
      ogr::UniqueGeometryPtr g = m_functor(feat.GetGeometry());
      feat.SetGeometryDirectly(boost::move(g));
      inout.SetFeature(feat);
      }
    }
  TransformationFunctor * operator->() { return &m_functor; }
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

  virtual void DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination) const
    {
    // std::cout << "DG2GF::DoProcessLayer: L("<<source.GetName()<<") -> L("<<destination.GetName()<<") ...\n";
    if (source != destination)
      {
      m_TransformationFunctor(source, destination); // if TransformedElementType == layer
      }
    else
      {
      m_TransformationFunctor(destination); // if TransformedElementType == layer
      }
    };
private:
  TransformationFunctorDispatcherType m_TransformationFunctor;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeometriesToGeometriesFilter.txx"
#endif

#endif // __otbGeometriesToGeometriesFilter_h
