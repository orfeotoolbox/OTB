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

  
  if ( (UIntVect[0]<MAJOR) || (UIntVect[1]<MINOR) )
	{
		cout << "WARNING : Version of GDAL must be >= " << MAJOR << "." << MINOR << " : " << UIntVect[0] << "." << UIntVect[1] << " detected)." << endl;
		return 1;
	}

   
  return EXIT_SUCCESS; 

}
