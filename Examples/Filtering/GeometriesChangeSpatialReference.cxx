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
  void SetTransformation(OGRSpatialReference & in, OGRSpatialReference & out)
    {
    m_reprojector.reset(OGRCreateCoordinateTransformation(&in, &out));
    }

private:
  boost::interprocess::unique_ptr<OGRCoordinateTransformation, internal::Deleters> m_reprojector;
};

otb::ogr::UniqueGeometryPtr
ReprojectTransformationFunctor::operator()(OGRGeometry const* in) const
{
  otb::ogr::UniqueGeometryPtr out(in ? in->clone() : 0);
  if (out)
    {
    const OGRErr err = out->transform(m_reprojector.get());
    if (err != OGRERR_NONE)
      {
      itkGenericExceptionMacro(<< "Cannot reproject a geometry: " << CPLGetLastErrorMsg());
      }
    }
  return otb::move(out);
}

/*===========================================================================*/
/*==========================[ reprojection filter ]==========================*/
/*===========================================================================*/
class MyReprojectionFilter : public otb::DefaultGeometriesToGeometriesFilter<ReprojectTransformationFunctor>
{
public:
  typedef MyReprojectionFilter            Self;
  typedef otb::DefaultGeometriesToGeometriesFilter<ReprojectTransformationFunctor> Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(MyReprojectionFilter, otb::DefaultGeometriesToGeometriesFilter);

  void SetOutputSpatialReference(std::string const& srs);

  void UpdateTransformation(OGRSpatialReference &in)
    {
    (*this)->SetTransformation(in, *m_osr);
    }

protected:
  /** Default constructor. */
  MyReprojectionFilter(){ }
  /** Destructor. */
  virtual ~MyReprojectionFilter(){ }

private:

  virtual OGRSpatialReference*     DoDefineNewLayerSpatialReference(otb::ogr::Layer const& source) const
    {
    return m_osr.get();
    }

  boost::interprocess::unique_ptr<OGRSpatialReference,         internal::Deleters> m_osr;
};


void
MyReprojectionFilter::SetOutputSpatialReference(std::string const& srs)
{
  OGRSpatialReference outSrs;

  char *name = const_cast<char *>(srs.c_str());

  OGRErr ret = outSrs.importFromWkt(&name);
  if (ret)
    {
    ret = outSrs.importFromProj4(name);
    }
  if (ret)
    {
    ret = outSrs.importFromEPSG(atoi(name));
    }

  if (ret)
    {
    std::cout << "Error : spatial reference system not recognized ("<<srs<<")" << std::endl;
    }

  m_osr .reset (outSrs.Clone());
}

/*===========================================================================*/
/*=================================[ main ]==================================*/
/*===========================================================================*/
typedef MyReprojectionFilter FilterType;

struct Options
{
  Options(int argc, char **argv)
    {
    workingInplace = true;
    outputIsStdout = false;

    for (int a=1; a!=argc; ++a)
      {
      if (!strcmp(argv[a], "-srs"))
        {
        if (a+1 < argc)
          {
          ++a;
          srs = argv[a];
          }
        }
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
    if (inputFile.empty() || srs.empty())
      throw std::runtime_error(usage(argv[0])+"Not enough parameters");
    }
  static std::string usage(std::string const& progname)
    {
    return progname + " <inputGeometriesFile> [<outputGeometriesFile>] -srs <id>\n";
    }
  std::string inputFile;
  std::string outputFile;
  std::string srs;
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
      options.workingInplace ? otb::ogr::DataSource::Modes::Update_LayerOverwrite : otb::ogr::DataSource::Modes::Read);

    otb::ogr::DataSource::Pointer output
      = options.workingInplace ? input
      : options.outputIsStdout ? 0
      : otb::ogr::DataSource::New( options.outputFile, otb::ogr::DataSource::Modes::Update_LayerCreateOnly);
    std::cout << "input: " << input -> ogr().GetName() << " should be: " << options.inputFile << "\n";
    if (output)
      {
      std::cout << "output: " << output -> ogr().GetName() << " should be: " << options.outputFile << "\n";
      }
    // std::cout << "\n";

    FilterType::Pointer filter = FilterType::New();
    // TODO: this make no sense for in-place transformations
    filter->SetOutputSpatialReference(options.srs);
    filter->UpdateTransformation( *(const_cast<OGRSpatialReference*>(input->GetLayer(0).GetSpatialRef())) );

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
