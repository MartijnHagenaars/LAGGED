#pragma once
#include <functional>

struct ChunkKey
{
	ChunkKey(int x, int y) :
		m_X(x), m_Y(y)
	{}


	int m_X, m_Y;
};

static bool operator==(const ChunkKey& lhs, const ChunkKey& rhs)
{
	return ((lhs.m_X == rhs.m_X) && (lhs.m_Y == rhs.m_Y));
}

//Implement std::hash functionality for ChunkKey struct.
//Required for use in a map
namespace std {
	template <> struct hash<ChunkKey> {
		size_t operator()(const ChunkKey& value) const noexcept
		{
			size_t xHash = std::hash<int>{}(value.m_X);
			size_t yHash = std::hash<int>{}(value.m_Y);

			return std::hash<size_t>{}((xHash ^ yHash) >> 2);
		}
	};
}
