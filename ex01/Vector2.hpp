#ifndef VECTOR2_HPP
# define VECTOR2_HPP

// Valeur simple sans invariant : n'importe quel couple de floats est valide.
// Attributs publics (comme std::pair) : des getters/setters n'apporteraient rien.
struct Vector2
{
	float	x;
	float	y;

	Vector2();
	Vector2(float px, float py);
	Vector2(const Vector2& other);
	Vector2&	operator=(const Vector2& other);
	~Vector2();
};

#endif
