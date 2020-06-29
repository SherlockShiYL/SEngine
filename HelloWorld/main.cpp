#include <Math/Inc/SMath.h>
#include <Geometry/Inc/Geometry.h>

int main(int argc, char* atgv[])
{
	S::Math::Vector2 vvv;
	S::Math::Vector2 vvvv = vvv / 0;
	S::Math::Vector3 v3;
	printf("%f,%f,%f\n", v3.x, v3.y, v3.z);
	float x = -0.2f;
	float y = 3.0f - 3.2f;
	float z = -0.2f;
	if (S::Math::Compare(x, z))
	{
		printf("TRUE");
	}
	else
	{
		printf("FALSE");
	}

	S::Math::Vector2 u{ 1.0f,10.0f };
	S::Math::Vector2 v{ 1.0f,20.0f };

	S::Math::Vector2 w = u + v;
	printf("w = %f, %f\n", w.x, w.y);

	x = 2.0f;
	x = S::Math::Abs(x);
	printf("%f\n", x);
	x = -x;
	printf("%f\n", x);

	x = S::Math::Abs(x);
	printf("%f\n", x);

	x = S::Math::kPi;
	printf("%f\n", x);

	S::Math::Matrix3 m;
	m = m * S::Math::Matrix3::Scaling(v.x,v.y);
	m += m;
	v = -v;

	S::Math::Matrix4 m4;

	return 0;
}