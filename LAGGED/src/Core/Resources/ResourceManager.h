#pragma once
#include "Utility/String.h"

#include <unordered_map>
#include <memory>

#include "Resource.h"
//#include ""

namespace LAG
{
	class ResourceManager
	{
	public:
		ResourceManager(const ResourceManager&) = delete;
		static ResourceManager& Get()
		{
			static ResourceManager instance;
			return instance;
		}

		template<typename T, typename... Args>
		bool AddResource(const Utility::String& path, Args&&... args)
		{
			//Check if template type is of type "Resource"
			constexpr bool validResType = std::is_base_of<LAG::Resource, T>::value; 
			if constexpr (validResType)
			{
				T* resPtr = new T(path, args...);
				resPtr->Load();
				m_Resources.emplace(path.GetValue(), std::move(resPtr));
				return true;
			}
			else if (!validResType)
			{
				LAG_ASSERT("Cannot load resource: Template type of is not child of type \"Resource\".");
			}
			return false;
		}

		template<typename T>
		T* GetResource(size_t assetID) const
		{
			T* res = dynamic_cast<T*>(m_Resources.find(assetID)->second.get());
			if (!res)
			{
				Utility::Logger::Error("Could not find resource.");
				return nullptr;
			}
			else return res;
		}

		template<typename T>
		T* GetResource(const Utility::String& path) const
		{
			return GetResource<T>(path.GetValue());
		}

		void Clear();

	private:
		ResourceManager();
		~ResourceManager();

		std::unordered_map<size_t, std::unique_ptr<LAG::Resource>> m_Resources;
	};
}