#pragma once
#include "Utility/HashedString.h"

#include <unordered_map>
#include <memory>

#include "Resource.h"
#include "Utility/Logger.h"
#include "Core/Defines.h"

namespace LAG
{
	class ResourceManager
	{
	public:
		ResourceManager(const ResourceManager&) = delete;

		template<typename T, typename... Args>
		bool AddResource(const HashedString& path, Args&&... args)
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
		}

		template<typename T>
		T* GetResource(size_t assetID) const
		{
			auto resource = m_Resources.find(assetID);
			if (resource != m_Resources.end())
			{
				T* res = dynamic_cast<T*>(resource->second.get());
				if (res != nullptr)
					return res;
				else
					Logger::Error("Resource could not be converted.");
			}
			else 
				Logger::Error("Resource is not stored in resource manager.");

			return nullptr;
		}

		template<typename T>
		T* GetResource(const HashedString& path) const
		{
			return GetResource<T>(path.GetValue());
		}

		template<typename T>
		std::vector<HashedString> GetResourceNames() const
		{
			std::vector<HashedString> nameVec;
			for (const auto& it : m_Resources)
			{
				T* resourcePtr = dynamic_cast<T*>(it.second.get());
				if (resourcePtr != nullptr)
				{
					nameVec.emplace_back(it.second->GetPath());
				}
			}
			return nameVec;
		}


		void Clear();

	private:
		friend class Engine;
		ResourceManager();
		~ResourceManager();

		std::unordered_map<size_t, std::unique_ptr<LAG::Resource>> m_Resources;
	};
}