#include <iostream>
#include "Bank.hpp"

static void report(const char *label, bool ok)
{
	std::cout << "  " << label << " -> " << (ok ? "accepte" : "refuse") << std::endl;
}

int main()
{
	try
	{
		Bank bank(1000);
		int a = bank.createAccount();
		int b = bank.createAccount();

		std::cout << "== Cas normaux ==" << std::endl;
		report("deposit(a, 100)", bank.deposit(a, 100));
		report("lendMoney(a, 200)", bank.lendMoney(a, 200));
		report("returnLoanMoney(a, 50)", bank.returnLoanMoney(a, 50));
		report("deposit(b, 40)", bank.deposit(b, 40));
		report("takeMoney(b, 10)", bank.takeMoney(b, 10));
		std::cout << bank << std::endl;

		std::cout << "== Cas refuses (rien ne doit changer) ==" << std::endl;
		report("deposit(a, -10)", bank.deposit(a, -10));
		report("deposit(a, 0)", bank.deposit(a, 0));
		report("deposit(42, 10)", bank.deposit(42, 10));
		report("takeMoney(b, 1000)", bank.takeMoney(b, 1000));
		report("lendMoney(a, 999999)", bank.lendMoney(a, 999999));
		report("returnLoanMoney(a, 1000)", bank.returnLoanMoney(a, 1000));
		report("deleteAccount(a) (dette en cours)", bank.deleteAccount(a));
		report("deleteAccount(42)", bank.deleteAccount(42));
		std::cout << bank << std::endl;

		std::cout << "== Depassement d'int (banque separee) ==" << std::endl;
		{
			Bank rich(0);
			int c = rich.createAccount();

			report("deposit(c, INT_MAX)", rich.deposit(c, 429496730));
			report("deposit(c, INT_MAX) encore", rich.deposit(c, 2147483647));
			std::cout << "  solde inchange : " << rich.getAccount(c)->getValue() << std::endl;
		}

		std::cout << "== Suppression ==" << std::endl;
		report("deleteAccount(b)", bank.deleteAccount(b));
		const Account *acc = bank.getAccount(b);
		std::cout << "  getAccount(b) == NULL ? " << (acc == NULL) << std::endl;
		std::cout << bank << std::endl;
	}
	catch (...)
	{
		std::cerr << "Erreur inattendue" << std::endl;
		return (1);
	}
	return (0);
}
