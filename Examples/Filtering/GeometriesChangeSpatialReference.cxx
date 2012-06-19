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
#include "otbGeometriesToGeometriesFilter.h"
#include <string>
#include <iostream>
#include "otbOGRDataSourceWrapper.h"
#include "otbGeometriesSet.h"
#include "otbOGRGeometryWrapper.h"

/*===========================================================================*/
/*=========================[ Reprojection functor ]==========================*/
/*===========================================================================*/
namespace internal
{
struct Deleters
{
  void operator()(OGRCoordinateTransformation *p) const { OCTDestroyCoordinateTransformation(p); }
  void operator()(OGRSpatialReference *p) const
    {
#if GDAL_VERSION_NUM >= 1700
    OGRSpatialReference::DestroySpatialReference(p);
#else
#warning the following resource release may crash, please update your version of GDAL
    delete p; // note there is no garanty
#endif
    }
};
} // internal namespace

struct ReprojectTransformationFunctor
{
  typedef OGRGeometry TransformedElementType;

  otb::ogr::UniqueGeometryPtr operator()(OGRGeometry const* in) const;
  void SetSpatialReferenceFromWkt(std::string const& wkt)
    {
    m_osr .reset (static_cast<OGRSpatialReference*>(OSRNewSpatialReference(wkt.c_str())));
    }
  //TODO: find a way to call it from DoProcessLayer
  void UpdateTransformation(OGRSpatialReference & in)
    {
    SetSpatialReferences(in, *m_osr);
    }
private:
  void SetSpatialReferences(OGRSpatialReference & in, OGRSpatialReference & out)
    {
    m_reprojector.reset(OGRCreateCoordinateTransformation(&in, &out));
    }
  boost::interprocess::unique_ptr<OGRCoordinateTransformation, internal::Deleters> m_reprojector;
  boost::interprocess::unique_ptr<OGRSpatialReference,         internal::Deleters> m_osr;
};


otb::ogr::UniqueGeometryPtr
ReprojectTransformationFunctor::operator()(OGRGeometry const* in) const
{
  otb::ogr::UniqueGeometryPtr out(in ? in->clone() : 0);
  if (out)
    {
    const OGRErr err = out->transform(m_reprojector.get());
    if (!err != OGRERR_NONE)
      {
      itkGenericExceptionMacro(<< "Cannot reproject a geometry: " << CPLGetLastErrorMsg());
      }
    }
  return otb::move(out);
}

/*===========================================================================*/
/*=================================[ main ]==================================*/
/*===========================================================================*/
typedef otb::DefaultGeometriesToGeometriesFilter<ReprojectTransformationFunctor> FilterType;

struct Options
{
  Options(int argc, char **argv)
    {
    workingInplace = true;
    outputIsStdout = false;

    std::string inputFile = argv[1];
    std::string outputFile = workingInplace ? argv[1] : argv[2];

    for (int a=1; a!=argc; ++a)
      {
      if (!strcmp(argv[a], "-wkt"))
        wkt = argv[a];
      else if (!strcmp(argv[a], "-"))
        outputIsStdout = true;
      else if (inputFile.empty())
        inputFile = argv[a];
      else if (outputFile.empty())
        {
        outputFile = argv[a];
        workingInplace = false;
        }
      else
        throw std::runtime_error(usage(argv[0])+"Too many (unexpected) arguments");
      }

    if (!outputFile.empty() && outputIsStdout)
      throw std::runtime_error(usage(argv[0])+"An output file ("+outputFile+") has already been set, cannot dump to stdout");
    if (inputFile.empty())
      throw std::runtime_error(usage(argv[0])+"Not enough parameters");
    }
  static std::string usage(std::string const& progname)
    {
    return progname + " <inputGeometriesFile> [<outputGeometriesFile>] -wkt <id>\n";
    }
  std::string inputFile;
  std::string outputFile;
  std::string wkt;
  bool        workingInplace;
  bool        outputIsStdout;
};

int main (int argc, char **argv)
{
  if (argc < 2)
    {
    return EXIT_FAILURE;
    }
  try
    {
    const Options options(argc, argv);

    otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
      options.inputFile,
      options.workingInplace ? otb::ogr::DataSource::Modes::write : otb::ogr::DataSource::Modes::read);

    otb::ogr::DataSource::Pointer output
      = options.workingInplace ? input
      : options.outputIsStdout ? 0
      :                  otb::ogr::DataSource::New( options.outputFile, otb::ogr::DataSource::Modes::write);
    std::cout << "input: " << input -> ogr().GetName() << " should be: " << options.inputFile << "\n";
    if (output)
      {
      std::cout << "output: " << output -> ogr().GetName() << " should be: " << options.outputFile << "\n";
      }
    // std::cout << "\n";

    FilterType::Pointer filter = FilterType::New();
    // TODO: this make no sense for in-place transformations
    (*filter)->SetSpatialReferenceFromWkt(options.wkt);

    if (!options.workingInplace)
      {
      otb::GeometriesSet::Pointer in_set = otb::GeometriesSet::New(input);
      filter->SetInput(in_set);
      }
    if (output)
      {
      otb::GeometriesSet::Pointer out_set = otb::GeometriesSet::New(output);
      filter->SetOutput(out_set);
      out_set->Update();
      }
    else
      {
      filter->UpdateOutputData(0);
      assert(filter->GetOutput() && "output not set");
      filter->GetOutput()->Print(std::cout, 0);
      }

    return EXIT_SUCCESS;
    }
  catch (std::exception const& e)
    {
    std::cerr << e.what() << "\n";
    }
  return EXIT_FAILURE;
}
