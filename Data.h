#pragma once


using namespace std;


/*
	Data class

*/
class Data
{
public:
	std::vector<const char*> place; //Places
	std::vector<double> year; //Year
	std::vector<double> month; //Month
	std::vector<double> day; //Day
	std::vector<double> time; //Time
	std::vector<double> temperature; //Temperature

	//Variables used to aid multi-threading
	int platform_id; //Platform ID
	int device_id; //Device ID
	cl::Context context; //Context
	cl::CommandQueue queue; //Command Queue
	cl::Program program; //Program

	/* Constructor - takes source of data file */
	/*
		Uses OPENCL for parallel processing of input data
	*/
	Data(char * src) {
		//Check for potential exceptions
		try
		{
			//Init multi-threading variables
			platform_id = 0;
			device_id = 0;
			//Select computing devices
			cl::Context context = GetContext(platform_id, device_id);

			//Display the running device
			DisplayRunningDevice(platform_id, device_id);

			//Create a queue which will be used to push commands to the device
			cl::CommandQueue queue(context);

			//First - attempt to build the kernel code, as multi-threading may be required
			//Attempt to build kernel code
			cl::Program program;
			try {
				program = BuildKernelCode(context, "data_kernels.cl");
			}
			catch (std::exception e) { std::cout << e.what() << std::endl; }

			//Holds current line
			string line = "";
			//Attempt to read in the file from given src
			ifstream _file(src);

			//Only attempt if file was opened successfully
			if (_file.is_open())
			{
				std::cout << "Loading data file.." << std::endl;
				//Read file contents
				while (getline(_file, line)) {
					/*
						Split the line
					*/
					stringstream ss(line);
					string item;
					vector<const char *> tokens;
					while (getline(ss, item,' ')) {
						tokens.push_back(item.c_str());
					}


					place.push_back(tokens[0]); //Place names arraylist
					year.push_back(atof(tokens[0])); //Year arraylist
					month.push_back(atof(tokens[0])); //Month arraylist
					day.push_back(atof(tokens[0])); //Day arraylist
					time.push_back(atof(tokens[0])); //Time arraylist
					temperature.push_back(atof(tokens[0])); //Temperature arraylist
				}
			}
			//Print error - unable to open data file
			else {
				std::cout << "Unable to open file from src" << std::endl;
			}
		}
		//Catcj any errors
		catch (cl::Error error) { std::cout << "Error occured: " << error.what() << "," << getErrorString(error.err()) << ""; }
	}

	/* Display all data to console */
	void Display() {
		for (int i = 0; i < place.size(); i++) {
			std::cout << "Place: " << place[i] << " " <<
				"Year: " << year[i] <<  " " <<
				"Month: " << month[i] <<  " " <<
				"Day: " << day[i] << " " <<
				"Time: " << time[i] << " " <<
				"Temp: " << temperature[i] << std::endl;
		}
	}

	/* Display data for given place name */
	void Display(string name) {
		for (int i = 0; i < place.size(); i++) {
			if(place[i]==name){
			std::cout << "Place: " << place[i] << " " <<
				"Year: " << year[i] << " " <<
				"Month: " << month[i] << " " <<
				"Day: " << day[i] << " " <<
				"Time: " << time[i] << " " <<
				"Temp: " << temperature[i] << std::endl;
			}
		}
	}
	/*
	
		Calculate the sum temperature value
	*/
	double SumTemperature() {
		//Check for errors
		try
		{
			//Input vector
			std::vector<double> temp = temperature;

			//Determine local size
			size_t local_size = 10;
			//Padding size
			size_t padding_size = temperature.size() % local_size;

			//If the vector is not a multiple of the local_size -- insert NULL elements, such that the result will not be affected
			if (padding_size) {
				//Create a padded vector with NULL values
				std::vector<double> temp_padded(local_size - padding_size, 0);
				//Append to input vector
				temp.insert(temp.end(), temp_padded.begin(), temp_padded.end());
			}

			//Memory alloc for input vector
			size_t input_elements = temp.size();
			size_t input_size = temp.size()*sizeof(double);
			size_t nr_groups = input_elements / local_size;

			//Output vector
			std::vector<double> out(input_elements);
			size_t output_size = out.size()*sizeof(double);

			//Device buffers
			cl::Buffer buffer_in(context, CL_MEM_READ_ONLY, input_size);
			cl::Buffer buffer_out(context, CL_MEM_READ_WRITE, output_size);

			//Setup buffers
			queue.enqueueWriteBuffer(buffer_in, CL_TRUE, 0, input_size, &temp[0]);
			queue.enqueueFillBuffer(buffer_out, 0, 0, output_size);

			//Setup, and execute kernels
			cl::Kernel kernel_sum = cl::Kernel(program, "sum");
			kernel_sum.setArg(0, buffer_in);
			kernel_sum.setArg(1, buffer_out);
			kernel_sum.setArg(2, cl::Local(local_size*sizeof(double)));

			//Call all kernels
			queue.enqueueNDRangeKernel(kernel_sum, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size));

			//Copy result from device, to host
			queue.enqueueReadBuffer(buffer_out, CL_TRUE, 0, output_size, &out[0]);

			//Write result
			std::cout << "Result = " << out << std::endl;

		}
		//Catch any potential errors
		catch(catch (cl::Error error) { std::cout << "Error occured: " << error.what() << "," << getErrorString(error.err()) << ""; })
	}



};


