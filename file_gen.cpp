#include <iostream>

#include <fstream>

#include <cstdlib>

#include <ctime>



int main() {

    // Open a file named "output.txt" for writing

    std::ofstream outFile("output1mil.txt");



    if (!outFile) {

        std::cerr << "Error opening file for writing." << std::endl;

        return 1;

    }



    // Seed the random number generator with the current time

    std::srand(std::time(0));



    // Write 10,000 random integers to the file

    for (int i = 0; i < 1024000; ++i) {

        outFile << std::rand() << std::endl;

    }



    // Close the file

    outFile.close();



    std::cout << "File created successfully." << std::endl;



    return 0;

}


