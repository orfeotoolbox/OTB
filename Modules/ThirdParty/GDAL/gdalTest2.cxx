#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>


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
					cout << "WARNING : Internal versions of libtiff/libgeotiff detected without symbol renaming." << endl;
					return 1; 
				}
				
	}
  
   }
   /*else
   {
	    cout << "WARNING  : Can't open file " << inputFilename << endl;
		return 2;
   }*/

   
  return EXIT_SUCCESS;

}
