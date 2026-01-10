#pragma once
#include <memory>
#include <unordered_map>

#include "ToolBase.h"
#include "UI/Gizmo.h"


namespace LAG
{
	class ToolsManager
	{
	public: 
		void Initialize();
		void Shutdown();

		template<typename T, typename... Args>
		void RegisterTool(Args&& ...args);

		void PresentEditor();

		/// <summary>
		/// Check if a specific tool is open and visible. Uses the internal tool ID for lookup.
		/// The tool ID is generated as a Hash64 of the tool's display name.
		/// </summary>
		/// <returns>Boolean that determines whether the tool is open or not.</returns>
		bool IsToolOpen(Hash64 toolID);

	private:
		void BeginDockSpace();
		void EndDockSpace();

		void LoadToolLayout();
		void SaveToolLayout();

		/// <summary>
		/// Map containing all registered tools as (unique_ptr) values, with an ID as the key. 
		/// The ID is generated as a Hash64 of the tool's display name and is used for internal identification.
		/// </summary>
		std::unordered_map<Hash64, std::unique_ptr<ToolBase>> m_Tools;

		std::unique_ptr<Gizmo> m_Gizmo;
	};

	template<typename T, typename... Args>
	inline void ToolsManager::RegisterTool(Args&& ...args)
	{
		static_assert(std::is_base_of_v<ToolBase, T>, "Cannot register tool: T must be a subclass of ToolBase.");

		constexpr Hash64 key = GetTypeHash64<T>();
		auto newTool = m_Tools.emplace(key, std::make_unique<T>(std::forward<Args>(args)...));
	}
}

