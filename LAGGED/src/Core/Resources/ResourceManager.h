#pragma once
#include "Utility/Hash.h"

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

		/// <summary>
		/// Add a resource to the resource manager
		/// </summary>
		/// <typeparam name="T">Resource type. The object type needs to be derived from the Resource class.</typeparam>
		/// <param name="path">The path/handle that will be used to store and retrieve the resource.</param>
		/// <param name="...args">The arguments for constructing the resource object.</param>
		/// <returns>A pointer to the resource is returned if the object was created and stored correctly. If the object cannot be created, a nullptr is returned.</returns>
		template<typename T, typename... Args>
		T* AddResource(const StringHash64& path, Args&&... args)
		{
			//Check if template type is of type "Resource"
			constexpr bool validResType = std::is_base_of<LAG::Resource, T>::value; 
			if constexpr (validResType)
			{
				T* resPtr = new T(path, args...);
				if (!resPtr->Load())
					CRITICAL("Failed to load resource: {}", path);
				m_Resources.emplace(path.Value(), std::move(resPtr));
				return resPtr;
			}
			else if (!validResType)
			{
				LAG_ASSERT("Cannot load resource: Template type of is not child of type \"Resource\".");
			}

			return nullptr;
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
					ERROR("Resource could not be converted.");
			}
			else 
				ERROR("Resource is not stored in resource manager.");

			return nullptr;
		}

		template<typename T>
		T* GetResource(const StringHash64& path) const
		{
			return GetResource<T>(path.Value());
		}

		bool Contains(const StringHash64& path);

		template<typename T>
		std::vector<StringHash64> GetResourceNames() const
		{
			std::vector<StringHash64> nameVec;
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