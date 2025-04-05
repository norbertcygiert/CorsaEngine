#pragma once
#include <queue>
#include <optional>

class MouseHandler
{
	friend class Window;
public:
	struct RawDelta 
	{
		int x, y;
	};
	class Event
	{
	public:
		enum class Type
		{
			LMB_DOWN,
			LMP_UP,
			RMB_DOWN,
			RMB_UP,
			MWHEEL_UP,
			MWHEEL_DOWN,
			MOVE,
			WINDOW_ENTER,
			WINDOW_LEAVE,
		};
		Event(Type type, const MouseHandler& parent) noexcept :
			type(type),
			lmbDown(parent.lmbDown),
			rmbDown(parent.rmbDown),
			x(parent.x),
			y(parent.y)
		{}
		Type getType() const noexcept { return type; }
		std::pair<int, int> getCursorPos() const noexcept { return{ x,y }; }
		int getX() const noexcept { return x; }
		int getY() const noexcept { return y; }
		bool lmbPressed() const noexcept { return lmbDown; }
		bool rmbPressed() const noexcept { return rmbDown; }
	private:
		Type type;
		bool lmbDown;
		bool rmbDown;
		int x;
		int y;
	};
public:
	MouseHandler() = default;
	MouseHandler(const MouseHandler&) = delete;
	MouseHandler& operator=(const MouseHandler&) = delete;
	std::pair<int, int> getCursorPos() const noexcept;
	std::optional<RawDelta> readRawDelta() noexcept;
	int getX() const noexcept;
	int getY() const noexcept;
	bool isInWindow() const noexcept;
	bool lmbPressed() const noexcept;
	bool rmbPressed() const noexcept;
	std::optional<MouseHandler::Event> read() noexcept;
	bool empty() const noexcept { return buf.empty(); }
	void clear() noexcept;
	void enableRaw() noexcept;
	void disableRaw() noexcept;
	bool rawOn() const noexcept;
private:
	void onMove(int x, int y) noexcept;
	void onWindowLeave() noexcept;
	void onWindowEnter() noexcept;
	void onRawDelta(int dx, int dy) noexcept;
	void onLMBDown(int x, int y) noexcept;
	void onLMBUp(int x, int y) noexcept;
	void onRMBDown(int x, int y) noexcept;
	void onRMBUp(int x, int y) noexcept;
	void onMWheelUp(int x, int y) noexcept;
	void onMWheelDown(int x, int y) noexcept;
	void trimBuffer() noexcept;
	void trimRawInputBuf() noexcept;
	void onMWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x, y;
	bool lmbDown = false;
	bool rmbDown = false;
	bool inWindow = false;
	int wheelDeltaCarry = 0;
	bool rawEnabled = false;
	std::queue<Event> buf;
	std::queue<RawDelta> rawDeltaBuffer;
};
