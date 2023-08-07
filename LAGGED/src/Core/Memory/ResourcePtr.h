#pragma once

namespace LAG
{
	//An (experimental) implementation for a (smart) pointer. 
	//Can be shared just like any pointer, but can be invalidated all across the board using the reset() function. 
	template<typename T>
	class ResourcePtr
	{
	public:
		template<typename ...Args>
		ResourcePtr(Args&&... args)
			try
		{
			T* tempData = new T(args...);
			m_Data = &tempData;
			//*m_Data = new T(args...);
		}
		catch (...)
		{
			//In case of exception, ensure that all memory is unloaded.
			delete* m_Data;
			*m_Data = nullptr;
			m_Data = nullptr;
			throw;
		}

		ResourcePtr(T* obj) : 
			m_Data(&obj)
		{}

		ResourcePtr(const ResourcePtr& other) :
			m_Data(other.m_Data)
		{}

		ResourcePtr(ResourcePtr& other) :
			m_Data(other.m_Data)
		{}


		T* operator->()
		{
			return *m_Data;
		}

		T* Get() const
		{
			if (m_Data == nullptr)
				return nullptr;
			return *m_Data;
		}

		//Invalidates the pointer in all areas of the project.
		void Reset()
		{
			delete *m_Data;
			*m_Data = nullptr;
			m_Data = nullptr;
		}

	private:
		T** m_Data = nullptr;
	};
}