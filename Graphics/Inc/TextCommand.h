#ifndef INCLUDED_GRAPHICS_TEXTCOMMAND_H
#define INCLUDED_GRAPHICS_TEXTCOMMAND_H

namespace S {
namespace Graphics {

struct TextCommand
{
	TextCommand(std::wstring inStr, float inSize, float inX, float inY, uint32_t inColor)
		: str(std::move(inStr))
		, size(inSize)
		, x(inX)
		, y(inY)
		, color(inColor)
	{}

	std::wstring str;
	float size, x, y;
	uint32_t color;
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_TEXTCOMMAND_H