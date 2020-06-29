#include "stdafx.h"
#include "CppUnitTest.h"

#include <Math\Inc\SMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		TEST_METHOD(Vector3Constructor)
		{
			S::Math::Vector3 v;
			Assert::AreEqual(v.x, 0.0f);
			Assert::AreEqual(v.y, 0.0f);
			Assert::AreEqual(v.z, 0.0f);
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 2.0f);
			Assert::AreEqual(v0.z, 3.0f);
			S::Math::Vector3 v1;
			Assert::AreEqual(v1.x, 0.0f);
			Assert::AreEqual(v1.y, 0.0f);
			Assert::AreEqual(v1.z, 0.0f);
		}
		TEST_METHOD(Vector3OperatorNegative)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			v0 = -v0;
			Assert::AreEqual(v0.x, -1.0f);
			Assert::AreEqual(v0.y, -2.0f);
			Assert::AreEqual(v0.z, -3.0f);
		}
		TEST_METHOD(Vector3OperatorPlus)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			S::Math::Vector3 v1{ 2.0f,5.0f,4.0f };
			S::Math::Vector3 sum = v0 + v1;
			Assert::AreEqual(sum.x, 3.0f);
			Assert::AreEqual(sum.y, 7.0f);
			Assert::AreEqual(sum.z, 7.0f);
		}
		TEST_METHOD(Vector3OperatorMinus)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			S::Math::Vector3 v1{ 2.0f,5.0f,4.0f };
			S::Math::Vector3 minus = v0 - v1;
			Assert::AreEqual(minus.x, 1.0f - 2.0f);
			Assert::AreEqual(minus.y, 2.0f - 5.0f);
			Assert::AreEqual(minus.z, 3.0f - 4.0f);
		}
		TEST_METHOD(Vector3OperatorTimes)
		{
			S::Math::Vector3 v{ 1.0f,2.0f,3.0f };
			float s = 2.5f;
			S::Math::Vector3 times1 = v * s;
			Assert::AreEqual(times1.x, 2.5f);
			Assert::AreEqual(times1.y, 5.0f);
			Assert::AreEqual(times1.z, 7.5f);
		}
		TEST_METHOD(Vector3OperatorDivide)
		{
			S::Math::Vector3 v{ 1.0f,2.0f,3.0f };
			float s = 0.5f;
			S::Math::Vector3 divide = v / s;
			Assert::AreEqual(divide.x, 2.0f);
			Assert::AreEqual(divide.y, 4.0f);
			Assert::AreEqual(divide.z, 6.0f);
		}
		TEST_METHOD(Vector3OperatorPlusEqual)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			S::Math::Vector3 v1{ 1.0f,2.0f,3.0f };
			v0 += v1;
			Assert::AreEqual(v0.x, 2.0f);
			Assert::AreEqual(v0.y, 4.0f);
			Assert::AreEqual(v0.z, 6.0f);
		}
		TEST_METHOD(Vector3OperatorMinusEqual)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			S::Math::Vector3 v1{ 1.0f,2.0f,3.0f };
			v0 -= v1;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);
		}
		TEST_METHOD(Vector3OperatorTimesEqual)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			float s = 2.5f;
			v0 *= s;
			Assert::AreEqual(v0.x, 2.5f);
			Assert::AreEqual(v0.y, 5.0f);
			Assert::AreEqual(v0.z, 7.5f);
		}
		TEST_METHOD(Vector3OperatorDivideEqual)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			float s = 2.5f;
			v0 /= s;
			Assert::AreEqual(v0.x, 0.4f);
			Assert::AreEqual(v0.y, 0.8f);
			Assert::AreEqual(v0.z, 1.2f);
		}
	};
}