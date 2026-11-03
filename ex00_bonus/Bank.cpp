#include <climits>
#include "Bank.hpp"

/* ---------------- Account (prive, immuable) ---------------- */

Bank::Account::Account(int id, int value, int debt) : _id(id), _value(value), _debt(debt)
{
}

const int &Bank::Account::getId() const
{
	return (_id);
}

const int &Bank::Account::getValue() const
{
	return (_value);
}

const int &Bank::Account::getDebt() const
{
	return (_debt);
}

/* ---------------- Exceptions ---------------- */

const char *Bank::AccountNotFound::what() const throw()
{
	return ("account not found");
}

const char *Bank::InvalidAmount::what() const throw()
{
	return ("invalid amount");
}

const char *Bank::InsufficientFunds::what() const throw()
{
	return ("insufficient funds");
}

const char *Bank::OutstandingDebt::what() const throw()
{
	return ("account has an outstanding debt");
}

const char *Bank::AmountOverflow::what() const throw()
{
	return ("amount too large");
}

/* ---------------- Bank ---------------- */

Bank::Bank(int initialLiquidity) : _liquidity(initialLiquidity),
								   _nextId(0),
								   _accounts()
{
	if (initialLiquidity < 0)
		throw InvalidAmount();
}

Bank::~Bank()
{
}

// 5 % arrondi a l'inferieur, sans risque de depassement d'int.
int Bank::feeOf(int amount)
{
	return (amount / 100 * FEE_PERCENT + (amount % 100) * FEE_PERCENT / 100);
}

bool Bank::addOverflows(int a, int b) // a et b sont toujours >= 0
{
	return (b > INT_MAX - a);
}

int Bank::createAccount()
{
	int id = _nextId;

	_accounts.insert(std::make_pair(id, Account(id, 0, 0)));
	++_nextId;
	return (id);
}

void Bank::deleteAccount(int id)
{
	std::map<int, Account>::iterator it = _accounts.find(id);

	if (it == _accounts.end())
		throw AccountNotFound();
	if (it->second.getDebt() > 0)
		throw OutstandingDebt();
	_accounts.erase(it); // le solde restant est verse au client
}

void Bank::deposit(int id, int amount)
{
	if (amount <= 0)
		throw InvalidAmount();
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end())
		throw AccountNotFound();

	int fee = feeOf(amount);
	int net = amount - fee;

	if (addOverflows(it->second.getValue(), net) || addOverflows(_liquidity, fee))
		throw AmountOverflow();
	_liquidity += fee;
	it->second = Account(id, it->second.getValue() + net, it->second.getDebt());
}

void Bank::withdraw(int id, int amount)
{
	if (amount <= 0)
		throw InvalidAmount();
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end())
		throw AccountNotFound();
	if (it->second.getValue() < amount)
		throw InsufficientFunds();
	it->second = Account(id, it->second.getValue() - amount, it->second.getDebt());
}

// Le pret est une entree d'argent sur le compte : 5 % pour la banque.
// Le client est credite de (amount - fee) mais doit rembourser amount.
void Bank::giveLoan(int id, int amount)
{
	if (amount <= 0)
		throw InvalidAmount();
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end())
		throw AccountNotFound();
	if (amount > _liquidity)
		throw InsufficientFunds();

	int fee = feeOf(amount);
	int net = amount - fee;

	if (addOverflows(it->second.getValue(), net) || addOverflows(it->second.getDebt(), amount))
		throw AmountOverflow();
	_liquidity -= net; // sort : amount, rentre : fee
	it->second = Account(id, it->second.getValue() + net,
						 it->second.getDebt() + amount);
}

void Bank::repayLoan(int id, int amount)
{
	if (amount <= 0)
		throw InvalidAmount();
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end())
		throw AccountNotFound();
	if (amount > it->second.getDebt())
		throw InvalidAmount();
	if (amount > it->second.getValue())
		throw InsufficientFunds();
	if (addOverflows(_liquidity, amount))
		throw AmountOverflow();
	_liquidity += amount;
	it->second = Account(id, it->second.getValue() - amount,
						 it->second.getDebt() - amount);
}

const Bank::Account &Bank::operator[](int id) const
{
	std::map<int, Account>::const_iterator it = _accounts.find(id);

	if (it == _accounts.end())
		throw AccountNotFound();
	return (it->second);
}

const int &Bank::getLiquidity() const
{
	return (_liquidity);
}

// Seule boucle du fichier : parcours d'affichage, pas une recherche par id.
void Bank::print(std::ostream &os) const
{
	os << "Bank informations :" << std::endl;
	os << "Liquidity : " << _liquidity << std::endl;
	for (std::map<int, Account>::const_iterator it = _accounts.begin();
		 it != _accounts.end(); ++it)
		os << "[" << it->second.getId() << "] - [" << it->second.getValue()
		   << "] - [debt " << it->second.getDebt() << "]" << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Bank &bank)
{
	bank.print(os);
	return (os);
}
