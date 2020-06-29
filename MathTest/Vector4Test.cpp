#include "stdafx.h"
#include "CppUnitTest.h"

#include <Math\Inc\SMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTest
{
	TEST_CLASS(Vector4Test)
	{
	public:
		TEST_METHOD(Vector4Constructor)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 2.0f);
			Assert::AreEqual(v0.z, 3.0f);
			Assert::AreEqual(v0.w, 4.0f);
			S::Math::Vector4 v1;
			Assert::AreEqual(v1.x, 0.0f);
			Assert::AreEqual(v1.y, 0.0f);
			Assert::AreEqual(v1.z, 0.0f);
			Assert::AreEqual(v1.w, 0.0f);
		}
		TEST_METHOD(Vector4OperatorNegative)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			v0 = -v0;
			Assert::AreEqual(v0.x, -1.0f);
			Assert::AreEqual(v0.y, -2.0f);
			Assert::AreEqual(v0.z, -3.0f);
			Assert::AreEqual(v0.w, -4.0f);
		}
		TEST_METHOD(Vector4OperatorPlus)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			S::Math::Vector4 v1{ 2.0f,5.0f,4.0f,2.0f };
			S::Math::Vector4 sum = v0 + v1;
			Assert::AreEqual(sum.x, 3.0f);
			Assert::AreEqual(sum.y, 7.0f);
			Assert::AreEqual(sum.z, 7.0f);
			Assert::AreEqual(sum.w, 6.0f);
		}
		TEST_METHOD(Vector4OperatorMinus)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			S::Math::Vector4 v1{ 2.0f,5.0f,4.0f,4.0f };
			S::Math::Vector4 minus = v0 - v1;
			Assert::AreEqual(minus.x, 1.0f - 2.0f);
			Assert::AreEqual(minus.y, 2.0f - 5.0f);
			Assert::AreEqual(minus.z, 3.0f - 4.0f);
			Assert::AreEqual(minus.w, 4.0f - 4.0f);
		}
		TEST_METHOD(Vector4OperatorTimes)
		{
			S::Math::Vector4 v{ 1.0f,2.0f,3.0f,4.0f };
			float s = 2.5f;
			S::Math::Vector4 times1 = v * s;
			Assert::AreEqual(times1.x, 2.5f);
			Assert::AreEqual(times1.y, 5.0f);
			Assert::AreEqual(times1.z, 7.5f);
			Assert::AreEqual(times1.w, 10.0f);
		}
		TEST_METHOD(Vector4OperatorDivide)
		{
			S::Math::Vector4 v{ 1.0f,2.0f,3.0f,4.0f };
			float s = 0.5f;
			S::Math::Vector4 divide = v / s;
			Assert::AreEqual(divide.x, 2.0f);
			Assert::AreEqual(divide.y, 4.0f);
			Assert::AreEqual(divide.z, 6.0f);
			Assert::AreEqual(divide.w, 8.0f);
		}
		TEST_METHOD(Vector4OperatorPlusEqual)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			S::Math::Vector4 v1{ 1.0f,2.0f,3.0f,4.0f };
			v0 += v1;
			Assert::AreEqual(v0.x, 2.0f);
			Assert::AreEqual(v0.y, 4.0f);
			Assert::AreEqual(v0.z, 6.0f);
			Assert::AreEqual(v0.w, 8.0f);
		}
		TEST_METHOD(Vector4OperatorMinusEqual)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			S::Math::Vector4 v1{ 1.0f,2.0f,3.0f,4.0f };
			v0 -= v1;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);
			Assert::AreEqual(v0.w, 0.0f);
		}
		TEST_METHOD(Vector4OperatorTimesEqual)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			float s = 2.5f;
			v0 *= s;
			Assert::AreEqual(v0.x, 2.5f);
			Assert::AreEqual(v0.y, 5.0f);
			Assert::AreEqual(v0.z, 7.5f);
			Assert::AreEqual(v0.w, 10.0f);
		}
		TEST_METHOD(Vector4OperatorDivideEqual)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			float s = 2.5f;
			v0 /= s;
			Assert::AreEqual(v0.x, 0.4f);
			Assert::AreEqual(v0.y, 0.8f);
			Assert::AreEqual(v0.z, 1.2f);
			Assert::AreEqual(v0.w, 1.6f);
		}
	};
}