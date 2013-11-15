
// iostream is used for general output
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ossim/base/ossimString.h"
#include "ossim/ossimTileMapModel.h"
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>

int ossimTileMapModelTest(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cout<<"<latitude> <longitude> <output ascii file>"<<std::endl;
    return EXIT_FAILURE;
  }
  else
  {
    // Ossim containers and tileMapModel
    ossimRefPtr<ossimplugins::ossimTileMapModel> tileMapModel = new ossimplugins::ossimTileMapModel;
    ossimGpt gpt;
    ossimDpt dpt;

    // Open & write ascii file
    ofstream file;

    std::cout<<"Ascii file :: "<<argv[3]<<std::endl;

    file.open(argv[3]);

    file << fixed;

    for (unsigned int depth=1; depth<19; depth++)
    {
      // Parameter tileMapModel
      tileMapModel->setDepth(depth);

      gpt.lon = static_cast<double>(atof(argv[2]));
      gpt.lat = static_cast<double>(atof(argv[1]));

      // Write in ascii file
      file<<setprecision(5);
      file<<"--- DEPTH = "<<tileMapModel->getDepth()<<" ---"<<std::endl;
      file<<"latitude base: "<<gpt.lat<<std::endl;
      file<<"longitude base: "<<gpt.lon<<std::endl;

      // Convert lat/lon to line sample
      tileMapModel->worldToLineSample(gpt, dpt);

      // Write in ascii file
      file << setprecision(0);
      file<<"tile x: "<<dpt.samp<<std::endl;
      file<<"tile y: "<<dpt.line<<std::endl;

      // Convert line sample to lat/lon
      tileMapModel->lineSampleToWorld(dpt, gpt);

      // Write in ascii file
      file << setprecision(5);
      file<<"latitude: "<<gpt.lat<<std::endl;
      file<<"longitude: "<<gpt.lon<<std::endl;
    }
    file.close();
  }
  return EXIT_SUCCESS;
}



