#ifndef INCLUDED_GRAPHICS_MATERIAL_H
#define INCLUDED_GRAPHICS_MATERIAL_H

namespace S {
namespace Graphics {

struct Material
{
	Math::Vector4 mAmbient;
	Math::Vector4 mDiffuse;
	Math::Vector4 mSpecular;
	float mPower;
	Math::Vector3 mPadding;

	Material():
		mAmbient{ 0.0f,0.0f,0.0f,0.0f },
		mDiffuse{ 0.0f,0.0f,0.0f,0.0f },
		mSpecular{ 0.0f,0.0f,0.0f,0.0f },
		mPower{ 1.0f }{}

	Material(Math::Vector4 ambient, Math::Vector4 diffuse, Math::Vector4 specular,float power):
		mAmbient{ ambient }, mDiffuse{ diffuse }, mSpecular{ specular }, mPower{ power }{}
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_MATERIAL_H