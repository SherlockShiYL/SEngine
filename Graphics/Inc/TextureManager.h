#ifndef INCLUDED_GRAPHICS_TEXTUREMANAGER_H
#define INCLUDED_GRAPHICS_TEXTUREMANAGER_H

namespace S {
namespace Graphics {

class Texture;

class TextureManager
{
public:
	static void StaticInitialize(const char* root);
	static void StaticTerminate();
	static TextureManager* Get();

public:
	TextureManager();
	~TextureManager();

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	void SetRootPath(const char* path);

	TextureId Load(const char* fileName);
	void Clear();

	void BindVS(TextureId id, uint32_t slot = 0);
	void BindPS(TextureId id, uint32_t slot = 0);

	Texture* GetTexture(TextureId id);

private:
	std::string mRoot;
	std::unordered_map<std::size_t, Texture*> mInventory;
};

} // namespace Graphics
} // namespace S

#endif // #ifndef INCLUDED_GRAPHICS_TEXTUREMANAGER_H