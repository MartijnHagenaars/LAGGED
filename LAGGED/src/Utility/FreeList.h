#pragma once
#include <vector>
#include "Core/Defines.h"

namespace LAG
{
	// Not used?
	template<typename T>
	class FreeList
	{
	public:
		FreeList() = default;
		explicit FreeList(unsigned int capacity)
		{
			m_Vec.reserve(capacity);
		}

		~FreeList()
		{ }

		//Using a variadic parameter for different instances of the class template type
		template<typename... Args>
		int Add(Args&&... arguments)
		{
			int newItemIndex = -1;
			if (m_NextFreeIndex == -1)
			{
				newItemIndex = m_Vec.size();
				m_Vec.emplace_back(std::forward<T>(arguments)...);
			}
			else
			{
				newItemIndex = m_NextFreeIndex;

				assert(index < m_Vec.size() && IsIndexAlreadyRemoved(index));

				//Using std::addressof, just in case the template type has overloaded the & operator. 
				m_NextFreeIndex = *(const int *const)std::addressof(m_Vec[newItemIndex]);
				new (std::addressof(m_Vec[newItemIndex])) T(std::forward<Args>(arguments)...);
			}

			m_Size++; 
			return newItemIndex;
		}

		void Remove(int index)
		{
			assert(index < m_Vec.size() && !IsIndexAlreadyRemoved(index));
			T& vecItem = m_Vec[index];
			vecItem.~T();
			DEBUG_OPERATION(memset(std::addressof(m_Vec[index], 0xcc, sizeof(T))));
			*(int* const)std::addressof(m_Vec[index]) = m_NextFreeIndex;

			m_NextFreeIndex = index;
			m_Size--;
		}

	private:
		bool IsIndexAlreadyRemoved(int index)
		{
			if constexpr (sizeof(T) > sizeof(int))
			{
				int i = sizeof(int);
				const int* const p{ (const int* const)std::addressof(m_Vec[index]) };
				while ((p[index] == 0xcc) && (i < sizeof(T))) i++;
				return i == sizeof(T);
			}
			else
			{
				return true; 
			}
		}

		std::vector<T> m_Vec;
		int m_Size = 0;
		int m_NextFreeIndex = -1;
	};
}