#include "Precompiled.h"
#include "TileMap.h"

using namespace S;

TileMap::TileMap()
{
}

void TileMap::Load(const char * levelFile, const char * tileSetFile)
{
	LoadLevel(levelFile);
	LoadTileSet(tileSetFile);
}

void TileMap::LoadLevel(const char * levelFile)
{
	SafeDeleteArray(mMap);

	FILE* file = nullptr;
	fopen_s(&file, levelFile, "r");
	ASSERT(file != nullptr, "TileMap Failed to open files %s", levelFile);

	fscanf_s(file, "TileSize: %f\n", &mTileSize);
	fscanf_s(file, "Digits: %d\n", &mDigits);
	fscanf_s(file, "Rows: %d\n", &mRows);
	fscanf_s(file, "Columns: %d\n", &mColumns);

	mMap = new int[mColumns*mRows*mDigits];
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns * mDigits; ++x)
		{
			int index = GetIndex(x, y);
			mMap[index] = fgetc(file) - '0';
		}
		fgetc(file);
	}
	fclose(file);
}

void TileMap::LoadTileSet(const char * tileSetFile)
{
	SafeDeleteArray(mTileIds);

	FILE* file = nullptr;
	fopen_s(&file, tileSetFile, "r");
	ASSERT(file != nullptr, "TileMap Failed to open file %s", tileSetFile);

	fscanf_s(file, "Count: %d", &mTileCount);
	mTileIds = new Graphics::TextureId[mTileCount];

	for (int x = 0; x < mTileCount; ++x)
	{
		char buffer[128];
		fscanf_s(file, "%s", buffer, 128);
		mTileIds[x] = Graphics::LoadTexture(buffer);
	}
	fclose(file);
}

void TileMap::ServerLevel(const char * levelFile)
{
	FILE* file = nullptr;
	fopen_s(&file, levelFile, "r");
	ASSERT(file != nullptr, "TileMap Failed to open files %s", levelFile);

	fscanf_s(file, "TileSize: %f\n", &mTileSize);
	fscanf_s(file, "Digits: %d\n", &mDigits);
	fscanf_s(file, "Rows: %d\n", &mRows);
	fscanf_s(file, "Columns: %d\n", &mColumns);

	mMap = new int[mColumns*mRows*mDigits];
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns * mDigits; ++x)
		{
			int index = GetIndex(x, y);
			mMap[index] = fgetc(file) - '0';
		}
		fgetc(file);
	}
	fclose(file);
}

void TileMap::Unload()
{
	SafeDeleteArray(mTileIds);
	SafeDeleteArray(mMap);
}

void TileMap::Render(Math::Vector2 p, float scale, bool renderLine)
{
	int tempYmin = p.y <= 0.0f ? static_cast<int>(-p.y / mTileSize / scale) : 0;
	int tempYmax = static_cast<int>((static_cast<float>(Graphics::GetScreenHeight()) - p.y) / mTileSize / scale) + 1;
	tempYmax = mRows <= tempYmax ? mRows : tempYmax;

	int tempXmin = p.x <= 0.0f ? static_cast<int>(-p.x / mTileSize / scale) : 0;
	int tempXmax = static_cast<int>((static_cast<float>(Graphics::GetScreenWidth()) - p.x) / mTileSize / scale) + 1;
	tempXmax = mColumns <= tempXmax ? mColumns : tempXmax;

	for (int y = tempYmin; y < tempYmax; ++y)
	{
		for (int x = tempXmin; x < tempXmax; ++x)
		{
			int tile = 0;
			for (int i = 0; i < mDigits; ++i)
			{
				int index = GetIndex(x*mDigits + i, y);
				tile += mMap[index] * static_cast<int>(pow(10, (mDigits - 1 - i)));
			}
			Math::Vector2 position = { mTileSize*scale*x + 0.5f*mTileSize*scale, mTileSize*scale*y + 0.5f*mTileSize*scale };
			Graphics::DrawSprite(mTileIds[tile], position + p, 0.0f, scale);
		}
	}
	if (renderLine)
	{
		for (int i = 1; i <= mColumns; ++i)
		{
			Graphics::DrawScreenLine({ static_cast<float>(i)*GetTileSize()*scale + p.x ,p.y }, { static_cast<float>(i)*GetTileSize()*scale + p.x, GetTileSize()*scale*static_cast<float>(mRows) + p.y }, Math::Vector4::Gray());
		}
		for (int i = 1; i <= mRows; ++i)
		{
			Graphics::DrawScreenLine({ p.x,static_cast<float>(i)*GetTileSize()*scale + p.y }, { GetTileSize()*scale*static_cast<float>(mColumns) + p.x,static_cast<float>(i)*GetTileSize()*scale + p.y }, Math::Vector4::Gray());
		}
	}
}

void TileMap::RenderMenu()
{
	int w = Graphics::GetScreenWidth() / static_cast<int>(mTileSize);
	int h = mTileCount / w;
	for (int j = 0; j <= h; ++j)
	{
		for (int i = 0; i < w; ++i)
		{
			int test = i + j * w;
			float t1 = (float)i*mTileSize + mTileSize * 0.5f;
			float t2 = j * mTileSize + mTileSize * 0.5f;
			if (i + j * w <= mTileCount)
			{
				Graphics::DrawSprite(mTileIds[i + j * w], { (float)i*mTileSize + mTileSize * 0.5f, j*mTileSize + mTileSize * 0.5f });
			}
			else
			{
				break;
			}
		}
	}
	for (int i = 1; i <= w; ++i)
	{
		Graphics::DrawScreenLine({ static_cast<float>(i)*GetTileSize() ,0.0f }, { static_cast<float>(i)*GetTileSize(), GetTileSize()*static_cast<float>(h + 1) }, Math::Vector4::Gray());
	}
	for (int i = 1; i <= h; ++i)
	{
		Graphics::DrawScreenLine({ 0.0f,static_cast<float>(i)*GetTileSize() }, { GetTileSize()*static_cast<float>(mColumns),static_cast<float>(i)*GetTileSize() }, Math::Vector4::Gray());
	}
}

void TileMap::SaveLevel(const char * fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName, "w");

	fprintf(file, "TileSize: %.6f\n", mTileSize);
	fprintf(file, "Digits: %d\n", mDigits);
	fprintf(file, "Rows: %d\n", mRows);
	fprintf(file, "Columns: %d\n\n", mColumns);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns*mDigits; ++x)
		{
			int i = GetIndex(x, y);
			fprintf(file, "%d", mMap[i]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
}

int TileMap::GetTile(int x, int y)
{
	int tile = 0;
	for (int i = 0; i < mDigits; ++i)
	{
		int index = GetIndex(x*mDigits + i, y);
		tile += mMap[index] * static_cast<int>(pow(10, (mDigits - 1 - i)));
	}
	return tile;
}

int TileMap::GetTile(Math::Vector2 position)
{
	return GetTile(static_cast<int>(position.x / mTileSize), static_cast<int>(position.y / mTileSize));
}

void TileMap::SetTile(int x, int y, int Tile)
{
	if (x >= 0 && x < mColumns&&y >= 0 && y < mRows)
	{
		int temp = Tile;
		for (int i = 0; i < mDigits; ++i)
		{
			int TileIndex = GetIndex(x*mDigits + i, y);
			mMap[TileIndex] = Tile / static_cast<int>(pow(10, (mDigits - 1 - i))) % 10;
		}
	}
}

int TileMap::ChooseTile(int x, int y)
{
	int w = x / static_cast<int>(mTileSize);
	int h = y / static_cast<int>(mTileSize);
	return w + h * Graphics::GetScreenWidth() / static_cast<int>(mTileSize);
}

Geometry::Rect TileMap::GetTileRect(int x, int y)
{
	return Geometry::Rect{ static_cast<float>(x)*mTileSize,static_cast<float>(y)*mTileSize,
		static_cast<float>(x + 1)*mTileSize,static_cast<float>(y + 1)*mTileSize };
}

Geometry::Rect TileMap::GetTileRect(Math::Vector2 position)
{
	int x = static_cast<int>(position.x / mTileSize);
	int y = static_cast<int>(position.y / mTileSize);
	return Geometry::Rect{ static_cast<float>(x)*mTileSize,static_cast<float>(y)*mTileSize,
		static_cast<float>(x + 1)*mTileSize,static_cast<float>(y + 1)*mTileSize };
}
