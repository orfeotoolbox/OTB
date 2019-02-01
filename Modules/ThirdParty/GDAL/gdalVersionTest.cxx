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

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <gdal.h>

using namespace std;

//----------------- FORMAT -------------------------
int main(int argc, char * argv[])
{

  int MAJOR   = atoi(argv[2]);
  int MINOR   = atoi(argv[3]);

  string version(GDALVersionInfo("RELEASE_NAME"));

  // Remember gdal version
  const char * inputFilename  = argv[1];
  ofstream file(inputFilename, ios::out | ios::trunc);
  if(file)
  {

	file << version;
	file.close();

  }
   /*else
   {
	    cout << "WARNING  : Can't open file " << inputFilename << endl;
		return 2;
   }*/


  //Version check
  istringstream iss(version);
  unsigned int pos=0;
  string sub;
  unsigned int someUIntVal;
  vector<unsigned int> UIntVect;
  while ( std::getline( iss, sub, '.' ) )
	{
		istringstream convert(sub);
		convert >> someUIntVal;
		//cout << someUIntVal << '\n';

		if (pos==0) //Major
			UIntVect.push_back(someUIntVal);
		if (pos==1) //Minor
			UIntVect.push_back(someUIntVal);

		pos++;
	}


  if ( (UIntVect[0]==MAJOR && UIntVect[1]<MINOR) || (UIntVect[0]<MAJOR) )
	{
		cout << "WARNING : Version of GDAL must be >= " << MAJOR << "." << MINOR << " : " << UIntVect[0] << "." << UIntVect[1] << " detected)." << endl;
		return EXIT_FAILURE;
	}


  return EXIT_SUCCESS;

}
