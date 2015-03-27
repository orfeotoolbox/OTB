#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

//----------------- FORMAT -------------------------
int main(int argc, char * argv[])
{

  const char * inputFilename  = argv[1];
  string format(argv[2]);
  
  ifstream file(inputFilename, ios::in); 
  if(file)  
  {
	string line; 
	
	while(getline(file, line))
		if (line.find(format) !=  string::npos)
			return 1; //SUCCESS
			
	file.close();
  
   }
   /*else
   {
	    cout << "WARNING  : Can't open file " << inputFilename << endl;
		return 2;
   }*/

   
  return 0; //FAIL

}
