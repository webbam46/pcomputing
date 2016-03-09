
//Add input vectors
__kernel void add(__global const int * A,__global const int * B,__global int * C){
	int id = get_global_id(0);
	C[id] = A[id] + B[id];
}
//Mult input vectors
__kernel void mult(__global const int * A,__global const int * B,__global int * C){
	int id = get_global_id(0);
	C[id] = A[id] * B[id];
}

//Kernel for gathering the max temperature value using reduce
__kernel void maxtemp(__global const double * temperatures,__global double * result){
	int id = get_global_id(0);
	int N = get_global_size(0);

	
	barrier(CLK_GLOBAL_MEM_FENCE);

	int stride = 10;
	for(int i = 1; i < N;i *= stride){
		if (!(id % (i * stride)) && ((id + i) < N)){
			if(temperatures[id] > result[id]){
				result[id] = temperatures[id];
			}
		} 

		barrier(CLK_GLOBAL_MEM_FENCE);
	
	}

	

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