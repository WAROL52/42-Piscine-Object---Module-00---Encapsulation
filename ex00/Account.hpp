#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <ostream>

// Compte client IMMUABLE : une fois construit, rien ne peut le modifier.
// Pour "modifier" un compte, la Bank le remplace par un nouvel Account.
// Pas de setter public : il permettrait de contourner la banque (frais de 5 %).
class Account
{
public:
	Account();
	Account(int id, int value, int debt);
	Account(const Account &other);
	Account &operator=(const Account &other);
	~Account();

	const int &getId() const;
	const int &getValue() const;
	const int &getDebt() const;

private:
	int _id;
	int _value;
	int _debt;
};

std::ostream &operator<<(std::ostream &os, const Account &account);

#endif
