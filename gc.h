#ifndef GC_H
#define GC_H

// A single threaded garbage collector

#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>

using namespace std;

using uint_t = unsigned int;
// To watch the action of the garbage collector, define DISPlay.
// #define DISPLAY

/**
	Exception thrown when an attempt is made to 
	use an Iter that exceeds the range of the 
	underlying object
*/
class OutOfRangeExc{

};

/**
	an iterator-like class for cycling through arrays
	that are pointed to by GCPtrs. Iter pointers
	** do not ** participate in or affect  garbage
	collection. Thus, an Iter point to some object
	does not prevent that object from being recycled
*/
template<class T> 
class Iter{
private:
	T* ptr;		// current pointer value
	T* end;		// points to element one past end
	T* begin	// points to start of allocated array
		uint_t length // length of sequence

		void checkInvariants(){
			if (ptr >= end || ptr < begin)
				throw OutOfRangeExc();
		}

public:
	Iter(){
		ptr = end = begin = nullptr;
		length = 0;
	}

	Iter(T* p, T* first, T* last){
		ptr = p;
		end = last;
		begin = first;
		length = last - first;
	}

	Iter(Iter& copy){
		ptr = copy.ptr;
		end = copy.end;
		begin = copy.begin;
		length = copy.length;
	}

	uint_t size() {
		return length;
	}

	T& operator *(){
		checkInvariants();
		return *ptr;
	}

	T* operator ->(){
		checkInvariants();
		return ptr;
	}

	// prefix
	Iter operator ++(){
		ptr++;
		return *this;
	}

	// prefix
	Iter operator --(){
		ptr--;
		return *this;
	}

	// postfix
	Iter operator ++(int notUsed){
		T * tmp = ptr;

		ptr++;
		return Iter<T>(temp, begin, end);
	}


	// postfix
	Iter operator --(int notUsed){
		T * tmp = ptr;

		ptr--;
		return Iter<T>(temp, begin, end);
	}

	T& operator [](int i){
		if (i < 0 || i >= (end - begin))
			throw OutOfRangeExc();
		return ptr[i];
	}

	bool operator ==(Iter& other){
		return ptr == other.ptr;
	}

	bool operator !=(Iter& other){
		return ptr != other.ptr;
	}

	bool operator <(Iter& other){
		return ptr < other.ptr;
	}

	bool operator <=(Iter& other){
		return ptr <= other.ptr;
	}

	bool operator >(Iter& other){
		return ptr > other.ptr;
	}

	bool operator >=(Iter& other){
		return ptr >= other.ptr;
	}

	Iter operator -(int n){
		ptr -= n;
		return *this;
	}

	Iter operator +(int n){
		ptr += n;
		return *this;
	}

	// Return number of elements between two iters.
	int operator -(Iter<T>& other){
		return ptr - other.ptr;
	}
};

/**
	This class defines an element that is stored
	in the garbage collection information list
*/
template <class T>
class GCInfo{
public:
	uint_t refcount; // current reference count
	T* memPtr;	// pointer to allocated memory

	/**
		isArray is true if memPtr points
		to an allocated array. It is false
		otherwise
	*/
	bool isArray;	

	/*
		If memPtr is pointing to an allocated
		array, then arraySize contains its size */
	uint_t arraySize

};

#endif // !GC_H
