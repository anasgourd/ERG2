#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <mpi.h>

#include <assert.h>

#include <time.h>

#include <iostream>

#include <fstream>

#include <vector>
#include <unordered_set>


using namespace std;



#define  MASTER     0



std::vector<int> read_array_from_file(const char* filename) {

    std::ifstream file(filename);

    std::vector<int> array;



    if (file.is_open()) {

        int value;

        while (file >> value) {

            array.push_back(value);

        }

        file.close();

    }

    else {

        std::cerr << "Unable to open file: " << filename << std::endl;

        MPI_Abort(MPI_COMM_WORLD, 1);

    }



    return array;

}

double get_elapsed_time(MPI_Comm comm = MPI_COMM_WORLD) {

    return MPI_Wtime();

}



void sort_array(int* array, int length) {

    for (int i = length - 1; i > 0; --i) {

        for (int j = 0; j < i; ++j) {

            if (array[j] > array[j + 1]) {

                int temp = array[j];

                array[j] = array[j + 1];

                array[j + 1] = temp;

            }

        }

    }

}



void print_array(int* array, int length) {

    printf("[");

    for (int i = 0; i < length; i++) {

        if (i == length - 1) {

            printf("%d", array[i]);

        }

        else {

            printf("%d,", array[i]);

        }

    }

    printf("]");

}

int get_array_sum(int* array, int length) {

    int sum = 0;

    for (int i = 0; i < length; i++) {

        sum += array[i];

    }

    return sum;

}



int get_array_median(int* array, int length) {



    if (length < 1) return 0;



    

    int* sorted = new int[length];

    for (int i = 0; i < length; ++i) {

        sorted[i] = array[i];

    }

    sort_array(sorted, length);



    if (length > 1) {

       

        int m = 0;

        m = (int)ceil((double)length / 2);

        int median = sorted[m];

        free(sorted);

        return median;

    }

    else {

        int median = sorted[0];

        free(sorted);

        return median;

    }

}

int get_array_weighted_median(int* mos, int* nos, int length) {

    //mos is median of each subarray

    //nos is number of elemnets in each subarray

    int N = get_array_sum(nos, length);



    // sort

    for (int i = length - 1; i > 0; --i) {

        for (int j = 0; j < i; ++j) {

            if (mos[j] > mos[j + 1]) {

                int temp = mos[j];

                mos[j] = mos[j + 1];

                mos[j + 1] = temp;

                temp = nos[j];

                nos[j] = nos[j + 1];

                nos[j + 1] = temp;

            }

        }

    }



    

    double S = 0; // weights sum

    double* weights = new double[length];

    for (int i = 0; i < length; ++i) {

        weights[i] = (double)nos[i] / (double)N;

        S += weights[i];

    }



    int k = 0;

    double sum = S - weights[0]; 

    while (sum > S / 2) {

        ++k;

        sum -= weights[k];

    }

    return mos[k];

}








int main(int argc, char** argv) {





    int rank, size;             

    //int* elements = NULL;       // array containing the elements

    std::vector<int> elements;

    int* proc_elements = NULL;  // array containing the elements of each process

    int elements_count;         

    int k;                      

    int c=1;



    MPI_Status status;

    if (argc != 3) {

        printf("Incorrect number of arguments\n");



        return 0;

    }

    elements_count = atoi(argv[1]);

    k = atoi(argv[2]);

    c = 1;



    if (k > elements_count) {

        printf("k cant't be larger than the array size!\n");

        return 0;

    }

   

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);

    std::unordered_set<std::string> uniqueHosts;



    // Add the current host name to the set

    uniqueHosts.insert(processor_name);

    double start_time = get_elapsed_time();

    if (elements_count % size != 0) {

        

        printf("number of array elements NOT dividable by number of processes!\n");

        MPI_Abort(MPI_COMM_WORLD, 0);

        return 0;

    }

    if (rank == MASTER) {

        

        elements = read_array_from_file("output1mil.txt");

        printf("%d elements.size",elements.size());

        if (elements.size() != elements_count) {

            std::cerr << "Mismatch in array size and specified size" << std::endl;

            MPI_Abort(MPI_COMM_WORLD, 1);

        }

        printf("Array from file\n");

        //print_array(elements.data(), elements_count);

        printf("\n*****************************************************\n");

        printf("\n");

        fflush(stdout);

        





    }

    //process elements count

    int ni = elements_count / size;



    //the array chunk of each process

    proc_elements = new int[ni];

    



    

    MPI_Scatter(elements.data(), ni, MPI_INT, proc_elements, ni, MPI_INT, MASTER, MPI_COMM_WORLD);



    

    //free(elements.data());



   

    int N = elements_count;



    

    int M;





    int* mis = NULL;    // array of medians of each process

    int* nis = NULL;    // array of elements count of each process



    while (!(N <= (double)elements_count / (double)(c * size))) {

        if (rank == MASTER) {



            fflush(stdout);

        }

        MPI_Barrier(MPI_COMM_WORLD);







        int mi = get_array_median(proc_elements, ni);

        if (rank == MASTER) {

            mis = new int[size];

            nis = new int[size];

        }

        MPI_Gather(&mi, 1, MPI_INT, mis, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

        MPI_Gather(&ni, 1, MPI_INT, nis, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

        int weighted_median;

        if (rank == MASTER) {

            weighted_median = get_array_weighted_median(mis, nis, size);

        }



        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Bcast(&weighted_median, 1, MPI_INT, MASTER, MPI_COMM_WORLD);





        int local_num[3] = { 0, 0, 0 };//number of elements in each processor lower,equal,bigger than M

        for (int i = 0; i < ni; i++) {

            if (proc_elements[i] < weighted_median)

                local_num[0]++;

            else if (proc_elements[i] == weighted_median)

                local_num[1]++;

            else

                local_num[2]++;

        }



        int global_num[3] = { 0, 0, 0 };

        MPI_Allreduce(&local_num, &global_num, 3, MPI_INT, MPI_SUM, MPI_COMM_WORLD);





        MPI_Barrier(MPI_COMM_WORLD);

        







        M = weighted_median;



        if (global_num[0] < k && k <= (global_num[0] + global_num[1])) {

            break;

        }

        

        else if (k < global_num[0]) {

            int* temp_array = new int[local_num[0]];

            int j = 0;

            for (int i = 0; i < ni; i++) {

                if (proc_elements[i] < weighted_median) {

                    temp_array[j] = proc_elements[i];

                    j++;

                }

            }

            ni = local_num[0];

            free(proc_elements);

            proc_elements = temp_array;



            N = global_num[0];

        }

        else if (k > (global_num[0] + global_num[1])) {

            int* temp_array = new int[local_num[2]];

            int j = 0;

            for (int i = 0; i < ni; i++) {

                if (proc_elements[i] > weighted_median) {

                    temp_array[j] = proc_elements[i];

                    j++;

                }

            }

            ni = local_num[2];

            free(proc_elements);

            proc_elements = temp_array;



            N = global_num[2];

            k = k - (global_num[0] + global_num[1]);

        }





        fflush(stdout);

    }

   

    if (rank == MASTER) {

        nis = new int[size];

    }

    MPI_Gather(&ni, 1, MPI_INT, nis, 1, MPI_INT, MASTER, MPI_COMM_WORLD);



    int* pos = new int[size];

    int s;

    if (rank == MASTER) {

        pos[0] = 0;

        for (int i = 1; i < size; i++) {

            pos[i] = nis[i - 1] + pos[i - 1];   //shows where in the final array each process works

        }

        s = get_array_sum(nis, size);

        elements.resize(s);

    }

    

    MPI_Gatherv(proc_elements, ni, MPI_INT, elements.data(), nis, pos, MPI_INT, MASTER, MPI_COMM_WORLD);



    // wait for other processes

    MPI_Barrier(MPI_COMM_WORLD);



    if (rank == MASTER) {

        sort_array(elements.data(), s);

        

        

        printf("\n");

        

        printf("The k-th(k=%d) smallest element is %d ", k, elements[k - 1]);

        

        printf("\n");

        fflush(stdout);

    }



    

    free(mis);

    free(nis);

    free(proc_elements);

    double end_time = get_elapsed_time();



    if (rank == MASTER) {

        printf("Total execution time: %f seconds\n", end_time - start_time);
	std::cout << "Number of processes are: " << size << std::endl;
	 std::cout << "Number of different host servers: " << uniqueHosts.size() << std::endl;

    }

   



    MPI_Finalize();

    

   



}
