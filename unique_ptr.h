#ifndef __UNIQUE_PTR_H__
#define __UNIQUE_PTR_H__

#include <iostream>


template <typename T>
class UniquePtr
{
	T* _ptr;
public:
	UniquePtr():_ptr(NULL) { }
	UniquePtr(T *ptr):_ptr(ptr) {}
	~UniquePtr() { delete _ptr; }
	
	UniquePtr(UniquePtr &rhs) { _ptr = rhs.Release(); }
	UniquePtr(const UniquePtr &rhs) { _ptr = NULL; }

	
	
	T* Get() { return _ptr; }
	T* Release() { T* tmp = _ptr; _ptr = NULL;  return tmp;}
	
	T* operator->() { return _ptr; }
	T& operator*() { return *_ptr; }
	
	bool operator==(T* ptr) { return ptr == _ptr; }
	bool operator!=(T* ptr) { return ptr != _ptr; }
	
};

template <typename T>
class RefPtr
{
	int *_refs;
	T *_ptr;
public:
	RefPtr() : _ptr(NULL), _refs(NULL) {}
	RefPtr(T* ptr): _ptr(ptr), _refs(new int(1)) {}
	RefPtr(const RefPtr &ptr):_ptr(ptr._ptr), _refs(ptr._refs){ IncRef(); std::cout << "Copying with " << *_refs << std::endl; }
	~RefPtr() { DecRef(); if( *_refs == 0 ) { std::cout << "No Refs" << std::endl; delete _ptr; delete _refs; }  } 	
	
	T* Get() { return _ptr; }
	
	T* operator->() { return _ptr; }
	T& operator*() { return *_ptr; }
	
	bool operator==(T* ptr) { return ptr == _ptr; }
	bool operator!=(T* ptr) { return ptr != _ptr; }
	
	void IncRef() {
      if(_refs) { ++(*_refs); }
   }

   int  DecRef() {
      return --(*_refs);
   }

};
#endif