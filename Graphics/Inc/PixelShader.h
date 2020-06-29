#ifndef INCLUDED_GRAPHICS_PIEXELSHADER_H
#define INCLUDED_GRAPHICS_PIEXELSHADER_H

namespace S {
namespace Graphics {

class PixelShader
{
public:
	PixelShader() = default;

	void Initialize(const wchar_t* fileName);
	void Initialize(const void* buffer, uint32_t size, const char* entryPoint, const char* pixelShaderModel);
	void Terminate();

	void Bind();

private:
	ID3D11PixelShader* mPixelShader{ nullptr };

};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_PIEXELSHADER_H