#include <iomanip>
#include <string>
#include "Graph.hpp"

Graph::Graph() : _size(0.0f, 0.0f), _points()
{
}

Graph::Graph(const Vector2 &size) : _size(sanitizeSize(size.x), sanitizeSize(size.y)),
									_points()
{
}

Graph::Graph(const Graph &other) : _size(other._size), _points(other._points)
{
}

Graph &Graph::operator=(const Graph &other)
{
	if (this != &other)
	{
		_size = other._size;
		_points = other._points;
	}
	return (*this);
}

Graph::~Graph()
{
}

// Negatif ou NaN -> 0 ; trop grand -> MAX_SIZE (evite une grille geante).
float Graph::sanitizeSize(float v)
{
	if (!(v >= 0.0f))
		return (0.0f);
	if (v > static_cast<float>(MAX_SIZE))
		return (static_cast<float>(MAX_SIZE));
	return (v);
}

// Arrondi au plus proche. Valable seulement pour v >= 0
// (le cast tronque vers 0 : -0.3f donnerait 0).
int Graph::toCell(float v)
{
	return (static_cast<int>(v + 0.5f));
}

int Graph::digits(int n)
{
	int d = 1;

	while (n >= 10)
	{
		n /= 10;
		++d;
	}
	return (d);
}

int Graph::maxX() const
{
	return (toCell(_size.x));
}

int Graph::maxY() const
{
	return (toCell(_size.y));
}

// Convertit un point en case (cx, cy). false si hors de la grille.
// Le test "!(v >= 0)" rejette aussi NaN ; la borne evite un cast d'un
// float gigantesque vers int (comportement indefini).
bool Graph::locate(const Vector2 &p, int &cx, int &cy) const
{
	const float limit = static_cast<float>(MAX_SIZE) + 1.0f;

	if (!(p.x >= 0.0f) || !(p.y >= 0.0f) || p.x > limit || p.y > limit)
		return (false);
	cx = toCell(p.x);
	cy = toCell(p.y);
	return (cx <= maxX() && cy <= maxY());
}

bool Graph::addPoint(const Vector2 &point)
{
	int cx;
	int cy;

	if (!locate(point, cx, cy))
		return (false);
	_points.push_back(point);
	return (true);
}

const Vector2 &Graph::getSize() const
{
	return (_size);
}

const std::vector<Vector2> &Graph::getPoints() const
{
	return (_points);
}

void Graph::print(std::ostream &os) const
{
	const int mx = maxX();
	const int my = maxY();
	const int w = digits(mx > my ? mx : my);

	std::vector<std::string> grid(my + 1, std::string(mx + 1, '.'));

	for (std::vector<Vector2>::const_iterator it = _points.begin();
		 it != _points.end(); ++it)
		grid[toCell(it->y)][toCell(it->x)] = 'X';

	for (int y = my; y >= 0; --y)
	{
		os << std::setw(w) << y;
		for (int x = 0; x <= mx; ++x)
			os << " " << std::setw(w) << grid[y][x];
		os << std::endl;
	}
	os << std::string(w, ' ');
	for (int x = 0; x <= mx; ++x)
		os << " " << std::setw(w) << x;
	os << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Graph &graph)
{
	graph.print(os);
	return (os);
}
