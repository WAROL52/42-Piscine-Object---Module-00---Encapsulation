#include <iostream>
#include <limits>
#include "Graph.hpp"

static void report(const char *label, bool ok)
{
	std::cout << "  " << label << " -> " << (ok ? "accepte" : "refuse") << std::endl;
}

int main()
{
	try
	{
		std::cout << "== Exemple du sujet (taille 5) ==" << std::endl;
		Graph g(Vector2(5, 5));

		report("addPoint(0/0)", g.addPoint(Vector2(0, 0)));
		report("addPoint(2/2)", g.addPoint(Vector2(2, 2)));
		report("addPoint(4/2)", g.addPoint(Vector2(4, 2)));
		report("addPoint(2/4)", g.addPoint(Vector2(2, 4)));
		std::cout << g << std::endl;

		std::cout << "== Points refuses (le graphe ne change pas) ==" << std::endl;
		report("addPoint(6/1)", g.addPoint(Vector2(6, 1)));
		report("addPoint(-0.3/1)", g.addPoint(Vector2(-0.3f, 1)));
		report("addPoint(1/NaN)", g.addPoint(Vector2(1,
													 std::numeric_limits<float>::quiet_NaN())));
		report("addPoint(1e30/1)", g.addPoint(Vector2(1e30f, 1)));
		report("addPoint(5.6/1) (arrondi a 6)", g.addPoint(Vector2(5.6f, 1)));
		std::cout << "  points stockes : " << g.getPoints().size() << std::endl;

		std::cout << "== Arrondi au plus proche ==" << std::endl;
		Graph r(Vector2(3, 3));
		report("addPoint(1.4/1.6) -> case 1/2", r.addPoint(Vector2(1.4f, 1.6f)));
		report("addPoint(2.9/0.2) -> case 3/0", r.addPoint(Vector2(2.9f, 0.2f)));
		std::cout << r << std::endl;

		std::cout << "== Taille 12 (alignement sur 2 chiffres) ==" << std::endl;
		Graph big(Vector2(12, 12));
		big.addPoint(Vector2(0, 0));
		big.addPoint(Vector2(11, 3));
		big.addPoint(Vector2(12, 12));
		std::cout << big << std::endl;

		std::cout << "== Graph par defaut / taille negative ==" << std::endl;
		Graph def;
		def.addPoint(Vector2(0, 0));
		std::cout << def << std::endl;
		Graph neg(Vector2(-4, -2));
		std::cout << neg << std::endl;
	}
	catch (...) // ex: std::bad_alloc, le programme ne doit jamais crasher
	{
		std::cerr << "Erreur inattendue" << std::endl;
		return (1);
	}
	return (0);
}
