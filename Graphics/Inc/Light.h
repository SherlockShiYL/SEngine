#ifndef INCLUDED_GRAPHICS_LIGHT_H
#define INCLUDED_GRAPHICS_LIGHT_H

namespace S{
namespace Graphics {

struct DirectionalLight
{
	Math::Vector3 mDirection;
	float mPadding;
	Math::Vector4 mAmbient;
	Math::Vector4 mDiffuse;
	Math::Vector4 mSpecular;

	DirectionalLight() : 
		mDirection{ 1.0f,0.0f,0.0f },
		mPadding{ 0.0f },
		mAmbient{ 0.0f,0.0f,0.0f,0.0f },
		mDiffuse{ 0.0f,0.0f,0.0f,0.0f },
		mSpecular{ 0.0f,0.0f,0.0f,0.0f }{}

	DirectionalLight(Math::Vector3 direction, Math::Vector4 ambient, Math::Vector4 diffuse, Math::Vector4 specular) :
		mDirection{ direction }, mPadding{ 0.0f }, mAmbient{ ambient }, mDiffuse{ diffuse }, mSpecular{ specular }{}
};

struct PointLight
{
	Math::Vector3 mPosition;
	float mPadding0;
	float mConstant;
	float mLinear;
	float mQuadratic;
	float mPadding1;
	Math::Vector4 mAmbient;
	Math::Vector4 mDiffuse;
	Math::Vector4 mSpecular;

	PointLight() :
		mPosition{ 0.0f,0.0f,0.0f }, mPadding0{ 0.0f },
		mConstant{ 0.0f }, mLinear{ 0.0f }, mQuadratic{ 0.0f }, mPadding1{ 0.0f },
		mAmbient{ 0.0f,0.0f,0.0f,0.0f },
		mDiffuse{ 0.0f,0.0f,0.0f,0.0f },
		mSpecular{ 0.0f,0.0f,0.0f,0.0f }{}

	PointLight(Math::Vector3 position, float constant, float linear, float quadratic,
		Math::Vector4 ambient, Math::Vector4 diffuse, Math::Vector4 specular) :
		mPosition{ position }, mPadding0{ 0.0f },
		mConstant{ constant }, mLinear{ linear }, mQuadratic{ quadratic }, mPadding1{ 0.0f }, 
		mAmbient{ ambient }, mDiffuse{ diffuse }, mSpecular{ specular }{}
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_LIGHT_H