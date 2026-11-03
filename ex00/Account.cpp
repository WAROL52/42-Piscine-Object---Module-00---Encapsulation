#include "Account.hpp"

Account::Account() : _id(-1), _value(0), _debt(0)
{
}

Account::Account(int id, int value, int debt) : _id(id), _value(value), _debt(debt)
{
}

Account::Account(const Account &other) : _id(other._id), _value(other._value), _debt(other._debt)
{
}

Account &Account::operator=(const Account &other)
{
	if (this != &other)
	{
		_id = other._id;
		_value = other._value;
		_debt = other._debt;
	}
	return (*this);
}

Account::~Account()
{
}

const int &Account::getId() const
{
	return (_id);
}

const int &Account::getValue() const
{
	return (_value);
}

const int &Account::getDebt() const
{
	return (_debt);
}

std::ostream &operator<<(std::ostream &os, const Account &account)
{
	os << "[" << account.getId() << "] - [" << account.getValue()
	   << "] - [debt " << account.getDebt() << "]";
	return (os);
}
