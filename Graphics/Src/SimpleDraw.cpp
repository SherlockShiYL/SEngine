#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "GraphicsSystem.h"
#include "PixelShader.h"
#include "VertexTypes.h"
#include "VertexShader.h"

using namespace S;
using namespace S::Graphics;

namespace
{
	const char* kSimpleShader =
		"cbuffer CBSimpleDraw : register(b0)"
		"{"
		"	matrix transform;"
		"}"
		"struct A2V"
		"{"
		"	float4 position : POSITION;"
		"	float4 color : COLOR;"
		"};"
		"struct V2P"
		"{"
		"	float4 position : SV_POSITION;"
		"	float4 color : COLOR;"
		"};"
		"V2P VS(A2V input)"
		"{"
		"	V2P output = (V2P)0;"
		"	output.position = mul(input.position, transform);"
		"	output.color = input.color;"
		"	return output;"
		"}"
		"float4 PS(V2P input) : SV_Target"
		"{"
		"	return input.color;"
		"}";

	struct CBSimpleDraw
	{
		Math::Matrix4 transform;
	};

	class SimpleDrawImpl
	{
	public:
		// Constructor
		SimpleDrawImpl();

		// Destructor
		~SimpleDrawImpl();

		// Functions to startup/shutdown simple draw
		void Initialize(uint32_t maxVertices);
		void Terminate();

		// Functions to add 3D lines
		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector4& color);
		void AddAABB(const Geometry::AABB& aabb, const Math::Vector4& color);
		void AddOBB(const Geometry::OBB& obb, const Math::Vector4& color);
		void AddSphere(const Geometry::Sphere& sphere, const Math::Vector4& color, uint32_t slices, uint32_t rings);
		void AddTransform(const Math::Matrix4& transform);

		// Functions to add screen lines
		void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color);
		void AddScreenRect(const Geometry::Rect& rect, const Math::Vector4& color);
		void AddScreenCircle(const Geometry::Circle& circle, const Math::Vector4& color);
		void AddScreenDiamond(const Math::Vector2& center, float size, const Math::Vector4& color);

		// Function to render all the lines added
		void Render(const Graphics::Camera& camera);

	private:
		Graphics::VertexShader mVertexShader;
		Graphics::PixelShader mPixelShader;

		TypedConstantBuffer<CBSimpleDraw> mConstantBuffer;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mVertexBuffer2D;

		Graphics::VertexPC* mVertices3D;
		Graphics::VertexPC* mVertices2D;

		uint32_t mMaxVertices;
		uint32_t mNumVertices3D;
		uint32_t mNumVertices2D;

		bool mInitialized;
	};

	SimpleDrawImpl::SimpleDrawImpl()
		: mVertexBuffer(nullptr)
		, mVertexBuffer2D(nullptr)
		, mVertices3D(nullptr)
		, mVertices2D(nullptr)
		, mMaxVertices(0)
		, mNumVertices2D(0)
		, mNumVertices3D(0)
		, mInitialized(false)
	{
	}

	SimpleDrawImpl::~SimpleDrawImpl()
	{
		ASSERT(!mInitialized, "[SimpleDraw] System not shutdown properly.");
	}

	void SimpleDrawImpl::Initialize(uint32_t maxVertices)
	{
		ASSERT(!mInitialized, "[SimpleDraw] Already initialized.");

		const uint32_t kSimpleShaderSize = (uint32_t)strlen(kSimpleShader) + 1;
		mVertexShader.Initialize(kSimpleShader, kSimpleShaderSize, VertexPC::Format, "VS", "vs_5_0");
		mPixelShader.Initialize(kSimpleShader, kSimpleShaderSize, "PS", "ps_5_0");
		mConstantBuffer.Initialize();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = maxVertices * sizeof(VertexPC);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		// Create vertex buffers for 3D/2D lines
		ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
		device->CreateBuffer(&bd, nullptr, &mVertexBuffer);
		device->CreateBuffer(&bd, nullptr, &mVertexBuffer2D);

		// Create line buffers
		mVertices3D = new VertexPC[maxVertices];
		mVertices2D = new VertexPC[maxVertices];

		mMaxVertices = maxVertices;
		mNumVertices3D = 0;
		mNumVertices2D = 0;

		// Set flag
		mInitialized = true;
	}

	void SimpleDrawImpl::Terminate()
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Release everything
		SafeDeleteArray(mVertices2D);
		SafeDeleteArray(mVertices3D);

		SafeRelease(mVertexBuffer2D);
		SafeRelease(mVertexBuffer);

		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();

		// Clear flag
		mInitialized = false;
	}

	void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector4& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices3D + 2 <= mMaxVertices)
		{
			// Add line
			mVertices3D[mNumVertices3D++] = { v0, color };
			mVertices3D[mNumVertices3D++] = { v1, color };
		}

		ASSERT(mNumVertices3D < mMaxVertices, "[SimpleDraw] Too many vertices!");
	}

	void SimpleDrawImpl::AddAABB(const Geometry::AABB& aabb, const Math::Vector4& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices3D + 24 <= mMaxVertices)
		{
			float minX = aabb.center.x - aabb.extend.x;
			float minY = aabb.center.y - aabb.extend.y;
			float minZ = aabb.center.z - aabb.extend.z;
			float maxX = aabb.center.x + aabb.extend.x;
			float maxY = aabb.center.y + aabb.extend.y;
			float maxZ = aabb.center.z + aabb.extend.z;

			// Add lines
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, minZ), color };
		}

		ASSERT(mNumVertices3D < mMaxVertices, "[SimpleDraw] Too many vertices!");
	}

	void SimpleDrawImpl::AddOBB(const Geometry::OBB& obb, const Math::Vector4& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
		Math::Matrix4 matRot = Math::MatrixRotationQuaternion(obb.orientation);
		Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
		Math::Matrix4 toWorld = matScale * matRot * matTrans;

		Math::Vector3 points[] =
		{
			Math::Vector3(-1.0f, -1.0f, -1.0f),
			Math::Vector3(-1.0f,  1.0f, -1.0f),
			Math::Vector3(1.0f,  1.0f, -1.0f),
			Math::Vector3(1.0f, -1.0f, -1.0f),
			Math::Vector3(-1.0f, -1.0f,  1.0f),
			Math::Vector3(-1.0f,  1.0f,  1.0f),
			Math::Vector3(1.0f,  1.0f,  1.0f),
			Math::Vector3(1.0f, -1.0f,  1.0f)
		};

		for (uint32_t i = 0; i < 8; ++i)
		{
			points[i] = Math::TransformCoord(points[i], toWorld);
		}

		AddLine(points[0], points[1], color);
		AddLine(points[1], points[2], color);
		AddLine(points[2], points[3], color);
		AddLine(points[3], points[0], color);

		AddLine(points[0], points[4], color);
		AddLine(points[1], points[5], color);
		AddLine(points[2], points[6], color);
		AddLine(points[3], points[7], color);

		AddLine(points[4], points[5], color);
		AddLine(points[5], points[6], color);
		AddLine(points[6], points[7], color);
		AddLine(points[7], points[4], color);
	}

	void SimpleDrawImpl::AddSphere(const Geometry::Sphere& sphere, const Math::Vector4& color, uint32_t slices, uint32_t rings)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		const float x = sphere.center.x;
		const float y = sphere.center.y;
		const float z = sphere.center.z;
		const float radius = sphere.radius;

		const uint32_t kSlices = Math::Max(3u, slices);
		const uint32_t kRings = Math::Max(2u, rings);
		const uint32_t kLines = (4 * kSlices * kRings) - (2 * kSlices);

		// Check if we have enough space
		if (mNumVertices3D + kLines <= mMaxVertices)
		{
			// Add lines
			const float kTheta = Math::kPi / (float)kRings;
			const float kPhi = Math::kTwoPi / (float)kSlices;
			for (uint32_t j = 0; j < kSlices; ++j)
			{
				for (uint32_t i = 0; i < kRings; ++i)
				{
					const float a = i * kTheta;
					const float b = a + kTheta;
					const float ay = radius * cos(a);
					const float by = radius * cos(b);

					const float theta = j * kPhi;
					const float phi = theta + kPhi;

					const float ar = sqrt(radius * radius - ay * ay);
					const float br = sqrt(radius * radius - by * by);

					const float x0 = x + (ar * sin(theta));
					const float y0 = y + (ay);
					const float z0 = z + (ar * cos(theta));

					const float x1 = x + (br * sin(theta));
					const float y1 = y + (by);
					const float z1 = z + (br * cos(theta));

					const float x2 = x + (br * sin(phi));
					const float y2 = y + (by);
					const float z2 = z + (br * cos(phi));

					mVertices3D[mNumVertices3D++] = { Math::Vector3(x0, y0, z0), color };
					mVertices3D[mNumVertices3D++] = { Math::Vector3(x1, y1, z1), color };

					if (i < kRings - 1)
					{
						mVertices3D[mNumVertices3D++] = { Math::Vector3(x1, y1, z1), color };
						mVertices3D[mNumVertices3D++] = { Math::Vector3(x2, y2, z2), color };
					}
				}
			}
		}

		ASSERT(mNumVertices3D < mMaxVertices, "[SimpleDraw] Too many vertices!");
	}

	void SimpleDrawImpl::AddTransform(const Math::Matrix4& transform)
	{
		Math::Vector3 position = Math::GetTranslation(transform);
		Math::Vector3 right = Math::GetRight(transform);
		Math::Vector3 up = Math::GetUp(transform);
		Math::Vector3 forward = Math::GetForward(transform);
		AddLine(position, position + right, Math::Vector4::Red());
		AddLine(position, position + up, Math::Vector4::Green());
		AddLine(position, position + forward, Math::Vector4::Blue());
	}

	void SimpleDrawImpl::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 2 <= mMaxVertices)
		{
			// Add line
			mVertices2D[mNumVertices2D++] = { Math::Vector3(v0.x, v0.y, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(v1.x, v1.y, 0.0f), color };
		}

		ASSERT(mNumVertices2D < mMaxVertices, "[SimpleDraw] Too many vertices!");
	}

	void SimpleDrawImpl::AddScreenRect(const Geometry::Rect& rect, const Math::Vector4& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 8 <= mMaxVertices)
		{
			float l = rect.left;
			float t = rect.top;
			float r = rect.right;
			float b = rect.bottom;

			// Add lines
			mVertices2D[mNumVertices2D++] = { Math::Vector3(l, t, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(r, t, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(r, t, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(r, b, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(r, b, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(l, b, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(l, b, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(l, t, 0.0f), color };
		}

		ASSERT(mNumVertices2D < mMaxVertices, "[SimpleDraw] Too many vertices!");
	}

	void SimpleDrawImpl::AddScreenCircle(const Geometry::Circle& circle, const Math::Vector4& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 32 <= mMaxVertices)
		{
			float x = circle.center.x;
			float y = circle.center.y;
			float r = circle.radius;

			// Add line
			const float kAngle = Math::kPi / 8.0f;
			for (uint32_t i = 0; i < 16; ++i)
			{
				const float alpha = i * kAngle;
				const float beta = alpha + kAngle;
				const float x0 = x + (r * sin(alpha));
				const float y0 = y + (r * cos(alpha));
				const float x1 = x + (r * sin(beta));
				const float y1 = y + (r * cos(beta));
				mVertices2D[mNumVertices2D++] = { Math::Vector3(x0, y0, 0.0f), color };
				mVertices2D[mNumVertices2D++] = { Math::Vector3(x1, y1, 0.0f), color };
			}
		}

		ASSERT(mNumVertices2D < mMaxVertices, "[SimpleDraw] Too many vertices!");
	}

	void SimpleDrawImpl::AddScreenDiamond(const Math::Vector2& center, float size, const Math::Vector4& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 8 <= mMaxVertices)
		{
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y - size, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x + size, center.y, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x + size, center.y, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y + size, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y + size, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x - size, center.y, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x - size, center.y, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y - size, 0.0f), color };
		}

		ASSERT(mNumVertices2D < mMaxVertices, "[SimpleDraw] Too many vertices!");
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		GraphicsSystem* gs = GraphicsSystem::Get();
		const uint32_t screenWidth = gs->GetWidth();
		const uint32_t screenHeight = gs->GetHeight();

		const Math::Matrix4& matView = camera.GetViewMatrix();
		const Math::Matrix4& matProj = camera.GetPerspectiveMatrix();

		mVertexShader.Bind();
		mPixelShader.Bind();

		CBSimpleDraw cb;
		cb.transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Set(cb);
		mConstantBuffer.BindVS(0);

		UINT stride = sizeof(VertexPC);
		UINT offset = 0;

		ID3D11DeviceContext* context = gs->GetContext();

		// Draw 3D lines
		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, mVertices3D, mNumVertices3D * stride);
		context->Unmap(mVertexBuffer, 0);

		context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context->Draw(mNumVertices3D, 0);

		const uint32_t w = gs->GetWidth();
		const uint32_t h = gs->GetHeight();
		Math::Matrix4 matInvScreen
		(
			2.0f / w, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / h, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		);
		cb.transform = Math::Transpose(matInvScreen);
		mConstantBuffer.Set(cb);
		mConstantBuffer.BindVS(0);

		// Draw 2D lines
		context->Map(mVertexBuffer2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, mVertices2D, mNumVertices2D * stride);
		context->Unmap(mVertexBuffer2D, 0);

		context->IASetVertexBuffers(0, 1, &mVertexBuffer2D, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context->Draw(mNumVertices2D, 0);

		// Reset index
		mNumVertices3D = 0;
		mNumVertices2D = 0;
	}

	SimpleDrawImpl* sSimpleDrawImpl = nullptr;
}

// Function Definitions
void SimpleDraw::Initialize(uint32_t maxVertices)
{
	if (nullptr == sSimpleDrawImpl)
	{
		sSimpleDrawImpl = new SimpleDrawImpl();
		sSimpleDrawImpl->Initialize(maxVertices);
	}
}

void SimpleDraw::Terminate()
{
	if (nullptr != sSimpleDrawImpl)
	{
		sSimpleDrawImpl->Terminate();
		delete sSimpleDrawImpl;
		sSimpleDrawImpl = nullptr;
	}
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector4& color)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddLine(v0, v1, color);
}

void SimpleDraw::AddLine(float x0, float y0, float z0, float x1, float y1, float z1, const Math::Vector4& color)
{
	AddLine(Math::Vector3(x0, y0, z0), Math::Vector3(x1, y1, z1), color);
}

void SimpleDraw::AddAABB(const Geometry::AABB& aabb, const Math::Vector4& color)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddAABB(aabb, color);
}

void SimpleDraw::AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Math::Vector4& color)
{
	AddAABB(Geometry::AABB((min + max) * 0.5f, (max - min) * 0.5f), color);
}

void SimpleDraw::AddAABB(const Math::Vector3& center, float radius, const Math::Vector4& color)
{
	AddAABB(Geometry::AABB(center, Math::Vector3(radius, radius, radius)), color);
}

void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Math::Vector4& color)
{
	AddAABB(Math::Vector3(minX, minY, minZ), Math::Vector3(maxX, maxY, maxZ), color);
}

void SimpleDraw::AddOBB(const Geometry::OBB& obb, const Math::Vector4& color)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddOBB(obb, color);
}

void SimpleDraw::AddSphere(const Geometry::Sphere& sphere, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddSphere(sphere, color, slices, rings);
}

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	AddSphere(Geometry::Sphere(center, radius), color, slices, rings);
}

void SimpleDraw::AddSphere(float x, float y, float z, float radius, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	AddSphere(Geometry::Sphere(x, y, z, radius), color, slices, rings);
}

void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddTransform(transform);
}

void SimpleDraw::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenLine(v0, v1, color);
}

void SimpleDraw::AddScreenLine(float x0, float y0, float x1, float y1, const Math::Vector4& color)
{
	AddScreenLine(Math::Vector2(x0, y0), Math::Vector2(x1, y1), color);
}

void SimpleDraw::AddScreenRect(const Geometry::Rect& rect, const Math::Vector4& color)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenRect(rect, color);
}

void SimpleDraw::AddScreenRect(const Math::Vector2& min, const Math::Vector2& max, const Math::Vector4& color)
{
	AddScreenRect(Geometry::Rect(min.x, min.y, max.x, max.y), color);
}

void SimpleDraw::AddScreenRect(float left, float top, float right, float bottom, const Math::Vector4& color)
{
	AddScreenRect(Geometry::Rect(left, top, right, bottom), color);
}

void SimpleDraw::AddScreenCircle(const Geometry::Circle& circle, const Math::Vector4& color)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenCircle(circle, color);
}

void SimpleDraw::AddScreenCircle(const Math::Vector2& center, float r, const Math::Vector4& color)
{
	AddScreenCircle(Geometry::Circle(center, r), color);
}

void SimpleDraw::AddScreenCircle(float x, float y, float r, const Math::Vector4& color)
{
	AddScreenCircle(Geometry::Circle(x, y, r), color);
}

void SimpleDraw::AddScreenDiamond(const Math::Vector2& center, float size, const Math::Vector4& color)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenDiamond(center, size, color);
}

void SimpleDraw::AddScreenDiamond(float x, float y, float size, const Math::Vector4& color)
{
	AddScreenDiamond(Math::Vector2(x, y), size, color);
}

void SimpleDraw::Render(const Camera& camera)
{
	ASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->Render(camera);
}