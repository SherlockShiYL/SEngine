#include "stdafx.h"
#include "CppUnitTest.h"

#include <Math\Inc\SMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTest
{		
	TEST_CLASS(Vector2Test)
	{
	public:
		
		TEST_METHOD(Vector2Constructor)
		{
			S::Math::Vector2 v{ 1.0f,2.0f };
			Assert::AreEqual(v.x, 1.0f);
			Assert::AreEqual(v.y, 2.0f);
		}
		TEST_METHOD(Vector2OperatorNegative)
		{
			S::Math::Vector2 v{ 1.0f,2.0f };
			v = -v;
			Assert::AreEqual(v.x, -1.0f);
			Assert::AreEqual(v.y, -2.0f);
		}
		TEST_METHOD(Vector2OperatorPlus)
		{
			S::Math::Vector2 v0{ 3.2f,4.0f };
			S::Math::Vector2 v1{ 1.4f,2.0f };
			S::Math::Vector2 sum = v0 + v1;
			Assert::AreEqual(sum.x, 4.6f);
			Assert::AreEqual(sum.y, 6.0f);
		}
		TEST_METHOD(Vector2OperatorMinus)
		{
			S::Math::Vector2 v0{ 3.2f,4.0f };
			S::Math::Vector2 v1{ 1.2f,2.0f };
			S::Math::Vector2 minus = v0 - v1;
			Assert::AreEqual(minus.x, 3.2f - 1.2f);
			Assert::AreEqual(minus.y, 4.0f - 2.0f);
		}
		TEST_METHOD(Vector2OperatorTimes)
		{
			S::Math::Vector2 v{ 3.2f,4.0f };
			S::Math::Vector2 times = v * 3.0f;
			Assert::AreEqual(times.x, 3.2f*3.0f);
			Assert::AreEqual(times.y, 4.0f*3.0f);
		}
		TEST_METHOD(Vector2OperatorDivide)
		{
			S::Math::Vector2 v{ 3.2f,4.0f };
			S::Math::Vector2 sum = v / 2.0f;
			Assert::AreEqual(sum.x, 1.6f);
			Assert::AreEqual(sum.y, 2.0f);
		}
		TEST_METHOD(Vector2OperatorPlusEqual)
		{
			S::Math::Vector2 v0{ 3.0f,4.0f };
			S::Math::Vector2 v1{ 3.2f,4.2f };
			v0 += v1;
			Assert::AreEqual(6.2f, v0.x);
			Assert::AreEqual(8.2f, v0.y);
		}
		TEST_METHOD(Vector2OperatorMinusEqual)
		{
			S::Math::Vector2 v0{ 3.0f,4.0f };
			S::Math::Vector2 v1{ 3.2f,4.2f };
			v0 -= v1;
			//Assert::AreEqual(v0.x, -0.2f);
			//Assert::AreEqual(v0.y, -0.2f);
			Assert::AreEqual(v0.x, 3.0f - 3.2f);
			Assert::AreEqual(v0.y, 4.0f - 4.2f);
		}
		TEST_METHOD(Vector2OperatorTimeEqual)
		{
			S::Math::Vector2 v{ 3.0f,4.0f };
			float s = 2.5f;
			v *= s;
			Assert::AreEqual(v.x, 3.0f*2.5f);
			Assert::AreEqual(v.y, 4.0f*2.5f);
			Assert::AreEqual(v.x, 7.5f);
			Assert::AreEqual(v.y, 10.0f);
		}
		TEST_METHOD(Vector2OperatorDivideEqual)
		{
			S::Math::Vector2 v{ 3.0f,4.0f };
			float s = 2.5f;
			v /= s;
			Assert::AreEqual(v.x, 3.0f/2.5f);
			Assert::AreEqual(v.y, 4.0f/2.5f);
			Assert::AreEqual(v.x, 1.2f);
			Assert::AreEqual(v.y, 1.6f);
		}
	};
}