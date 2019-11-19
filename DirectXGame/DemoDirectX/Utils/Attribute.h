#pragma once

//template<class T>
//class Array
//{
//private:
//	T* _array;
//	int _size;
//
//public:
//	Array(int sz) : _size(sz)
//	{
//		_array = new T[_size];
//		memset(array, 0, _size * sizeof(T));
//	}
//
//	Array(const Array& a)
//	{
//		_size = a._size;
//		_array = new T[_size];
//		memcpy_s(_array, a._array, sizeof(T));
//	}
//
//	T& operator[](int i)
//	{
//		return *(_array + i);
//	}
//
//	int Length() { return size; }
//
//	void print() {
//		for (int i = 0; i < size; i++)
//			cout << *(_array + i) << " ";
//
//		cout << endl;
//	}
//
//	template<class T>
//	friend Array<T>* combine(Array<T>& a1, Array<T>& a2);
//};
//
//template<class T>
//Array<T>* combine(Array<T>& a1, Array<T>& a2)
//{
//	Array<T>* a = new Array<T>(a1._size + a2._size);
//
//	for (int i = 0; i < a1._size; i++)
//		(*a)[i] = *(a1._array + i); // *(a1._array + i) ~ a1._array[i]
//
//	for (int i = 0; i < a2._size; i++)
//		(*a)[i + a1._size] = *(a2._array + i);
//
//	return a;
//}

template<typename T>
class Attribute
{
protected:
	T _value;

public:
	operator T()
	{
		return _value;
	}

	T& operator=(const T& a) {
		_value = a;
		return _value;
	}

	T* operator&()
	{
		return &_value;
	}
};
