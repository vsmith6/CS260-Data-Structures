//============================================================================
// Name        : HashTable.cpp
// Author      : John Watson/Valerie J. Smith
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
// Course      : CS-260-X6351 Data Structures and Algorithms 21EW6
// Date        : July 22, 2021
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

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
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
	struct Node {
		Bid bid;
		// this value is for the hash
		unsigned key;
		// Node pointer to next node
		Node* next;

		// default constructor
		Node(){
			key = UINT_MAX;  //largest unsigned integer maximum value
			next = nullptr;
		};
		// initialize second constructor that takes a bid
		// will invoke the default constructor
		Node(Bid aBid) : Node() {
			bid = aBid;
		};
		// initialize third constructor with a bid and a key
		Node(Bid aBid, unsigned aKey): Node(aBid){
			key = aKey;
		}
	};

	// create a vector named nodes to hold the nodes
    vector<Node> nodes;

    // create a variable to hold the hash table size - will allow us to change the size
    unsigned tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // Initialize the structures used to hold bids
	//  call resize method of the vector class on the size of the table
	nodes.resize(tableSize);
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // Implement logic to free storage when class is destroyed
	// use the erase method of a vector to free the storage
	nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // Implement logic to calculate a hash value
	// use modulo division to calculate hash value
	return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // Implement logic to insert a bid
	// will call hash method on the bid id passed in
	//  c_str() converts string object to string of characters
	// a to i converts ASCII string to an integer
	unsigned key = hash(atoi(bid.bidId.c_str()));

	// try and retrieve node using the key
	// look for the old node address
	Node* oldNode = &(nodes.at(key));

	// if no entry found for this entry
	if(oldNode == nullptr){
	// if no node is found at this address, insert one with the bid and key
      Node* newNode = new Node(bid, key);
      // insert the node into the vector at this position plus the offset
      nodes.insert(nodes.begin() + key, (*newNode));

	} else {
		// node found with something in it, look at it and see if something is in it
		// if it has not been used, give it the bid and the key
		if(oldNode->key == UINT_MAX){
			// old node key equals key created above
			oldNode->key = key;
			// old node bid equals the bid given
			oldNode->bid = bid;
			// make sure pointer is null
			oldNode->next = nullptr;
		} else {
			// find the next open node(last)
			// if the next node is not null, make it be the next one
			while(oldNode->next != nullptr){
				oldNode = oldNode->next;
			}
			oldNode->next = new Node(bid, key);
		}

	}

}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // Implement logic to print all bids
	for (unsigned int i = 0; i < nodes.size(); i++) {
		Bid bid = nodes[i].bid;
		cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "<< bid.fund << endl;
	}


}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // Implement logic to remove a bid
	unsigned key = hash(atoi(bidId.c_str()));
	nodes.erase(nodes.begin() + key);
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // Implement logic to search for and return a bid
    //calculate the key for this bid
    unsigned key = hash(atoi(bidId.c_str()));

	//try and retrieve node using the key
	Node* node = &(nodes.at(key));

	//if no entry found
	if (node == nullptr || node->key == UINT_MAX) {
		return bid;
	}

	//if node found that matches key
	if (node != nullptr && node->key != UINT_MAX
			&& node->bid.bidId.compare(bidId) == 0) {
		return node->bid;
	}

	//walk the linked list to find the match
	while (node != nullptr) {
		if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
			return node->bid;
		}
		node = node->next;
	}



    return bid;
}

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
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
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
    string csvPath, searchValue;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        searchValue = "98109";
        break;
    case 3:
        csvPath = argv[1];
        searchValue = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        searchValue = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(searchValue);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << searchValue << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(searchValue);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
