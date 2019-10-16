// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <iostream>
using std::cout;
#include <algorithm>
#include <cassert>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure to
            // initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            // TODO: Add one extra pointer (parent or previous) as desired.
			Node *parent;
	}; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, sz{ 0 } {
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, sz{ 0 } {
        // TODO: Implement this function.
			while (start != end) {
			push(*start);
			++start;
		}
	} // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare }, root{ nullptr }, sz{ 0 } {
        // TODO: Implement this function.
		if (root) {
			std::deque<Node *> dq;
			Node *tmp = root;
			if (tmp->child) {
				dq.push_back(tmp->child);
			}
			root->child = nullptr;
			while (!dq.empty()) {
				tmp = dq.front();
				if (tmp->sibling) {
					dq.push_back(tmp->sibling);
				}
				if (tmp->child) {
					dq.push_back(tmp->child);
				}
				tmp->parent = nullptr;
				tmp->sibling = nullptr;
				tmp->child = nullptr;
				addNode(tmp->getElt());
				dq.pop_front();
			} // while dq not empty
		} // if root
	} // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        // TODO: Implement this function.
		// copy-swap method
		PairingPQ tmp(rhs);
		std::swap(this->root, tmp.root);
		std::swap(this->sz, tmp.sz);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
		if (root) {
			std::deque<Node *> dq;
			Node *tmp = root;
			root = nullptr;
			if (tmp->child) {
				dq.push_back(tmp->child);
			}
			tmp->child = nullptr;
			delete root;
			while (!dq.empty()) {
				tmp = dq.front();
				if (tmp->sibling) {
					dq.push_back(tmp->sibling);
				}
				if (tmp->child) {
					dq.push_back(tmp->child);
				}
				tmp->parent = nullptr;
				tmp->sibling = nullptr;
				tmp->child = nullptr;
				delete tmp;
				dq.pop_front();
			} // while dq not empty
		} // if root
	} // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
		if (root) {
			std::deque<Node *> dq;
			assert(root->sibling == nullptr);
			if (root->child) {
				dq.push_back(root->child);
			}
			root->child = nullptr;
			while (!dq.empty()) {
				Node *tmp = dq.front();
				if (tmp->sibling) {
					dq.push_back(tmp->sibling);
				}
				if (tmp->child) {
					dq.push_back(tmp->child);
				}
				tmp->parent = nullptr;
				tmp->sibling = nullptr;
				tmp->child = nullptr;
				root = meld(root, tmp);
				dq.pop_front();
			} // while dq not empty
		} // if root
	} // updatePriorities()


    // Description: Add a new element to the priority_queue. This is almost done,
    //              in that you should implement push functionality in the addNode()
    //              function, and this function should call addNode().
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: Implement this function.
		addNode(val);
	} // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
		//printNode(root);
		assert(!root->parent);
		assert(!root->sibling);

		if (root) {
			std::deque<Node *> dq;
			Node *tmp = root->child;
			while (tmp) {
				dq.push_back(tmp);
				tmp = tmp->sibling;
			} // tmp

			for (size_t i = 0; i < dq.size(); ++i) {
				tmp = dq[i];
				tmp->parent = nullptr;
				tmp->sibling = nullptr;
			} // for dq

			while (dq.size() > 1) {
				tmp = meld(dq[0], dq[1]);
				dq.pop_front();
				dq.pop_front();
				dq.push_back(tmp);
			} // while dq.size() != 1

			tmp = root;
			tmp->child = nullptr;
			delete tmp;
			if (dq.size() == 1) {
				root = dq.front();
			} else {
				root = nullptr;
			}
			//cout << "|root after pop = ";
			//printPtr(root);
			//cout << "|";
			--sz;
		} // if root
	} // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // TODO: Implement this function

        // These lines are present only so that this provided file compiles.
		//cout << "\nInside top\n";
		if (root) {
			//cout << "top() = " << root->getElt() << "\n";
			return root->getElt();
		} else {
			cout << "Root is nullptr when top is called!\n";
			exit(1);
		}
	} // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function
		return sz;
	} // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function
		return sz == 0;
	} // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        // TODO: Implement this function
		if (!node->parent) {
			node->elt = new_value;
			return;
		}
		Node *par = node->parent;
		Node *tmp = par->child;
		while (tmp) {
			if (tmp->sibling == node) {
				tmp->sibling = node->sibling;
				node->parent = nullptr;
				meld(root, node);
				break;
			}
			tmp = tmp->sibling;
		}
		//Node *par = node->parent;
		//node->elt = new_value;
		//while (par && this->compare(node->getElt(), par->getElt())) {
		//	std::swap(node->elt, par->elt);
		//	node = par;
		//	par = node->parent;
		//}
	} // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE & val) {
        // TODO: Implement this function
		Node *newNode = new Node(val);
		++sz;
		root = meld(root, newNode);
		return newNode;
	} // addNode()


private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
	Node* root;
	size_t sz;

	// Runtime: O(1)
	Node* meld(Node *first, Node *second) {
		// make b the child of a and the subtree of a sibling of b
		//cout << "Inside meld\n";
		//assert(first->sibling == nullptr);
		//assert(second->sibling == nullptr);
		assert(!(first == nullptr && second == nullptr));
		if (second == nullptr) {
			return first;
		}
		if (first == nullptr) {
			return second;
		}
		if (this->compare(second->getElt(), first->getElt())) {
			second->parent = first;
			second->sibling = first->child;
			first->child = second;
			//printNode(first);
			//printNode(second);
			return first;
		}
		first->parent = second;
		first->sibling = second->child;
		second->child = first;
		//printNode(first);
		//printNode(second);
		return second;
	}

	void printNode(const Node *n) {
		if (n == nullptr) {
			cout << "-------------------------------------\n";
			printPtr(n);
			cout << "-------------------------------------\n";
			return;
		}
		cout << "-------------------------------------\n";
		cout << "Node value =  " << n->getElt() << "\n";
		cout << "Node parent: ";
		printPtr(n->parent);
		cout << "Node sibling: ";
		printPtr(n->sibling);
		cout << "Node child: ";
		printPtr(n->child);
		cout << "-------------------------------------\n";
	}

	void printPtr(const Node *n) {
		if (n == nullptr) {
			cout << "NULL\n";
		} else {
			cout << "Node (val = " << n->getElt() << ")\n";
		}
	}
}; // PairingPQ


#endif // PAIRINGPQ_H
