#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

//----------------- BIGTIFF -------------------------
int main(int argc, char * argv[])
{

  const char * inputFilename  = argv[1];
	
  string patternDone("done");
  ifstream file(inputFilename, ios::in); 
  if(file)  
  {
	string line; 
	bool bigtiff=false;
	
	while(getline(file, line))
		if (line.find(patternDone) !=  string::npos)
			bigtiff=true;
	
	if (!bigtiff)
	{
		cout << "WARNING : No BIGTIFF capatilities" << endl;
		return 1; 
	}
	
	file.close();
  
   }
   /*else
   {
	    cout << "WARNING  : Can't open file " << inputFilename << endl;
		return 2;
   }*/

   
  return EXIT_SUCCESS;

}
