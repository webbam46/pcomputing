//Compute histogram of the given data
__kernel void hist(__global const int * data,__global int * hist,int nr_bins){
	//Get the global id
	int id = get_global_id(0);

	//Zero bin counts
	if(id < nr_bins){
		//Ensure initialised to 0
		hist[id] = 0;
	}

	//Sync
	barrier(CLK_GLOBAL_MEM_FENCE);

	//Set bin index
	int bin_index = A[id];

	//Use bin_index to increment histogram value
	atomic_inc(&H[bin_index]);
}

//Compute the sum of the given data
__kernel void sum(__global const int * data,__global int * out,__local int * buffer){
	int id = get_global_id(0); //Global ID
	int local_id = get_local_id(0); //Local ID
	int local_size = get_local_size(0); //Local size

	//Store values from global memory into local memory (buffer)
	buffer[local_id] = data[id];

	//Now wait for all local threads to finish
	barrier(CLK_LOCAL_MEM_FENCE);

	//Calculate sum using reduction
	for (int i = 0; i < local_size;i *= 2){
		if (!local_id % (i * 2)) && ((local_id + 1) < local_size)){
			buffer[local_id] += buffer[local_id + 1];
		}
	}

	//Now copy from the buffer - to the output array
	out[id] = buffer[id];
}

//Compute the max of the given data
__kernel void max(__global const int * data,__global int * out,__local int * buffer){
	int id = get_global_id(0); //Global ID
	int local_id = get_local_id(0); //Local ID
	int local_size = get_local_size(0); //Local size

	//Store values from global memory - into local memory (buffer)
	buffer[local_id] = data[id];

	//Calculate max using reduction
	for (int i = 0; i < local_size;i *= 2){
		if (!local_id % (i * 2)) && ((local_id + 1) < local_size)){
			if(buffer[local_id] < buffer[local_id + 1]){
				buffer[local_id] = 0;
			}
		}
	}

	//Now copy from the buffer - to the output array
	out[id] = buffer[id];

}

//Compute the min of the given data
__kernel void max(__global const int * data,__global int * out,__local int * buffer){
	int id = get_global_id(0); //Global ID
	int local_id = get_local_id(0); //Local ID
	int local_size = get_local_size(0); //Local size

	//Store values from global memory - into local memory (buffer)
	buffer[local_id] = data[id];

	//Calculate max using reduction
	for (int i = 0; i < local_size;i *= 2){
		if (!local_id % (i * 2)) && ((local_id + 1) < local_size)){
			if(buffer[local_id] > buffer[local_id + 1]){
				buffer[local_id] = 0;
			}
		}
	}

	//Now copy from the buffer - to the output array
	out[id] = buffer[id];

}



//Gather data
__kernel void sortdata(__global const char * lines,__global char * place,__global double  * year,__global double * month,__global double * day,__global double * time,__global double * temp){
	int id = get_global_id(0);
	//printf("%i",id);
	//char * line = lines[id];
	//printf("%s\n",line);

	


	//printf("%c",&line);
	
	//place[id] = " ";
	//year[id] = 0.0;
	//month[id] = 0.0;
	//day[id] = 0.0;
	//time[id] = 0.0;
	//temp[id] = 0.0;
}