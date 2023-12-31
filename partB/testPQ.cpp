// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043
/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to test updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * These tests are NOT a complete test of your priority queues!
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <numeric>

#include "BinaryPQ.h"
#include "Eecs281PQ.h"
#include "UnorderedPQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"

using namespace std;


// Used to test a priority queue containing pointers to integers.
struct IntPtrComp {
    bool operator() (const int *a, const int  *b) const {
        return *a < *b;
    }
};

// JC: used to test a priority queue containing unsigned integers.
struct UnsignedIntComp {
	bool operator() (const unsigned a, unsigned int b) const {
		return a < b;
	}
};

// TODO: Make sure that you're using this-compare() properly, and everywhere
// that you should.  Complete this function by adding a functor that compares
// two HiddenData structures, create a PQ of the specified type, and call
// this function from main().
void testHiddenData(const string &pqType) {
    struct HiddenData {
        int data;
    };
    struct HiddenDataComp {
        bool operator()(const HiddenData &a, const HiddenData &b) const {
            // TODO: Finish this functor; when you do, uncomment the
            // parameters in the line above
            return a.data < b.data;
        }
    };

    cout << "Testing " << pqType << " with hidden data\n";
} // testHiddenData()

// Print out contents of a PQ in one line
// Implemented by Johnny Chan as a helper function
void printPQHelper(Eecs281PQ<int> *pq) {
	std::deque<int> temp;
	cout << "Priority Queue => ";
	if (pq->empty()) {
		cout << "(Empty)";
	}
	while (!pq->empty()) {
		cout << pq->top() << " ";
		temp.push_back(pq->top());
		pq->pop();
	}
	//cout << "|PQ empty|";
	assert(pq->empty());
	while(!temp.empty()) {
		//cout << "pushing " << temp.back() << "\n";
		pq->push(temp.back());
		temp.pop_back();
	}
	//cout << "|PQ refilled|";
	cout << "\n";
}

// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq) {
    vector<int> data{ 1, 5 };
	vector<int> johnny_primes{ 19, 7, 17, 23, 11, 13 };
	vector<int> ret = { 23, 19, 17, 13, 11, 10, 7, 5 };
	pq->push(&data[0]);
    pq->push(&data[1]);
    data[0] = 10;
    //cout << "Before calling updatePriorities()\n";
	pq->updatePriorities();
    assert(*pq->top() == 10);
	for (size_t i = 0; i < johnny_primes.size(); ++i) {
		pq->push(&johnny_primes[i]);
	}
	pq->updatePriorities();
	for (size_t i = 0; i < ret.size(); ++i) {
		assert(*pq->top() == ret[i]);
		pq->pop();
	}
	//cout << __func__ << " succeeded!\n";
} // testUpdatePrioritiesHelper()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePriorities(const string &pqType) {
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on " << pqType << endl;

    // TODO: Add more types here
    if (pqType == "Unordered") {
        pq = new UnorderedPQ<int *, IntPtrComp>;
    } else if (pqType == "Sorted") {
		pq = new SortedPQ<int *, IntPtrComp>;
	} else if (pqType == "Binary") {
		pq = new BinaryPQ<int *, IntPtrComp>;
	} else if (pqType == "Pairing") {
		pq = new PairingPQ<int *, IntPtrComp>;
	} else {
		cout << "You shouldn't be here!\n";
		exit(1);
	}

    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType) {
    cout << "Testing priority queue: " << pqType << endl;

    pq->push(3);
	//printPQHelper(pq);
    assert(pq->top() == 3);
	assert(pq->size() == 1);
	pq->push(4);
    //printPQHelper(pq);
    assert(pq->top() == 4);
    assert(pq->size() == 2);

    pq->pop();
    //printPQHelper(pq);
    assert(pq->size() == 1);
    assert(pq->top() == 3);
    assert(!pq->empty());

    pq->pop();
    //printPQHelper(pq);
    assert(pq->size() == 0);
    assert(pq->empty());

    // TODO: Add more testing here!
	/*
	const int NUM = 100;
	for (int i = 0; i < NUM; ++i) {
		pq->push(i * 10);
		assert(pq->size() == (size_t)i+1);
		assert(pq->top() == i * 10);
	}
	for (int i = NUM-1; i >= 0; --i) {
		assert(pq->top() == i * 10);
		pq->pop();
		assert((int)pq->size() == i);
	}
	*/
	// Testing range-based constructed PQ
	//const size_t RANGE = 10000;
	//vector<unsigned int> v(RANGE);
	//iota(v.begin(), v.end(), 0);
	//if (pqType == "Binary") {
	//	BinaryPQ<unsigned int> *bpq = new BinaryPQ(v.begin(), v.end(), UnsignedIntComp());
	//	assert(bpq->top() == RANGE - 1);
	//	assert(bpq->size() == RANGE);
	//	delete bpq;
	//}

    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()


// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().  Still not complete, should have more code, test
// addNode(), updateElt(), etc.
void testPairing(vector<int> & vec) {
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    //printPQHelper(pq1);
	Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    //printPQHelper(pq2);
	// This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    *pq3 = *((PairingPQ<int> *)pq2);

    pq1->push(3);
    pq2->pop();
    //printPQHelper(pq2);
	assert(pq2->top() == 0); // JC
	assert(pq2->size() == 1); // JC
    assert(pq1->size() == 3);
	assert(pq1->top() == 3); // JC
    assert(!pq1->empty());
    assert(pq1->top() == 3);
    pq2->push(pq3->top());
    assert(pq2->top() == pq3->top());
	assert(pq3->top() == 1); // JC
	assert(pq2->top() == 1); // JC

    cout << "Basic tests done, calling destructors" << endl;

    delete pq1;
    delete pq2;
    delete pq3;

    cout << "testPairing() succeeded" << endl;
} // testPairing()


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{ "Unordered", "Sorted", "Binary", "Pairing" };
    int choice = 3;

    //cout << "PQ tester" << endl << endl;
    //for (size_t i = 0; i < types.size(); ++i)
    //    cout << "  " << i << ") " << types[i] << endl;
    //cout << endl;
    //cout << "Select one: ";
    //cin >> choice;

    if (choice == 0) {
        pq = new UnorderedPQ<int>;
    } // if
    else if (choice == 1) {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2) {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3) {
        pq = new PairingPQ<int>;
    } // else if
    else {
        cout << "Unknown container!" << endl << endl;
        exit(1);
    } // else

    testPriorityQueue(pq, types[(size_t)choice]);
	testUpdatePriorities(types[(size_t)choice]);

    if (choice == 3) {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        testPairing(vec);
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()
