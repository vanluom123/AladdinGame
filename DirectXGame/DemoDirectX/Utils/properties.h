#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

#include <iostream>


template <typename ClassT, typename TypeT, TypeT(ClassT:: * Getter)() const>
class _Get
{
public:
	explicit _Get(ClassT const* self)
		: _this(self) {
	}
	inline operator TypeT() const
	{
		return (_this->*Getter)();
	}
private:
	TypeT operator=(TypeT value) const;
private:
	ClassT const* _this;
};


template <typename ClassT, typename TypeT, void (ClassT:: * Setter)(TypeT)>
class _Set
{
public:
	explicit _Set(ClassT* self)
		: _this(self) {
	}
	inline TypeT operator=(TypeT value) const
	{
		(_this->*Setter)(value);
		return value;
	}
private:
	operator TypeT() const;
private:
	ClassT* _this;
};


template <typename ClassT, typename TypeT, TypeT(ClassT:: * Getter)() const, void (ClassT:: * Setter)(TypeT)>
class _GetSet
{
public:
	explicit _GetSet(ClassT* self)
		: _this(self) {
	}
	inline operator TypeT() const
	{
		return (_this->*Getter)();
	}
	inline TypeT operator=(TypeT value) const
	{
		(_this->*Setter)(value);
		return value;
	}
private:
	ClassT* _this;
};

#define GET(CLASS, TYPE, NAME, CODE)         \
  inline TYPE get##NAME() const CODE		 \
  _Get<CLASS, TYPE, &CLASS::get##NAME> NAME;

#define SET(CLASS, TYPE, NAME, CODE)         \
  inline void set##NAME(TYPE value) CODE     \
  _Set<CLASS, TYPE, &CLASS::set##NAME> NAME;

#define GETSET(CLASS, TYPE, NAME, GET, SET)  \
  inline TYPE get##NAME() const GET			 \
  inline void set##NAME(TYPE value) SET      \
  _GetSet<CLASS, TYPE, &CLASS::get##NAME, &CLASS::set##NAME> NAME;

#endif // __PROPERTIES_H__