#include <iostream>
#include "Bank.hpp"

int main()
{
	try
	{
		Bank bank(1000);
		int a = bank.createAccount();
		int b = bank.createAccount();

		std::cout << "== Cas normaux ==" << std::endl;
		bank.deposit(a, 100);
		bank.giveLoan(a, 200);
		bank.repayLoan(a, 50);
		bank.deposit(b, 40);
		bank.withdraw(b, 10);
		std::cout << "Compte " << a << " : " << bank[a].getValue()
				  << " (dette " << bank[a].getDebt() << ")" << std::endl;
		std::cout << bank << std::endl;

		std::cout << "== Cas d'erreur (un try par cas) ==" << std::endl;
		try
		{
			bank[42];
		}
		catch (const Bank::AccountNotFound &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		try
		{
			bank.deposit(a, -10);
		}
		catch (const Bank::InvalidAmount &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		try
		{
			bank.deposit(42, 10);
		}
		catch (const Bank::AccountNotFound &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		try
		{
			bank.withdraw(b, 1000);
		}
		catch (const Bank::InsufficientFunds &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		try
		{
			bank.giveLoan(a, 999999);
		}
		catch (const Bank::InsufficientFunds &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		try
		{
			bank.repayLoan(a, 1000);
		}
		catch (const Bank::InvalidAmount &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		try
		{
			bank.deleteAccount(a);
		}
		catch (const Bank::OutstandingDebt &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		try
		{
			bank.deleteAccount(42);
		}
		catch (const Bank::AccountNotFound &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}

		std::cout << "== Depassement d'int (banque separee) ==" << std::endl;
		{
			Bank rich(0);
			int c = rich.createAccount();

			rich.deposit(c, 2147483647); // accepte : le solde tient dans un int
			try
			{
				rich.deposit(c, 2147483647);
			} // le 2e ferait deborder
			catch (const Bank::AmountOverflow &e)
			{
				std::cout << "  OK : " << e.what() << std::endl;
			}
			std::cout << "  solde inchange : " << rich[c].getValue() << std::endl;
		}

		std::cout << "== Suppression ==" << std::endl;
		bank.deleteAccount(b);
		try
		{
			bank[b];
		}
		catch (const Bank::AccountNotFound &e)
		{
			std::cout << "  OK : " << e.what() << std::endl;
		}
		std::cout << bank << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Erreur : " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
