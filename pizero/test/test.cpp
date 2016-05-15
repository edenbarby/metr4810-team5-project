#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

int main(int argc, char **argv)
{
    std::cout << CV_VERSION << std::endl;
    std::cout << CV_MAJOR_VERSION << std::endl;
    std::cout << CV_MINOR_VERSION << std::endl;
    std::cout << CV_SUBMINOR_VERSION << std::endl;
    return 0;
}