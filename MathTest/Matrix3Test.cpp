#include "stdafx.h"
#include "CppUnitTest.h"

#include <Math\Inc\SMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTest
{
	TEST_CLASS(Matrix3Test)
	{
	public:
		TEST_METHOD(Matrix3Constructor)
		{
			S::Math::Matrix3 m;
			Assert::AreEqual(m._11, 1.0f);
			Assert::AreEqual(m._12, 0.0f);
			Assert::AreEqual(m._13, 0.0f);
			Assert::AreEqual(m._21, 0.0f);
			Assert::AreEqual(m._22, 1.0f);
			Assert::AreEqual(m._23, 0.0f);
			Assert::AreEqual(m._31, 0.0f);
			Assert::AreEqual(m._32, 0.0f);
			Assert::AreEqual(m._33, 1.0f);

			Assert::AreEqual(m._11, m._1[0]);
			Assert::AreEqual(m._12, m._1[1]);
			Assert::AreEqual(m._13, m._1[2]);
			Assert::AreEqual(m._21, m._2[0]);
			Assert::AreEqual(m._22, m._2[1]);
			Assert::AreEqual(m._23, m._2[2]);
			Assert::AreEqual(m._31, m._3[0]);
			Assert::AreEqual(m._32, m._3[1]);
			Assert::AreEqual(m._33, m._3[2]);

			S::Math::Matrix3 m0{ 2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
			Assert::AreEqual(m0._11, 2.0f);
			Assert::AreEqual(m0._12, 2.0f);
			Assert::AreEqual(m0._13, 2.0f);
			Assert::AreEqual(m0._21, 2.0f);
			Assert::AreEqual(m0._22, 2.0f);
			Assert::AreEqual(m0._23, 2.0f);
			Assert::AreEqual(m0._31, 2.0f);
			Assert::AreEqual(m0._32, 2.0f);
			Assert::AreEqual(m0._33, 2.0f);
		}
		TEST_METHOD(Matrix3OperatorNegative)
		{
			S::Math::Matrix3 m{ 2.0f,-2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
			m = -m;
			Assert::AreEqual(m._11, -2.0f);
			Assert::AreEqual(m._12, 2.0f);
			Assert::AreEqual(m._13, -2.0f);
			Assert::AreEqual(m._21, -2.0f);
			Assert::AreEqual(m._22, -2.0f);
			Assert::AreEqual(m._23, -2.0f);
			Assert::AreEqual(m._31, -2.0f);
			Assert::AreEqual(m._32, -2.0f);
			Assert::AreEqual(m._33, -2.0f);
		}
		TEST_METHOD(Matrix3OperatorPlus)
		{
			S::Math::Matrix3 m0{ 2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
			S::Math::Matrix3 m1{ 2.0f,-2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
			S::Math::Matrix3 m = m0 + m1;
			Assert::AreEqual(m._11, 4.0f);
			Assert::AreEqual(m._12, 0.0f);
			Assert::AreEqual(m._13, 4.0f);
			Assert::AreEqual(m._21, 4.0f);
			Assert::AreEqual(m._22, 4.0f);
			Assert::AreEqual(m._23, 4.0f);
			Assert::AreEqual(m._31, 4.0f);
			Assert::AreEqual(m._32, 4.0f);
			Assert::AreEqual(m._33, 4.0f);
		}
		TEST_METHOD(Matrix3OperatorMinus)
		{
			S::Math::Matrix3 m0{ 2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
			S::Math::Matrix3 m1{ 2.0f,-2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
			S::Math::Matrix3 m = m0 - m1;
			Assert::AreEqual(m._11, 0.0f);
			Assert::AreEqual(m._12, 4.0f);
			Assert::AreEqual(m._13, 0.0f);
			Assert::AreEqual(m._21, 0.0f);
			Assert::AreEqual(m._22, 0.0f);
			Assert::AreEqual(m._23, 0.0f);
			Assert::AreEqual(m._31, 0.0f);
			Assert::AreEqual(m._32, 0.0f);
			Assert::AreEqual(m._33, 0.0f);
		}
		TEST_METHOD(Matrix3OperatorTimesMatrix3)
		{

		}
		TEST_METHOD(Matrix3OperatorTimes)
		{
			S::Math::Matrix3 m{ 2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
			float s = 2.5f;
			m = m * s;
			Assert::AreEqual(m._11, 5.0f);
			Assert::AreEqual(m._12, 5.0f);
			Assert::AreEqual(m._13, 5.0f);
			Assert::AreEqual(m._21, 5.0f);
			Assert::AreEqual(m._22, 5.0f);
			Assert::AreEqual(m._23, 5.0f);
			Assert::AreEqual(m._31, 5.0f);
			Assert::AreEqual(m._32, 5.0f);
			Assert::AreEqual(m._33, 5.0f);
		}
	};
}