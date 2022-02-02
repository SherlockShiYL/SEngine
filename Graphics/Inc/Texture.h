#ifndef INCLUDED_GRAPHICS_TEXTURE_H
#define INCLUDED_GRAPHICS_TEXTURE_H

namespace S {
namespace Graphics {

class Texture
{
public:
	Texture();
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& rhs) noexcept;
	Texture& operator=(Texture&& rhs) noexcept;

	void Swap(Texture& rhs) noexcept;

	bool Initialize(const char* fileName);
	void Initialize(const void* data, uint32_t width, uint32_t height);
	void Terminate();

	void Set(const void* data);

	void BindVS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;

	uint32_t GetWidth() const { return mTextureDescription.Width; }
	uint32_t GetHeight() const { return mTextureDescription.Height; }

	ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }
	
private:
	friend class SpriteRenderer;

	ID3D11ShaderResourceView* mShaderResourceView;
	D3D11_TEXTURE2D_DESC mTextureDescription;
};

} // namespace Graphics
} // namespace S

#endif // #ifndef INCLUDED_GRAPHICS_TEXTURE_H