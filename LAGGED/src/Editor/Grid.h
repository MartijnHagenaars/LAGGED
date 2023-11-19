#pragma once

namespace LAG
{
	class GridBase
	{
	public:
		GridBase();
		virtual ~GridBase() {};

		virtual void Render(uint32_t cameraEntityID) = 0;

	private:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;

	};
}
