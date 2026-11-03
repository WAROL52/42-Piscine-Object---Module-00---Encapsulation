#include <climits>
#include "Bank.hpp"

Bank::Bank(const Bank &other)
{
	*this = other;
}

Bank &Bank::operator=(const Bank &other)
{
	if (this != &other)
	{
		_liquidity = other._liquidity;
		_nextId = other._nextId;
		_accounts = other._accounts;
	}
	return *this;
}

Bank::Bank(int initialLiquidity) : _liquidity(initialLiquidity < 0 ? 0 : initialLiquidity),
								   _nextId(0),
								   _accounts()
{
}

Bank::~Bank()
{
}

// 5 % arrondi a l'inferieur, calcule sans risque de depassement d'int.
// amount = 100*q + r  =>  amount*5/100 = 5*q + (5*r)/100
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

bool Bank::deleteAccount(int id)
{
	std::map<int, Account>::iterator it = _accounts.find(id);

	if (it == _accounts.end() || it->second.getDebt() > 0)
		return (false);
	_accounts.erase(it); // le solde restant est verse au client
	return (true);
}

bool Bank::deposit(int id, int amount)
{
	if (amount <= 0)
		return (false);
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end())
		return (false);

	int fee = feeOf(amount);
	int net = amount - fee;

	if (addOverflows(it->second.getValue(), net) || addOverflows(_liquidity, fee))
		return (false);
	_liquidity += fee;
	it->second = Account(id, it->second.getValue() + net, it->second.getDebt());
	return (true);
}

bool Bank::withdraw(int id, int amount)
{
	if (amount <= 0)
		return (false);
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end() || it->second.getValue() < amount)
		return (false);
	it->second = Account(id, it->second.getValue() - amount, it->second.getDebt());
	return (true);
}

// Le pret est une entree d'argent sur le compte : 5 % pour la banque.
// Le client est credite de (amount - fee) mais doit rembourser amount.
bool Bank::giveLoan(int id, int amount)
{
	if (amount <= 0 || amount > _liquidity)
		return (false);
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end())
		return (false);

	int fee = feeOf(amount);
	int net = amount - fee;

	if (addOverflows(it->second.getValue(), net) || addOverflows(it->second.getDebt(), amount))
		return (false);
	_liquidity -= net; // sort : amount, rentre : fee
	it->second = Account(id, it->second.getValue() + net,
						 it->second.getDebt() + amount);
	return (true);
}

bool Bank::repayLoan(int id, int amount)
{
	if (amount <= 0)
		return (false);
	std::map<int, Account>::iterator it = _accounts.find(id);
	if (it == _accounts.end() || amount > it->second.getDebt() || amount > it->second.getValue() || addOverflows(_liquidity, amount))
		return (false);
	_liquidity += amount;
	it->second = Account(id, it->second.getValue() - amount,
						 it->second.getDebt() - amount);
	return (true);
}

const Account *Bank::getAccount(int id) const
{
	std::map<int, Account>::const_iterator it = _accounts.find(id);

	if (it == _accounts.end())
		return (NULL);
	return (&it->second);
}

const int &Bank::getLiquidity() const
{
	return (_liquidity);
}

void Bank::print(std::ostream &os) const
{
	os << "Bank informations :" << std::endl;
	os << "Liquidity : " << _liquidity << std::endl;
	for (std::map<int, Account>::const_iterator it = _accounts.begin();
		 it != _accounts.end(); ++it)
		os << it->second << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Bank &bank)
{
	bank.print(os);
	return (os);
}
