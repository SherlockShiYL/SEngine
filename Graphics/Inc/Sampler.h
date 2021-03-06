#ifndef INCLUDED_GRAPHICS_SAMPLER_H
#define INCLUDED_GRAPHICS_SAMPLER_H

namespace S {
namespace Graphics {

class Sampler
{
public:
	enum class Filter { Point, Linear, Anisotropic };
	enum class AddressMode { Border, Clamp, Mirror, Wrap, };

	Sampler();
	~Sampler();
	
	void Initialize(Filter filter, AddressMode addressMode);
	void Terminate();

	void BindVS(uint32_t slot = 0) const;
	void BindGS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;

private:
	Sampler(const Sampler&) = delete;
	Sampler& operator=(const Sampler&) = delete;

	ID3D11SamplerState* mSampler;
};

} // namespace Graphics
} // namespace S

#endif // #ifndef INCLUDED_GRAPHICS_SAMPLER_H