#ifndef INCLUDED_GRAPHICS_RENDERTARGET_H
#define INCLUDED_GRAPHICS_RENDERTARGET_H

namespace S {
namespace Graphics {

class Texture;

class RenderTarget
{
public:
	enum class Format
	{
		RGBA_U8,
		RGBA_F16,
		RGBA_U32,
		R_F16,
		R_S32
	};

	RenderTarget() = default;
	~RenderTarget();

	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;

	void Initialize(uint32_t width = 1280u, uint32_t height = 1280u, Format format = Format::RGBA_U8);
	void Terminate();

	void BeginRender();
	void EndRender();

	void BindPS(uint32_t slot = 0);
	void UnbindPS(uint32_t slot = 0);

	ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }

private:
	ID3D11ShaderResourceView* mShaderResourceView;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewport;
	D3D11_MAPPED_SUBRESOURCE mSubresource;
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_RENDERTARGET_H