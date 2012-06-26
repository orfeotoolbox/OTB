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
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "itksys/SystemTools.hxx"
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
}

BOOST_AUTO_TEST_CASE(OGRDataSource_new_exceptions)
{
  BOOST_CHECK_THROW(ogr::DataSource::New("name-that-shall-not-exist.shp", ogr::DataSource::Modes::Read),
    itk::ExceptionObject);
  BOOST_CHECK_THROW(ogr::DataSource::New("unsupported.extension", ogr::DataSource::Modes::Overwrite),
    itk::ExceptionObject)
}

BOOST_AUTO_TEST_CASE(OGRDataSource_new_shp)
{
  if (boost::unit_test::framework::master_test_suite().argc < 2)
    {
    BOOST_THROW_EXCEPTION( std::runtime_error("not enough arguments") );
    }

  const std::string k_shp = boost::unit_test::framework::master_test_suite().argv[1];
  const std::string k_one = k_shp;
  ogr::DataSource::Pointer ds = ogr::DataSource::New(k_shp+".shp", ogr::DataSource::Modes::Overwrite);

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


BOOST_AUTO_TEST_CASE(OGRDataSource_shp_overwrite)
{
  if (boost::unit_test::framework::master_test_suite().argc < 2)
    {
    BOOST_THROW_EXCEPTION( std::runtime_error("not enough arguments") );
    }

  const std::string k_name = boost::unit_test::framework::master_test_suite().argv[1];
  const std::string workingdir = k_name + "/shp";

  // Create an empty temporary directory for the test
  if ( itksys::SystemTools::FileExists(workingdir.c_str()) )
    {
    itksys::SystemTools::RemoveADirectory(workingdir.c_str());
    }
  itksys::SystemTools::MakeDirectory(workingdir.c_str());

  std::string filename = workingdir + "/" + k_name + ".shp";
  filename = itksys::SystemTools::ConvertToOutputPath(filename.c_str());

  const std::string layer1 = k_name;

  // Cannot create read data source if file does not exists
  BOOST_CHECK_THROW(ogr::DataSource::New(filename, ogr::DataSource::Modes::Read),
                    itk::ExceptionObject);

  // Check invalid modes
  BOOST_CHECK_THROW(ogr::DataSource::New(filename, ogr::DataSource::Modes::Invalid),
                    itk::ExceptionObject);
  BOOST_CHECK_THROW(ogr::DataSource::New(filename, ogr::DataSource::Modes::MAX__),
                    itk::ExceptionObject);

  // Overwrite mode supports the creation of file if it does not exists
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Overwrite);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1, 0, wkbPoint);
  OGRFeatureDefn & defn = l.GetLayerDefn();
  l.CreateField(k_f0);
  l.CreateField(k_f1);
  ogr::Feature f(defn);
  f[0].SetValue(42);
  f[1].SetValue(42.0);
  const OGRPoint p(42, 42);
  f.SetGeometry(&p);
  l.CreateFeature(f);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 1);
  }

  // Read file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> GetLayerChecked(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);
  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 42);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 42.0);

  ogr::UniqueGeometryPtr p = f.StealGeometry();
  const OGRPoint ref(42, 42);
  BOOST_CHECK(ogr::Equals(*p, ref));
  }

  // Now really test overwriting the file already exists)
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Overwrite);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1, 0, wkbPoint);
  OGRFeatureDefn & defn = l.GetLayerDefn();
  l.CreateField(k_f0);
  l.CreateField(k_f1);
  ogr::Feature f(defn);
  f[0].SetValue(43);
  f[1].SetValue(43.0);
  const OGRPoint p(43, 43);
  f.SetGeometry(&p);
  l.CreateFeature(f);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 1);
  }

  // Read file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> GetLayerChecked(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);
  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);

  ogr::UniqueGeometryPtr p = f.StealGeometry();
  const OGRPoint ref(43, 43);
  BOOST_CHECK(ogr::Equals(*p, ref));
  }


  // Open in Update_LayerUpdate
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Update_LayerUpdate);

  // Check that we can read the file
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> GetLayerChecked(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);
  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);

  ogr::UniqueGeometryPtr p = f.StealGeometry();
  const OGRPoint ref(43, 43);
  BOOST_CHECK(ogr::Equals(*p, ref));

  // Add a second feature
  ogr::Feature f2(l.GetLayerDefn());
  f2[0].SetValue(44);
  f2[1].SetValue(44.0);
  const OGRPoint p2(44, 44);
  f2.SetGeometry(&p2);
  l.CreateFeature(f2);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 2);
  }


  // Read the file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> GetLayerChecked(0);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 2);

  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);
  ogr::UniqueGeometryPtr p1 = f.StealGeometry();
  const OGRPoint ref1(43, 43);
  BOOST_CHECK(ogr::Equals(*p1, ref1));

  f = l.GetFeature(1);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 44);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 44.0);
  ogr::UniqueGeometryPtr p2 = f.StealGeometry();
  const OGRPoint ref2(44, 44);
  BOOST_CHECK(ogr::Equals(*p2, ref2));

  }


  // Open in Update_LayerCreateOnly
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Update_LayerCreateOnly);

  // Check that we can read the file
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> GetLayerChecked(0);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 2);

  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);
  ogr::UniqueGeometryPtr p1 = f.StealGeometry();
  const OGRPoint ref1(43, 43);
  BOOST_CHECK(ogr::Equals(*p1, ref1));

  f = l.GetFeature(1);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 44);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 44.0);
  ogr::UniqueGeometryPtr p2 = f.StealGeometry();
  const OGRPoint ref2(44, 44);
  BOOST_CHECK(ogr::Equals(*p2, ref2));


  // Cannot modify a layer which already exists
  std::string name = l.GetName();
  ogr::Layer createdlayer = ds->CreateLayer(name);

  OGRFeatureDefn & defn = l.GetLayerDefn();
  ogr::Feature f2(defn);
  f2[0].SetValue(45);
  f2[1].SetValue(45.0);
  const OGRPoint p(45, 45);
  f2.SetGeometry(&p);
  BOOST_CHECK_THROW(l.CreateFeature(f2), itk::ExceptionObject);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 2);

  }


  // Read the file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> GetLayerChecked(0);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 2);

  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);
  ogr::UniqueGeometryPtr p1 = f.StealGeometry();
  const OGRPoint ref1(43, 43);
  BOOST_CHECK(ogr::Equals(*p1, ref1));

  f = l.GetFeature(1);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 44);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 44.0);
  ogr::UniqueGeometryPtr p2 = f.StealGeometry();
  const OGRPoint ref2(44, 44);
  BOOST_CHECK(ogr::Equals(*p2, ref2));

  }

#if 0 // shp files do not support Update_LayerOverwrite
  // Open in Update_LayerOverwrite
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Update_LayerOverwrite);

  // Check that we can read the file
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1);
  BOOST_ASSERT(l.GetFeatureCount(true) == 2);


  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);
  ogr::UniqueGeometryPtr p1 = f.StealGeometry();
  const OGRPoint ref1(43, 43);
  BOOST_CHECK(ogr::Equals(*p1, ref1));

  f = l.GetFeature(1);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 44);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 44.0);
  ogr::UniqueGeometryPtr p2 = f.StealGeometry();
  const OGRPoint ref2(44, 44);
  BOOST_CHECK(ogr::Equals(*p2, ref2));

  // Add a new feature
  ogr::Feature f3(l.GetLayerDefn());
  f3[0].SetValue(45);
  f3[1].SetValue(45.0);
  const OGRPoint p3(45, 45);
  f3.SetGeometry(&p3);
  l.CreateFeature(f3);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 1);
  }


  // Read file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> GetLayerChecked(0);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);

  ogr::Feature f = l.GetFeature(0);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);
  ogr::UniqueGeometryPtr p1 = f.StealGeometry();
  const OGRPoint ref1(43, 43);
  BOOST_CHECK(ogr::Equals(*p1, ref1));

  f = l.GetFeature(1);
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 44);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 44.0);
  ogr::UniqueGeometryPtr p2 = f.StealGeometry();
  const OGRPoint ref2(44, 44);
  BOOST_CHECK(ogr::Equals(*p2, ref2));

  }
#endif

}



BOOST_AUTO_TEST_CASE(OGRDataSource_sqlite_overwrite)
{
  if (boost::unit_test::framework::master_test_suite().argc < 2)
    {
    BOOST_THROW_EXCEPTION( std::runtime_error("not enough arguments") );
    }

  const std::string k_name = boost::unit_test::framework::master_test_suite().argv[1];
  const std::string workingdir = k_name + "/sqlite";

  // Create an empty temporary directory for the test
  if ( itksys::SystemTools::FileExists(workingdir.c_str()) )
    {
    itksys::SystemTools::RemoveADirectory(workingdir.c_str());
    }
  itksys::SystemTools::MakeDirectory(workingdir.c_str());

  std::string filename = workingdir + "/" + k_name + ".sqlite";
  filename = itksys::SystemTools::ConvertToOutputPath(filename.c_str());

  const std::string layer1 = boost::algorithm::to_lower_copy(k_name);

  // Cannot create read data source if file does not exists
  BOOST_CHECK_THROW(ogr::DataSource::New(filename, ogr::DataSource::Modes::Read),
                    itk::ExceptionObject);

  // Check invalid modes
  BOOST_CHECK_THROW(ogr::DataSource::New(filename, ogr::DataSource::Modes::Invalid),
                    itk::ExceptionObject);
  BOOST_CHECK_THROW(ogr::DataSource::New(filename, ogr::DataSource::Modes::MAX__),
                    itk::ExceptionObject);

  // Overwrite mode supports the creation of file if it does not exists
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Overwrite);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1, 0, wkbPoint);
  OGRFeatureDefn & defn = l.GetLayerDefn();
  l.CreateField(k_f0);
  l.CreateField(k_f1);
  ogr::Feature f(defn);
  f[0].SetValue(42);
  f[1].SetValue(42.0);
  const OGRPoint p(42, 42);
  f.SetGeometry(&p);
  l.CreateFeature(f);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 1);
  }

  // Read file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds->GetLayerChecked(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);
  ogr::Feature f = *l.begin();
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 42);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 42.0);

  ogr::UniqueGeometryPtr p = f.StealGeometry();
  const OGRPoint ref(42, 42);
  BOOST_CHECK(ogr::Equals(*p, ref));
  }

  // Now really test overwriting the file already exists)
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Overwrite);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1, 0, wkbPoint);
  OGRFeatureDefn & defn = l.GetLayerDefn();
  l.CreateField(k_f0);
  l.CreateField(k_f1);
  ogr::Feature f(defn);
  f[0].SetValue(43);
  f[1].SetValue(43.0);
  const OGRPoint p(43, 43);
  f.SetGeometry(&p);
  l.CreateFeature(f);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 1);
  }

  // Read file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);
  ogr::Feature f = *l.begin();
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);

  ogr::UniqueGeometryPtr p = f.StealGeometry();
  const OGRPoint ref(43, 43);
  BOOST_CHECK(ogr::Equals(*p, ref));
  }


  // Open in Update_LayerUpdate
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Update_LayerUpdate);

  // Check that we can read the file
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);

  ogr::Layer::iterator fIt = l.begin();

  ogr::Feature f = *fIt;
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);

  ogr::UniqueGeometryPtr p = f.StealGeometry();
  const OGRPoint ref(43, 43);
  BOOST_CHECK(ogr::Equals(*p, ref));

  // Add a second feature
  ogr::Feature f2(l.GetLayerDefn());
  f2[0].SetValue(44);
  f2[1].SetValue(44.0);
  const OGRPoint p2(44, 44);
  f2.SetGeometry(&p2);
  l.CreateFeature(f2);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 2);
  }


  // Read the file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 2);

  ogr::Layer::iterator fIt = l.begin();
  ogr::Feature f = *fIt;
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 43);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 43.0);
  ogr::UniqueGeometryPtr p1 = f.StealGeometry();
  const OGRPoint ref1(43, 43);
  BOOST_CHECK(ogr::Equals(*p1, ref1));

  f = *++fIt;
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 44);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 44.0);
  ogr::UniqueGeometryPtr p2 = f.StealGeometry();
  const OGRPoint ref2(44, 44);
  BOOST_CHECK(ogr::Equals(*p2, ref2));

  }

  // Open in Update_LayerOverwrite
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Update_LayerOverwrite);

  // Check that we can read the file
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1, 0, wkbPoint);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 0);

  OGRFeatureDefn & defn = l.GetLayerDefn();
  l.CreateField(k_f0);
  l.CreateField(k_f1);


  // Add a new feature
  ogr::Feature f3(l.GetLayerDefn());
  f3[0].SetValue(45);
  f3[1].SetValue(45.0);
  const OGRPoint p3(45, 45);
  f3.SetGeometry(&p3);
  l.CreateFeature(f3);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 1);
  }


  // Read file we have written
  {
  ogr::DataSource::Pointer ds
    = ogr::DataSource::New(filename, ogr::DataSource::Modes::Read);
  BOOST_ASSERT(ds);
  ogr::Layer l = ds -> CreateLayer(layer1);
  BOOST_CHECK_EQUAL(l.GetFeatureCount(true), 1);

  ogr::Layer::iterator fIt = l.begin();

  ogr::Feature f = *fIt;
  BOOST_CHECK_EQUAL(f[0].GetValue<int>(), 45);
  BOOST_CHECK_EQUAL(f[1].GetValue<double>(), 45.0);
  ogr::UniqueGeometryPtr p1 = f.StealGeometry();
  const OGRPoint ref1(45, 45);
  BOOST_CHECK(ogr::Equals(*p1, ref1));

  }

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

#if GDAL_VERSION_NUM >= 1900
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
#endif
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

  // ----[ int
  ogr::Field f0 = g0[0];
#if GDAL_VERSION_NUM >= 1800
  BOOST_CHECK_EQUAL(f0.GetName(), k_f0.GetName());
#endif
  f0.SetValue(42);
  BOOST_ASSERT(f0.HasBeenSet());
  BOOST_CHECK_EQUAL(f0.GetValue<int>(), 42);
  BOOST_CHECK_EQUAL(g0[0].GetValue<int>(), 42);
  // BOOST_CHECK_EQUAL((*l.begin())[0].GetValue<int>(), 42);
  // BOOST_CHECK_assert_FAILS(f0.GetValue<double>(), itk::ExceptionObject);

  // ----[ double
  ogr::Field f1 = g0["OFTReal"];
#if GDAL_VERSION_NUM >= 1800
  BOOST_CHECK_EQUAL(f1.GetName(), k_f1.GetName());
#endif
  // f1.SetValue(42); // need to support types promotion
  f1.SetValue(42.0); // need to support types promotion
  BOOST_ASSERT(f1.HasBeenSet());
  BOOST_CHECK_EQUAL(f1.GetValue<double>(), 42.0);

  // ----[ string
  ogr::Field f2 = g0["OFTString"];
#if GDAL_VERSION_NUM >= 1800
  BOOST_CHECK_EQUAL(f2.GetName(), k_f2.GetName());
#endif
  f2.SetValue(("foobar")); // need to support types promotion
  BOOST_ASSERT(f2.HasBeenSet());
  BOOST_CHECK_EQUAL(f2.GetValue<std::string>(), "foobar");
  f2.Unset();
  BOOST_ASSERT(!f2.HasBeenSet());

  f2.SetValue(std::string("foo"));
  BOOST_ASSERT(f2.HasBeenSet());
  BOOST_CHECK_EQUAL(f2.GetValue<std::string>(), "foo");

  // ----[ list of ints
    {
    ogr::Field f3 = g0["OFTIntegerList"];
    std::vector<int> v0;
    v0.push_back(42);
    v0.push_back(12);
    f3.SetValue(v0);

    std::vector<int> w0 = f3.GetValue<std::vector<int> >();
    BOOST_CHECK_EQUAL_COLLECTIONS(v0.begin(),v0.end(),w0.begin(),w0.end());
    }

  // ----[ list of doubles
    {
    ogr::Field f4 = g0["OFTRealList"];
    std::vector<double> v1;
    v1.push_back(42);
    v1.push_back(12);
    f4.SetValue(v1);

    std::vector<double> w1 = f4.GetValue<std::vector<double> >();
    BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(),v1.end(),w1.begin(),w1.end());
    }

  // ----[ list of string
    {
    ogr::Field f5 = g0["OFTStringList"];
    std::vector<std::string> v2;
    v2.push_back("42");
    v2.push_back("12");
    f5.SetValue(v2);

    std::vector<std::string> w2 = f5.GetValue<std::vector<std::string> >();
    BOOST_CHECK_EQUAL_COLLECTIONS(v2.begin(),v2.end(),w2.begin(),w2.end());
    }
}

BOOST_AUTO_TEST_CASE(OGRDataSource_new_shp_with_features)
{
  const std::string k_shp = "SomeShapeFileWithFeatures";
  const std::string k_one = k_shp;
  ogr::DataSource::Pointer ds = ogr::DataSource::New(k_shp+".shp", ogr::DataSource::Modes::Overwrite);

  ogr::Layer l = ds -> CreateLayer(k_one, 0, wkbPoint);

  OGRFeatureDefn & defn = l.GetLayerDefn();
  l.CreateField(k_f0);
  ogr::Feature g0(defn);
  g0[0].SetValue(42);
  l.CreateFeature(g0);
}

BOOST_AUTO_TEST_CASE(Local_Geometries)
{
  ogr::UniqueGeometryPtr gp (OGRGeometryFactory::createGeometry(wkbPoint));
  BOOST_REQUIRE(gp);
  OGRPoint * p = dynamic_cast<OGRPoint*>(gp.get());
  BOOST_REQUIRE(p);
  p->setX(0);
  p->setY(0);

  OGRPoint x0(-10, 0);
  OGRPoint x1(+10, 0);
  OGRLineString X;
  X.addPoint(&x0);
  X.addPoint(&x1);

  OGRPoint y0(0, -10);
  OGRPoint y1(0, +10);
  OGRLineString Y;
  Y.addPoint(&y0);
  Y.addPoint(&y1);

  ogr::UniqueGeometryPtr i = ogr::Intersection(X, Y);
  BOOST_CHECK(ogr::Equals(*i, *p));
  BOOST_CHECK(ogr::Equals(*i, *gp));
}

BOOST_AUTO_TEST_CASE(Add_n_Read_Geometries)
{
  ogr::DataSource::Pointer ds = ogr::DataSource::New();
  ogr::Layer l = ds -> CreateLayer(k_one, 0, wkbPoint);

  OGRFeatureDefn & defn = l.GetLayerDefn();
  for (int u=-10; u!=10; ++u) {
    ogr::Feature f(defn);
    const OGRPoint p(u, u);
    f.SetGeometry(&p);
    l.CreateFeature(f);
  }
  BOOST_CHECK_EQUAL(l.GetFeatureCount(false), 20);

  int u=-10;
  BOOST_FOREACH(ogr::Feature f, l)
    {
    const OGRPoint ref(u, u);
    ogr::UniqueGeometryPtr p = f.StealGeometry();
    BOOST_REQUIRE(p);
    BOOST_CHECK(! f.GetGeometry());
    BOOST_CHECK(ogr::Equals(*p, ref));
    f.SetGeometryDirectly(otb::move(p));
    BOOST_CHECK(!p);
    ++u;
    }

}

#if 0
BOOST_AUTO_TEST_CASE(OGRDataSource_new_shp_with_features_raw)
{
  const std::string k_shp = "SomeShapeFileWithFeaturesRaw";
  const std::string k_one = k_shp;
  OGRSFDriver * dr = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
  BOOST_ASSERT(dr);
  OGRDataSource * ds =  dr->CreateDataSource((k_shp+".shp").c_str());
  BOOST_ASSERT(ds);
  OGRLayer * l = ds -> CreateLayer(k_one.c_str(), 0, wkbPoint);
  BOOST_ASSERT(l);
  l->CreateField(&k_f0_);

  OGRFeatureDefn * defn = l->GetLayerDefn();
  BOOST_ASSERT(defn);
  OGRFeature * g0 =  OGRFeature::CreateFeature(defn);
  BOOST_ASSERT(g0);
  g0 -> SetField(0, 42);
  l->CreateFeature(g0);
  OGRFeature::DestroyFeature(g0);
  OGRDataSource::DestroyDataSource(ds);
}
#endif
