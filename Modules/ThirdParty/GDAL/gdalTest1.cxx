#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;


//----------------- gdal >= 1.10 -------------------------
int main(int argc, char * argv[])
{

  const char * inputFilename  = argv[1];
  
  ifstream file(inputFilename, ios::in); 
  if(file)  
  {
	
	string line;  
	vector<unsigned int> UIntVect;
	
	getline(file, line);

		//Parsing 
		istringstream iss(line);
		unsigned int pos=0;
		string sub;
		unsigned int someUIntVal;
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
	 
	file.close();
	
	if ( (UIntVect[0]<1) || (UIntVect[1]<10) )
	{
		cout << "WARNING : Version of GDAL must be >= 1.10 (" << UIntVect[0] << "." << UIntVect[1] << " detected)." << endl;
		return 1;
	}

   }
   /*else
   {
	    cout << "WARNING  : Can't open file " << inputFilename << endl;
		return 2;
   }*/
 

   
  return EXIT_SUCCESS;

}
