#ifndef INCLUDED_GRAPHICS_FONT_H
#define INCLUDED_GRAPHICS_FONT_H

interface IFW1Factory;
interface IFW1FontWrapper;

namespace S {
namespace Graphics {

class Font
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static Font* Get();

public:
	Font();
	~Font();

	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	void Initialize();
	void Terminate();

	void Draw(const wchar_t* str, float x, float y, float size, uint32_t color);

private:
	IFW1Factory* mFW1Factory;
	IFW1FontWrapper* mFontWrapper;
};

} // namespace Graphics
} // namespace S

#endif // #ifndef INCLUDED_GRAPHICS_FONT_H