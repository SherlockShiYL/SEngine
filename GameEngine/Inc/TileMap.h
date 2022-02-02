#ifndef INCLUDED_GAMEENGINE_TILEMAP_H
#define INCLUDED_GAMEENGINE_TILEMAP_H

namespace S {

class TileMap
{
public:
	TileMap();

	void Load(const char* levelFile, const char* tileSetFile);
	void LoadLevel(const char* levelFile);
	void LoadTileSet(const char* tileSetFile);
	void ServerLevel(const char* levelFile);

	void Unload();
	void Render(S::Math::Vector2 p, float scale, bool renderLine = true);
	void RenderMenu();

	void ExtendMap(int r, int c, const char* levelFile);
	void SaveLevel(const char* fileName);

	bool CheckCollision(const S::Geometry::LineSegment2D& edge) const;

	int GetTile(int x, int y);
	int GetTile(Math::Vector2 position); // Get Tile from position
	float GetTileSize() const						{ return mTileSize; }
	int GetTileCount() const						{ return mTileCount; }
	int GetMapRows() const							{ return mRows; }
	int GetMapColumns() const						{ return mColumns; }

	void SetMapRows(int rows)						{ mRows = rows; }
	void SetMapColumns(int columns)					{ mColumns = columns; }
	void SetTileSize(float size)					{ mTileSize = size; }
	Graphics::TextureId GetTextureId(int index)		{ return mTileIds[index]; }

	void SetTile(int x, int y, int Tile);
	int ChooseTile(int x, int y);

	Geometry::Rect GetTileRect(int x, int y);
	Geometry::Rect GetTileRect(Math::Vector2 position); // Get Tile from position

private:
	int GetIndex(int x, int y) const
	{
		if (x < 0 || x >= mColumns * mDigits || y < 0 || y >= mRows)
		{
			return -1;
		}
		return x + (y*mColumns*mDigits);
	}

	int* mMap = nullptr;
	Graphics::TextureId* mTileIds = nullptr;

	int mRows{ 0 };
	int mColumns{ 0 };
	int mTileCount{ 0 };
	int mDigits{ 1 };
	float mTileSize{ 0.0f };
};

} // namespace S

#endif // !INCLUDED_GAMEENGINE_TILEMAP_H
