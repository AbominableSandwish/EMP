#include <math/matrice.h>

namespace emp
{
	Matrice4::Matrice4(float x, float y, float z)
	{
		matrice[0][0] = 1;
		matrice[1][1] = 1;
		matrice[2][2] = 1;
		matrice[3][3] = 1;

		matrice[3][0] = x;
		matrice[3][1] = y;
		matrice[3][2] = z;
	}

}
