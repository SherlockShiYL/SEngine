#ifndef INCLUDED_GRAPHICS_CONSTANTBUFFER_H
#define INCLUDED_GRAPHICS_CONSTANTBUFFER_H

namespace S {
namespace Graphics {

class ConstantBuffer
{
public:
	ConstantBuffer() = default;

	void Initialize(uint32_t bufferSize, const void* initData = nullptr);
	void Terminate();

	void Set(const void* data);

	void BindVS(uint32_t slot = 0);
	void BindGS(uint32_t slot = 0);
	void BindPS(uint32_t slot = 0);
	void BindCS(uint32_t slot = 0);

private:
	ID3D11Buffer* mConstantBuffer{ nullptr };

};

template <typename T>
class TypedConstantBuffer : public ConstantBuffer
{
public:
	void Initialize()
	{
		ConstantBuffer::Initialize(GetPaddedSize());
	}

	void Initialize(const T& data)
	{
		ConstantBuffer::Initialize(GetPaddedSize(), &data);
	}

	void Set(const T& data)
	{
		ConstantBuffer::Set(&data);
	}

private:
	uint32_t GetPaddedSize() const
	{
		const uint32_t typeSize = sizeof(T);
		const uint32_t bufferSize = (typeSize % 16) ? ((typeSize / 16) + 1) * 16 : typeSize;
		return bufferSize;
	}
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_CONSTANTBUFFER_H