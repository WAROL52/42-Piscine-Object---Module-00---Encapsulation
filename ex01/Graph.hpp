#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <ostream>
#include "Vector2.hpp"

// Graph a un invariant : tous ses points sont DANS la grille.
// Donc attributs prives, taille figee a la construction (pas de setSize),
// et un seul moyen d'ajouter un point : addPoint, qui valide.
class Graph
{
public:
	Graph();							 // taille (0, 0) : une seule case
	explicit Graph(const Vector2 &size); // tailles bornees a [0, MAX_SIZE]
	Graph(const Graph &other);
	Graph &operator=(const Graph &other);
	~Graph();

	bool addPoint(const Vector2 &point); // false si hors de la grille

	const Vector2 &getSize() const;
	const std::vector<Vector2> &getPoints() const;

	void print(std::ostream &os) const;

private:
	static const int MAX_SIZE = 1000;

	static float sanitizeSize(float v);
	static int toCell(float v); // v doit etre dans [0, MAX_SIZE + 1]
	static int digits(int n);

	int maxX() const;
	int maxY() const;
	bool locate(const Vector2 &p, int &cx, int &cy) const;

	Vector2 _size;
	std::vector<Vector2> _points;
};

std::ostream &operator<<(std::ostream &os, const Graph &graph);

#endif
