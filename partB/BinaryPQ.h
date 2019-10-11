// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include <utility>
using std::swap;
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
		TYPE dummy = TYPE();
		data.resize(1, dummy);
	} // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data{ start, end } {
			// TODO: Implement this function.
		TYPE dummy = TYPE();
		data.insert(begin(data), dummy);
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    // JC: use bottom-up fixDown()
	virtual void updatePriorities() {
        // TODO: Implement this function.
		for (unsigned i = (unsigned)size() / 2; i != 0 ; --i) {
			fixDown(i);
		}
    } // updatePriorities()


		// Description: Add a new element to the heap.
    // Runtime: O(log(n))
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: Implement this function.
		data.push_back(val);
		fixUp();
	} // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
		data[1] = data[size()];
		data.pop_back();
		fixDown(1);
	} // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // TODO: Implement this function.

        // These lines are present only so that this provided file compiles.
		return data.back();
	} // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
		return data.size() - 1;
	} // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
		return size() == 0;
	} // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

    // TODO: Add any additional member functions or data you require here.
    // For instance, you might add fixUp() and fixDown().
	void fixUp() {
		size_t k = size();
		//std::cout << "k = " << k << "\n";
		while (k > 1 && data[k / 2] < data[k]) {
			swap(data[k], data[k / 2]);
			k /= 2; // move up to parent
		} // while
	} // fixUp

	void fixDown(unsigned k) {
		while (2 * k <= size()) {
			unsigned j = 2 * k; // start with left child
			if (j < size() && data[j] < data[j + 1]) ++j;
			if (data[k] >= data[j]) break; // heap restored
			swap(data[k], data[j]);
			k = j; // move down
		} // while
	} // fixDown
}; // BinaryPQ


#endif // BINARYPQ_H
