#ifndef BANK_HPP
#define BANK_HPP

#include <map>
#include <ostream>
#include "Account.hpp"

// La banque possede ses comptes (par valeur dans une map) :
//  - ids uniques : generes par la banque + cle de la map
//  - aucune reference non-const vers un compte ne sort jamais de la banque
// Toutes les operations retournent true (succes) ou false (refus, rien n'est modifie).
class Bank
{
public:
	explicit Bank(int initialLiquidity = 0);
	~Bank();

	int createAccount();			  // retourne l'id du nouveau compte
	bool deleteAccount(int id);		  // refuse si dette en cours
	bool deposit(int id, int amount); // 5 % pour la banque
	bool withdraw(int id, int amount);
	bool giveLoan(int id, int amount); // limite par la liquidite, 5 % aussi
	bool repayLoan(int id, int amount);

	const Account *getAccount(int id) const; // NULL si inconnu
	const int &getLiquidity() const;

	void print(std::ostream &os) const;

private:
	// Une banque ne se copie pas : copier dupliquerait l'argent.
	Bank(const Bank &other);
	Bank &operator=(const Bank &other);

	static const int FEE_PERCENT = 5;
	static int feeOf(int amount);
	static bool addOverflows(int a, int b);

	int _liquidity;
	int _nextId;
	std::map<int, Account> _accounts;
};

std::ostream &operator<<(std::ostream &os, const Bank &bank);

#endif
