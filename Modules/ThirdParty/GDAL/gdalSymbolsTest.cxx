/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include <fstream>
#include <iostream>

#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

//----------------- if internal libtiff/libgeotiff are detected, symbol renaming is mandatory -------------------------
int main(int argc, char * argv[])
{

  const char * inputFilename  = argv[1];

  string pattern1("TIFFClose");
  string pattern2("gdal_");
  ifstream file(inputFilename, ios::in);
  if(file)
  {
	string line;
	while(getline(file, line))
	{
		if (line.find(pattern1) !=  string::npos)
			if (line.find(pattern2) == string::npos)
				{
        cout << "Internal versions of libtiff/libgeotiff detected without symbol renaming!" << std::endl;
        cout << "When configuring GDAL, if options --with-libtiff or --with-geotiff are set to 'internal', then options --with-rename-internal-libtiff-symbols and --with-rename-internal-libgeotiff-symbols should be set to 'yes')" << std::endl;
					return EXIT_FAILURE;
				}

	}
	file.close();
   }


  return EXIT_SUCCESS;

}
