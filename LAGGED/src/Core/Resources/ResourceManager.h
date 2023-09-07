#pragma once
#include "Utility/String.h"

#include <unordered_map>
#include <memory>

#include "Resource.h"

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
			if constexpr (std::is_base_of<LAG::Resource, T>::value)
			{
				T* resPtr = std::make_unique<T>(args...);
				resPtr->Load();
				m_Resources.emplace(path.GetValue(), std::move(resPtr));
			}
			else LAG_ASSERT("Cannot load resource: Template type of is not child of type \"Resource\".");
			return false;
		}

		template<typename T>
		T& GetResource(const Utility::String& path) const
		{
			T* res = dynamic_cast<T*>(m_Resources.find(path.GetValue())->second);
			if (!res)
				Utility::Logger::Error("Could not find resource.");
			else return res;
		}

		void Clear();

	private:
		ResourceManager();
		~ResourceManager();

		std::unordered_map<size_t, std::unique_ptr<LAG::Resource>> m_Resources;
	};
}