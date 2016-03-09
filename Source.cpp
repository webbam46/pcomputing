#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "Utils.h"





int main(int argc, char ** argv) {
	//Detect potential exceptions
	try
	{
		/*
			Initial setup
		
		*/
		//Platform ID
		int platform_id = 0;
		//Device ID
		int device_id = 0;

		//Select computing devices
		cl::Context context = GetContext(platform_id, device_id);

		//Display the running device
		DisplayRunningDevice(platform_id, device_id);

		//Create a queue which will be used to push commands to the device
		cl::CommandQueue queue(context);

		//Attempt to build kernel code
		cl::Program program;
		try {
			program = BuildKernelCode(context, "kernels.cl");
		}
		catch (std::exception e) { std::cout << e.what() << std::endl; }


		/*
			Memory Allocation

		*/

		//Input vectors
		std::vector<int> A = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::vector<int> B = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };

		size_t vector_elements = A.size(); //Number of elements
		size_t vector_size = A.size()*sizeof(int); //Size in bytes

		//Output vectors
		std::vector<int> C(vector_elements);

		//Device buffers
		cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, vector_size);
		cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, vector_size);
		cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, vector_size);

		/*
		
			DEVICE OPERATIONS
		
		*/

		//Copy input arrays to device memory
		queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, vector_size, &A[0]);
		queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, vector_size, &B[0]);

		//Setup, and execute the kernel (device code)
		cl::Kernel kernel_add = cl::Kernel(program, "mult");
		//Set arguments
		kernel_add.setArg(0, buffer_A);
		kernel_add.setArg(1, buffer_B);
		kernel_add.setArg(2, buffer_C);

		queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(vector_elements), cl::NullRange);

		/*
		
			RESULTS
		*/

		//Copy the result from device to host
		queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, vector_size, &C[0]);

		//Display results to console
		std::cout << "A = " << A << std::endl;
		std::cout << "B = " << B << std::endl;
		std::cout << "C = " << C << std::endl;

	}
	catch (cl::Error error) { std::cout << "Error occured: " << error.what() << "," << getErrorString(error.err()) << ""; }


	cin.get();
	return 0;
}