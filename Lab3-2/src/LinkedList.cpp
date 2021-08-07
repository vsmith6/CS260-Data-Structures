//============================================================================
// Name        : LinkedList.cpp
// Author      : Valerie J. Smith
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
// Course      : CS-260-X6351 Data Structures and Algorithms 21EW6
// Date        : July 11, 2021
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
    // constructor will be automatically called when a new bid is created
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    // Internal structure for list entries, housekeeping variables
  struct Node {
    Bid bid;
    Node *next;

     // default constructor
    Node() {
    	next = nullptr;
    }
    // constructor used to initialize a node with a bid
    Node(Bid aBid){
    	bid = aBid;
    	next = nullptr;
    }


  };
  // node pointer to head of list
  Node *head;
  // node pointer to tail of list
  Node *tail;
  int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // Initialize housekeeping variables
	// shortcut for both head =nullptr and tail = nullptr
	head = tail = nullptr;


}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // Implement append logic
	// create a node pointer from the bid that's passed in
   Node *node = new Node(bid);

   if(head == nullptr){
	   head = node;
   } else {
	   if(tail != nullptr){
		   tail->next = node;

	   }
   }
   // new node is always the tail, sets the tail of the linked list to be the node pointer node
   tail = node;
   // increment the size of the list with every bid added
   size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // Implement prepend logic
	// create a node pointer from the bid that's passed in
	  Node *node = new Node(bid);
	  //if head is not null, we want the bid we are appending to be the head of the list and it's next pointer points to what was
	  //already in the head
	  if(head != nullptr){
		  node-> next = head;
	  }
     head = node;
     // increment size of list because of inserting
     size++;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // Implement print logic
	// create a new node pointer current that points to head of list
	Node *current = head;
	// loop over each node and print the information for each bid that is in the list
	while(current != nullptr){
		cout << current->bid.bidId << ": " << current-> bid.title<< " | "<<current-> bid.amount <<" | "<< current-> bid.fund <<endl;
		current = current->next;
	}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
   // Implement remove logic
   // case for when item to be removed is in the head position
	if (head != nullptr) {
		if (head->bid.bidId.compare(bidId) ==0) {
			Node* tempNode = head->next;
			delete head;
			head = tempNode;
		}
	}
   // create a node pointer to a current position for the head
	Node* current = head;

	// loop over each node looking for a match
	while (current->next != nullptr) {
		if (current->next->bid.bidId.compare(bidId) == 0) {
			//save the next node (one to be removed)
			Node* tempNode = current->next;

			// made current node point beyond the next one (to be removed)
			current->next = tempNode->next;

			//delete the temp node now that the next one has been set
			delete tempNode;

			// reduce count
			size--;

			return;
		}
		current = current->next;
	}

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
//    // Implement search logic

	//point to the head of the list
	   Node* current = head;
	   // set a new node to return an empty bid
	   Node* bidHolder = new Node;
	   bidHolder->bid.bidId = "";
	   // search through and find the bid Id that matches what is passed in
	   while(current != nullptr){
	       // if bid id is found, return it
	       if(current->bid.bidId == bidId){
	           return current->bid;
	       }
	       current = current->next;
	   }
	   return bidHolder->bid;

}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
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
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
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
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  6. Prepend Bid"<< endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);

            break;

        case 2:
            ticks = clock();

            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 3:
            bidList.PrintList();

            break;

        case 4:
            ticks = clock();

            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            // this did not work with the instructions given from the video as there was no return type to the Search() method
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 5:
            bidList.Remove(bidKey);

            break;

        case 6:
            bid = getBid();
            bidList.Prepend(bid);
            displayBid(bid);

            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
