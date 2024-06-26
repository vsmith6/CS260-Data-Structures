//============================================================================
// Name        : VectorSorting.cpp
// Author      : Valerie J. Smith
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Vector Sorting Algorithms
// Course      : CS-260-X6351 Data Structures and Algorithms 21EW6
// Date        : July 22, 2021
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

// Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
	// set variables for high and low ends of bid vector
	int low = begin;
	int high = end;

	// pick the middle element as the pivot point
	int pivot = (begin + (end - begin) / 2);

    bool done = false;

    while(!done){

    	// keep incrementing low(or left side) as long as it is less than the pivot
        while(bids.at(low).title.compare(bids.at(pivot).title) < 0){
        	++low;
        }
    	// keep decrementing high(or right side) as long as it is less than the pivot
        while(bids.at(pivot).title.compare(bids.at(high).title) < 0){
        	--high;
        }
        if(low >=high){
        	done = true;
        } else {
        	// swap the low and high bids using built in vector method
        	swap(bids.at(low), bids.at(high));

            // move endpoints closer
            ++low;
            --high;
        }

    }

	return high;

}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
	// midpoint for the variables
	unsigned int midPoint = 0;

	// if 0 or 1 bids left to sort, then finish
    if(begin >= end){
    	return;
    }
    // partition bids into low and high parts
   midPoint = partition(bids, begin, end);

   // recursively call quicksort using midpoint value-(begin point to midpoint)
   quickSort(bids, begin, midPoint);
   // recursively call quicksort using midpoint value-(midpoint to end)
   quickSort(bids, midPoint + 1, end);
}

// Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
	// start with a min index for the outer part of the list
	int min;
	// pos is the position within the bids that marks sorted/unsorted
    for (unsigned pos = 0; pos < bids.size(); ++pos){
    	//saving min spot as 0
     min = pos;
        // inner index search throughout the rest of the loop
        for(unsigned j = pos + 1; j < bids.size(); ++j){
        	if(bids.at(j).title.compare(bids.at(min).title) < 0){
        		min = j;

        	}
        }
        // if we found something smaller than our current index, swap their position
        if(min != pos){
        	swap(bids.at(pos), bids.at(min));
        }
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

        // Invoke the selection sort and report timing results
            // Initialize a timer variable before loading bids
        case 3:

            ticks = clock();

            // Complete the method call to sort the bids
           selectionSort(bids);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        // Invoke the quick sort and report timing results
        case 4:

            ticks = clock();

            // Complete the method call to sort the bids
           quickSort(bids, 0, bids.size() - 1);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
