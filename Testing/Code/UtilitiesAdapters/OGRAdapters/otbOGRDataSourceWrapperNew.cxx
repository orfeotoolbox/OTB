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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#define BOOST_TEST_MODULE "otb::org::DataSource creation unit testing"
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "otbOGRDataSourceWrapper.h"


using namespace otb;

/**\ingroup gGeometry
 * \file    otbOGRDataSourceWrapperNew.cxx
 *
 *\todo Test: CopyLayer from one datasource to another
 *\todo Test: read datasource
 *\todo Test: write datasource
 */

/*===========================================================================*/
/*===============================[ Constants ]===============================*/
/*===========================================================================*/
const std::string k_one = "one";
const std::string k_two = "two";

/*const*/ OGRFieldDefn k_f0_("OFTInteger", OFTInteger);
/*const*/ OGRFieldDefn k_f1_("OFTReal", OFTReal);
/*const*/ OGRFieldDefn k_f2_("OFTString", OFTString);
/*const*/ OGRFieldDefn k_f3_("OFTIntegerList", OFTIntegerList);
/*const*/ OGRFieldDefn k_f4_("OFTRealList", OFTRealList);
/*const*/ OGRFieldDefn k_f5_("OFTStringList", OFTStringList);

ogr::FieldDefn k_f0(k_f0_);
ogr::FieldDefn k_f1(k_f1_);
ogr::FieldDefn k_f2(k_f2_);
ogr::FieldDefn k_f3(k_f3_);
ogr::FieldDefn k_f4(k_f4_);
ogr::FieldDefn k_f5(k_f5_);

/*===========================================================================*/
/*==================================[ UTs ]==================================*/
/*===========================================================================*/

BOOST_AUTO_TEST_CASE(OGRDataSource_new_empty)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  BOOST_ASSERT(ds);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 0);
  BOOST_CHECK_EQUAL(ds->Size(false), 0);
  BOOST_CHECK_EQUAL(ds->Size(true), 0);
  BOOST_CHECK_THROW(ds->GetLayerChecked(0), itk::ExceptionObject);
}

BOOST_AUTO_TEST_CASE(OGRDataSource_mem_add_n_del_layer)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(k_one);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 0);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 0);
  ogr::Layer l0 = ds->GetLayerChecked(0);
  BOOST_CHECK_THROW(ds->GetLayerChecked(1), itk::ExceptionObject);
  BOOST_CHECK_EQUAL(l.GetName(), k_one);
  BOOST_CHECK_EQUAL(l0.GetName(), k_one);
  BOOST_CHECK_EQUAL(l0.GetName(), k_one);

  BOOST_CHECK_EQUAL(ds->GetLayer(k_one).GetName(), k_one);
  BOOST_CHECK_EQUAL(ds->GetLayerChecked(k_one).GetName(), k_one);
  BOOST_ASSERT(! ds->GetLayer(k_two));
  BOOST_CHECK_THROW(ds->GetLayerChecked(k_two), itk::ExceptionObject);

  ogr::Layer l2 = ds -> CreateLayer(k_two);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 2);
  BOOST_CHECK_EQUAL(ds->Size(false), 0);
  BOOST_CHECK_EQUAL(ds->Size(true), 0);
  BOOST_CHECK_EQUAL(ds->GetLayer(0).GetName(), k_one);
  BOOST_CHECK_EQUAL(ds->GetLayer(1).GetName(), k_two);
  BOOST_CHECK_EQUAL(ds->GetLayer(k_two).GetName(), k_two);
  BOOST_CHECK_THROW(ds->GetLayerChecked(2), itk::ExceptionObject);

  ds->DeleteLayer(0);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 1);
  BOOST_CHECK_EQUAL(ds->GetLayer(0).GetName(), k_two);
  BOOST_CHECK_THROW(ds->GetLayerChecked(1), itk::ExceptionObject);
  BOOST_CHECK_EQUAL(ds->Size(false), 0);
  BOOST_CHECK_EQUAL(ds->Size(true), 0);

  std::cout << "End of scope" << std::endl;
}

BOOST_AUTO_TEST_CASE(OGRDataSource_new_exceptions)
{
  BOOST_CHECK_THROW(ogr::DataSource::New("name-that-shall-not-exist.shp", ogr::DataSource::Modes::read),
    itk::ExceptionObject);
  BOOST_CHECK_THROW(ogr::DataSource::New("unsupported.extension", ogr::DataSource::Modes::write),
    itk::ExceptionObject)
}

BOOST_AUTO_TEST_CASE(OGRDataSource_new_shp)
{
  const std::string k_shp = "SomeShapeFile";
  const std::string k_one = k_shp;
  ogr::DataSource::Pointer ds = ogr::DataSource::New(k_shp+".shp", ogr::DataSource::Modes::write);

  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 0);
  ogr::Layer l = ds -> CreateLayer(k_one);
  BOOST_CHECK_EQUAL(ds->GetLayersCount(), 1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 0);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 0);
  ogr::Layer l0 = ds->GetLayerChecked(0);
  BOOST_CHECK_THROW(ds->GetLayerChecked(1), itk::ExceptionObject);
  BOOST_CHECK_EQUAL(l.GetName(), k_one);
  BOOST_CHECK_EQUAL(l0.GetName(), k_one);
  BOOST_CHECK_EQUAL(l0.GetName(), k_one);

  BOOST_CHECK_EQUAL(ds->GetLayer(k_one).GetName(), k_one);
  BOOST_CHECK_EQUAL(ds->GetLayerChecked(k_one).GetName(), k_one);
  BOOST_ASSERT(! ds->GetLayer(k_two));
  BOOST_CHECK_THROW(ds->GetLayerChecked(k_two), itk::ExceptionObject);

  // no multi layer as it will create other shape files, with the name of the
  // layer as the name of the shape file, whilst the name of the first layer is
  // forced to be the name of the datasource...
  // BOOST_CHECK_THROW(ds -> CreateLayer(k_two), itk::ExceptionObject);
  // BOOST_CHECK_EQUAL(ds->GetLayersCount(), 1);
}

BOOST_AUTO_TEST_CASE(Add_n_Del_Fields)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  ogr::Layer l = ds -> CreateLayer(k_one);
  ogr::Layer l0 = ds->GetLayerChecked(0);

  OGRFeatureDefn & defn = l.GetLayerDefn();
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 0);
  l.CreateField(k_f0);
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 1);
  l.CreateField(k_f1);
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 2);
  l.CreateField(k_f2);
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 3);
  l.CreateField(k_f3);
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 4);
  l.CreateField(k_f4);
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 5);
  l.CreateField(k_f5);
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 6);

    {
    ogr::FieldDefn f0(*defn.GetFieldDefn(0));
    BOOST_CHECK_EQUAL(f0, k_f0);
    ogr::FieldDefn f1(*defn.GetFieldDefn(1));
    BOOST_CHECK_EQUAL(f1, k_f1);
    ogr::FieldDefn f2(*defn.GetFieldDefn(2));
    BOOST_CHECK_EQUAL(f2, k_f2);
    ogr::FieldDefn f3(*defn.GetFieldDefn(3));
    BOOST_CHECK_EQUAL(f3, k_f3);
    ogr::FieldDefn f4(*defn.GetFieldDefn(4));
    BOOST_CHECK_EQUAL(f4, k_f4);
    ogr::FieldDefn f5(*defn.GetFieldDefn(5));
    BOOST_CHECK_EQUAL(f5, k_f5);

    BOOST_CHECK_EQUAL(defn.GetFieldDefn(6), (void*)0);
    }

  l.DeleteField(1);
  BOOST_CHECK_EQUAL(defn.GetFieldCount(), 5);

    {
    ogr::FieldDefn f0(*defn.GetFieldDefn(0));
    BOOST_CHECK_EQUAL(f0, k_f0);
    ogr::FieldDefn f1(*defn.GetFieldDefn(1));
    BOOST_CHECK_EQUAL(f1, k_f2);
    ogr::FieldDefn f2(*defn.GetFieldDefn(2));
    BOOST_CHECK_EQUAL(f2, k_f3);
    ogr::FieldDefn f3(*defn.GetFieldDefn(3));
    BOOST_CHECK_EQUAL(f3, k_f4);
    ogr::FieldDefn f4(*defn.GetFieldDefn(4));
    BOOST_CHECK_EQUAL(f4, k_f5);
    }
  // todo: add reoder tests
}

BOOST_AUTO_TEST_CASE(Add_n_Read_Fields)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  ogr::Layer l = ds -> CreateLayer(k_one);
  ogr::Layer l0 = ds->GetLayerChecked(0);

  OGRFeatureDefn & defn = l.GetLayerDefn();
  l.CreateField(k_f0);
  l.CreateField(k_f1);
  l.CreateField(k_f2);
  l.CreateField(k_f3);
  l.CreateField(k_f4);
  l.CreateField(k_f5);

  ogr::Feature g0(defn);
  const size_t NbFields = defn.GetFieldCount();
  BOOST_CHECK_EQUAL(NbFields, g0.GetSize());
  for (size_t i=0; i!=NbFields; ++i)
    {
    BOOST_ASSERT(!g0[i].HasBeenSet());
    }

  ogr::Field f0 = g0[0];
  BOOST_CHECK_EQUAL(f0.GetName(), k_f0.GetName());
  f0.SetValue(42);
  BOOST_ASSERT(f0.HasBeenSet());
  BOOST_CHECK_EQUAL(f0.GetValue<int>(), 42);
  BOOST_CHECK_EQUAL(g0[0].GetValue<int>(), 42);
  BOOST_CHECK_EQUAL((*l.begin())[0].GetValue<int>(), 42);
  // BOOST_CHECK_assert_FAILS(f0.GetValue<double>(), itk::ExceptionObject);

  ogr::Field f1 = g0["OFTReal"];
  BOOST_CHECK_EQUAL(f1.GetName(), k_f1.GetName());
  // f1.SetValue(42); // need to support types promotion
  f1.SetValue(42.0); // need to support types promotion
  BOOST_ASSERT(f1.HasBeenSet());
  BOOST_CHECK_EQUAL(f1.GetValue<double>(), 42.0);

#if 0 // not ready
  ogr::Field f2 = g0["OFTString"];
  BOOST_CHECK_EQUAL(f2.GetName(), k_f2.GetName());
  f2.SetValue(("foobar")); // need to support types promotion
  BOOST_ASSERT(f2.HasBeenSet());
  BOOST_CHECK_EQUAL(f2.GetValue<std::string>(), "foobar");
#endif

}
