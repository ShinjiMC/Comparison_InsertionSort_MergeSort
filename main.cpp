#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <chrono>

using namespace std;

std::vector<double> GetPoints(const int& d){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0);
    std::vector<double> points;
    for (int n = 0; n < d; ++n){
        points.push_back(dis(gen));
    }
    return points;
}

double InsertionSort(const std::vector<double>& point) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> points = point;
    for (int i = 1; i < points.size(); ++i) {
        int cur_value = points[i];
        int j = i - 1;
        
        while (j >= 0 && points[j] > cur_value) {
            points[j + 1] = points[j];
            j -= 1;
        }
        
        points[j + 1] = cur_value;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

void merge(std::vector<double>& array, int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    std::vector<double> left(n1);
    std::vector<double> right(n2);

    for (int i = 0; i < n1; ++i) {
        left[i] = array[low + i];
    }
    for (int i = 0; i < n2; ++i) {
        right[i] = array[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = low;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            array[k] = left[i];
            ++i;
        } else {
            array[k] = right[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        array[k] = left[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        array[k] = right[j];
        ++j;
        ++k;
    }
}

void mergeSort(std::vector<double>& array, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSort(array, low, mid);
        mergeSort(array, mid + 1, high);
        merge(array, low, mid, high);
    }
}

double getTimerge(const std::vector<double>& point){
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> points = point;
    mergeSort(points,0,points.size()-1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

void LoadFile(const int& n){
    std::ofstream outputFile1("Lineal/Insertion",std::ios::trunc);
    std::ofstream outputFile2("Lineal/Merge",std::ios::trunc);
    if (!outputFile1.is_open() || !outputFile2.is_open()) {
        std::cerr << "Error opening the file for writing." << std::endl;
        return;
    }
    for(int i = 0; i < n; ++i){
        if (i==0){
            outputFile1 << "0 ";
            outputFile2 << "0 ";
        }
        std::vector<double> points = GetPoints(i);
        outputFile1 << InsertionSort(points) << " ";
        outputFile2 << getTimerge(points) << " ";
    }
    outputFile1.close();
    outputFile2.close();
}

int InsertInt() {
    int num;
    while (true) {
        std::cout << "Enter an integer of data points (between 10 and 2500): ";
        if (std::cin >> num) {
            if (num >= 10 && num <= 2500) {
                return num; // El número está en el rango deseado
            } else {
                std::cout << "Number out of range. Please try again." << std::endl;
            }
        } else {
            std::cout << "Number out of range. Please try again." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}
int 
main(){
    int n = InsertInt();
    LoadFile(n);
    int result = std::system(".\\Lineal\\Lineal.exe");
    if (result != 0) {
        std::cerr << "Error executing the plotting in GO." << std::endl;
    }
    std::remove(".\\Lineal\\Insertion");
    std::remove(".\\Lineal\\Merge");
    int results = std::system(".\\Lineal.png");
    if (results != 0) {
        std::cerr << "Error opening the image." << std::endl;
    }
    return 0;
}

//Credits ShinjiMC