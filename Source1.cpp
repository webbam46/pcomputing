#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.h>
#endif

#include "Utils.h"
#include "Data.h"





//Program entry
int main(int argc, char ** argv) {
	//Detect potential exceptions
	try
	{
		/*
			1. SETUP
		
		*/

		//Import data
		Data data = Data("data_short.txt");

		//Calculate the sum of the temperature values
		data.SumTemperature();



	}
	catch (std::exception e){  }
	
	//catch(cl::Error error){ std::cout << "Error occured: " << error.what() << "," << getErrorString(error.err()) << ""; }
	std::cin.get();
}