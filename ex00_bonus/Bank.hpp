#ifndef BANK_HPP
#define BANK_HPP

#include <map>
#include <ostream>
#include <exception>

class Bank
{
private:
	// Type PRIVE et IMMUABLE : personne hors de Bank ne peut le nommer,
	// donc le creer ou le stocker. Aucune methode autre que des getters const.
	// Declare avant les signatures publiques qui l'utilisent.
	struct Account
	{
	public:
		Account(int id, int value, int debt);

		const int &getId() const;
		const int &getValue() const;
		const int &getDebt() const;

	private:
		int _id;
		int _value;
		int _debt;
	};

public:
	// Erreurs : publiques pour que le main puisse les attraper.
	struct AccountNotFound : public std::exception
	{
		virtual const char *what() const throw();
	};
	struct InvalidAmount : public std::exception
	{
		virtual const char *what() const throw();
	};
	struct InsufficientFunds : public std::exception
	{
		virtual const char *what() const throw();
	};
	struct OutstandingDebt : public std::exception
	{
		virtual const char *what() const throw();
	};
	struct AmountOverflow : public std::exception
	{
		virtual const char *what() const throw();
	};

	explicit Bank(int initialLiquidity = 0); // throw InvalidAmount si < 0
	~Bank();

	int createAccount();
	void deleteAccount(int id);
	void deposit(int id, int amount);
	void withdraw(int id, int amount);
	void giveLoan(int id, int amount);
	void repayLoan(int id, int amount);

	// Acces par id, sans boucle (std::map::find). throw AccountNotFound.
	const Account &operator[](int id) const;
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
