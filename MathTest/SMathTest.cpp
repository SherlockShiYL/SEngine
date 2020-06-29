#include "stdafx.h"
#include "CppUnitTest.h"

#include <Math\Inc\SMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTest
{
	TEST_CLASS(SMathTest)
	{
		TEST_METHOD(SMathCompare)
		{
			float a = -0.2f;
			float b = -0.2f;
			Assert::IsTrue(S::Math::Compare(a, b));
			Assert::IsTrue(S::Math::Compare(a, 3.0f - 3.2f));
		}

		TEST_METHOD(SMathCompareVector2)
		{
			S::Math::Vector2 v{ 3.4f - 0.2f,4.2f - 0.2f };
			S::Math::Vector2 v0{ 3.2f,4.0f };
			S::Math::Vector2 v1{ 3.2f,4.0f };
			S::Math::Vector2 vx{ 3.1f,4.0f };
			S::Math::Vector2 vy{ 3.2f,4.1f };
			Assert::IsTrue(Compare(v0, v1));
			Assert::IsFalse(Compare(v0, vx));
			Assert::IsFalse(Compare(v0, vy));
			Assert::IsFalse(Compare(vx, vy));
			Assert::IsTrue(Compare(v, v0));
		}
		TEST_METHOD(SMathCompareVector3)
		{
			S::Math::Vector3 v0{ 1.0f,2.0f,3.0f };
			S::Math::Vector3 v1{ 1.0f,2.0f,3.0f };
			S::Math::Vector3 vx{ 0.0f,2.0f,3.0f };
			S::Math::Vector3 vy{ 1.0f,3.0f,3.0f };
			S::Math::Vector3 vz{ 1.0f,2.0f,4.0f };
			S::Math::Vector3 v{ 1.7f - 0.7f,3.8f - 1.8f,3.33f - 0.33f };
			Assert::IsTrue(Compare(v0, v1));
			Assert::IsFalse(Compare(v0, vx));
			Assert::IsFalse(Compare(v0, vy));
			Assert::IsFalse(Compare(v0, vz));
			Assert::IsFalse(Compare(vx, vy));
			Assert::IsFalse(Compare(vy, vz));
			Assert::IsFalse(Compare(vz, vx));
			Assert::IsTrue(Compare(v, v0));
		}
		TEST_METHOD(SMathCompareVector4)
		{
			S::Math::Vector4 v0{ 1.0f,2.0f,3.0f,4.0f };
			S::Math::Vector4 v1{ 1.0f,2.0f,3.0f,4.0f };
			S::Math::Vector4 vx{ 0.0f,2.0f,3.0f,4.0f };
			S::Math::Vector4 vy{ 1.0f,3.0f,3.0f,4.0f };
			S::Math::Vector4 vz{ 1.0f,2.0f,2.0f,4.0f };
			S::Math::Vector4 vw{ 1.0f,2.0f,3.0f,1.0f };
			S::Math::Vector4 v{ 1.2f - 0.2f,2.3f - 0.3f,4.7f - 1.7f,5.9f - 1.9f };
			Assert::IsTrue(Compare(v0, v1));
			Assert::IsFalse(Compare(v0, vx));
			Assert::IsFalse(Compare(v0, vy));
			Assert::IsFalse(Compare(v0, vz));
			Assert::IsFalse(Compare(v0, vw));
			Assert::IsFalse(Compare(vy, vx));
			Assert::IsFalse(Compare(vy, vz));
			Assert::IsFalse(Compare(vy, vw));
			Assert::IsFalse(Compare(vx, vw));
			Assert::IsTrue(Compare(v, v0));
		}
	};
}