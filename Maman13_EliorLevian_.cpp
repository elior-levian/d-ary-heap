

/*******************************************************************
 * 
 * Author: Elior Levian 
 * Program: D-ary Heap Implementation
 * Description: A simple implementation of a d-ary heap that utilises the cpp vector library to build a d-ary heap class.                                                  
 *              Includes a menu to navigate methods such as: inserting a new key, extracting maximum key, and 
 *              increasing the value of an existing key in the heap while maintaining the max-heap property. 
 *              Also contains utility function that prints the heap.
 *******************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>



using namespace std;

#define MAX_CAPACITY 5000
#define MAX_NUM_SIZE 99999
#define MIN_NUM_SIZE -99999

class d_heap{
    /*class for d-ary heap  */
public:
    int d;
    vector<int> heap;

    // d_heap constructor
    d_heap(int chosen_d, vector<int> input_array){
        d = chosen_d;
        heap = input_array;
        build_d_heap();// builds heap 
        cout << "Heap built successfully for d = " << d << " value\n";
    } 
  
    // Get the index of the parent of the node at index i
    int parent(int i) { return (i - 1) / d; }

    // Get the index of the k-th child of the node at index i
    int child(int i, int k) { return d * i + k ; }

    int tree_height(int n, int d){
        // Calculates and returns the tree height by converting log bases  
        return ceil(log(n * (d - 1) + 1) / log(d)) - 1;
    }

    bool is_full(){ return (heap.size()== MAX_CAPACITY) ? true : false;}// returns true if heap reached limit
    

    void d_max_heap(int i){
        /*this function is called recoursively from the build_d_heap function and compares */
        int max = i;
        int child_index;
        for(int k = 1; k <= d; k++){
            // compare every parent with it's k-children, k being 1 to d
            child_index = child(i, k);
            if(child_index < heap.size() && heap[child_index] > heap[max]){
                // change max to be the max value encountered so far  
                max = child_index;
            }
        }
        if(max != i){
            // percolate the largest element up in the heap
            swap(heap[i], heap[max]);
            d_max_heap(max);// recoursive call starting from the tree rooted at index 'max'
        }
    }

     void insert(int value) {
        //Adding new element at the end of heap
        heap.push_back(value);

        //Percolate the element up until the max heap property is restored
        int index = heap.size() - 1;  // The index of the newly added element
        while (index > 0 && heap[index] > heap[parent(index)]) {
            //Swapping the element with its parent 
            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

    void increase_key(int i, int key){
        /*function to increase the value of a certain key in heap*/
        if(i > heap.size()){
            // check if inside's heap bounds, if not doesn't do anything.
            cout << "ERROR, input index reaches out of heap bounds\n";
            return;
        }
        if(key < heap[i]){
            // checking  if input value is smaller then actual value, if so doesn't increase
            cout << "ERROR, value at " << i << "-th index is larger than the input key\n";
            return;
        }
        heap[i] = key;
        while (i > 0 && heap[parent(i)] < heap[i]){
            //percolating the element up as long as it's larger than its parent to preserve the max heap quality
            swap(heap[i], heap[parent(i)]);
            i = parent(i); 
        }
    }
    
    void extract_max(){
        /*function to extract the maximum value out of heap, based on the one in the textbook,
         but using the d-max-heap function*/
        if (heap.empty()){
            // check if heap is empty, if so doesn't do anything
            cout << "ERROR, Heap underflow\n";
            return;
        }

        int max = heap[0];//Max element is at the root
        heap[0] = heap.back(); // Percolate the last  element up to the root
        heap.pop_back(); 

        if (!heap.empty()){
            d_max_heap(0);//Restore the max-heap property starting from the root
        }
        cout << "Extracted max: " << max << "\n";
    }

    void build_d_heap(){
        // 'caller' function for heapify function of d-heap
        int size = (heap.size())/d;
        for (int i = size; i >= 0; i--) {
            d_max_heap(i);
        }
    }


    void print_heap(){
        /*utility function to print the heap, 
        it separates the heap by levels and prints what values are at each level*/
        if(heap.empty()){
            cout << "Heap is empty\n";
            return; 
        }

        int height = tree_height(heap.size(), d);
        int start = 0;
        int node_count = 0;
        cout << "Printing d-heap for d = " << d << "\n";
        cout << "---------------------------\n";
        for (int i = 0; i <= height; i++) {
            cout << "Level " << i << ":\n";
            node_count = pow(d, i);
            for (int level = start; level < start + node_count && level < heap.size(); level++) {
                cout << heap[level] << " "; 
            }
            cout << "\n---------------------------\n";
            start += node_count;
        }
        cout << "Printing as an array:\n";
        for(int num: heap){
            cout << num << ","; 
        }
        cout << "\n";
    }
};

vector<int> read_numbers( const string& file_name){
    /*simple function to implement reading number tokens 
    that are separated by commas from the file */
    bool max_capacity_flag = false; 
    int counter = 0; 
    vector<int> heap;
    ifstream file(file_name);
    if(!file.is_open()){
        cout << "ERROR, could not open file\n";
        return heap;// returns empty d-heap in case of unopened file
    }
    string line; 
    string curr_num;
    int converted_num;
    while(getline(file,line,',')){
        stringstream stream(line);
        while(getline(stream, curr_num, ',')){
            converted_num = stoi(curr_num);
            if(converted_num > MAX_NUM_SIZE || converted_num < MIN_NUM_SIZE){
                // check for invalid input,if indeed invalid, returns array up until invalid value
                return heap;
            }

            heap.push_back(stoi(curr_num));// converts string numbers to integers 
            counter++;
            if(counter == MAX_CAPACITY){
                max_capacity_flag = true;
                return heap;// returns the heap when we reach the max size
                }
            }
        }
    file.close();
    return heap;
}

bool is_valid(int num){
    /*function to check if input number complies with size restrictions
    */
    if(num < MAX_NUM_SIZE && num > MIN_NUM_SIZE){
        return true;
    }
    cout << "Error, Invalid value\n";
    return false;
}

void driver(){
    /* driver function to start the program */
    bool quit = false;
    int d_choice;
    vector<int> heap = read_numbers("numbers.txt");
    cout << "Please enter a positive integer value - d for d-heap,\n";
    cout << "non-integer values will result in malfunction \n";
    cin >> d_choice;

    if (d_choice < 1) {
        cout << "Invalid value for d. d should be >= 1.\n";
        return;
    }

    d_heap curr_d_heap(d_choice, heap);
    int choice, index, value;
    while (!quit) {// setting up menu
        cout << "Choose one of the following by inputting the corresponding number:\n";
        cout << "1) Extract max value from d-heap\n";
        cout << "2) Insert new value to d-heap\n";
        cout << "3) Increase the value of a key at a certain index\n";
        cout << "4) Print current d-heap\n";
        cout << "5) Quit program\n";
        if(cin >> choice){// checking for int only input from user
            switch(choice){
                case 1:
                    cout << "Extracting maximum value from heap...\n";
                    curr_d_heap.extract_max();
                    break;  
                case 2:
                    cout << "Enter new (5 digit max) integer value to insert into d-heap\n";
                    cin >> value;
                    if (is_valid(value) && !curr_d_heap.is_full()) {
                        curr_d_heap.insert(value);
                    }
                    break;  
                case 3:
                    cout << "Enter the index of the key you want to change\n";
                    cin >> index;
                    cout << "Enter the new value:\n";
                    cin >> value;
                    if (is_valid(value)) {
                        curr_d_heap.increase_key(index, value);
                    }
                    break;
                case 4:
                    curr_d_heap.print_heap();
                    break;      
                case 5:
                    cout << "Have a nice day!\nQuitting program...\n";
                    quit = true;
                    break;  
                default:// handles invalid inputs by user
                    cout << "Invalid choice. Please choose a valid option.\n";
                    break; 
            }
        }
        else{
            cout << "Invalid input, please enter an integer.\n";
            // clears buffer in case of non numerical input
            cin.clear();
            cin.ignore(1000,'\n');
        }
    }
}

int main(){
    driver();
    return 1; 
}
