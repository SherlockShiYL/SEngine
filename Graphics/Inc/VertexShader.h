#ifndef INCLUDED_GRAPHICS_VERTEXSHADER_H
#define INCLUDED_GRAPHICS_VERTEXSHADER_H

namespace S {
namespace Graphics {

class VertexShader
{
public:
	VertexShader() = default;

	void Initialize(const wchar_t* fileName, int vertexFormat);
	void Initialize(const void* buffer, uint32_t size, uint32_t vertexFormat, const char* entryPoint, const char* shaderModel);
	void Terminate();

	void Bind();

private:
	ID3D11VertexShader* mVertexShader{ nullptr };
	ID3D11InputLayout* mInputLayout{ nullptr };

};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_VERTEXSHADER_H