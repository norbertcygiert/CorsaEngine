#include "BetterWin.h"
#include "MouseHandler.h"

std::pair<int, int> MouseHandler::getCursorPos() const noexcept { return { x, y }; }

std::optional<MouseHandler::RawDelta> MouseHandler::readRawDelta() noexcept
{
	if (rawDeltaBuffer.empty()) {
		return std::nullopt;
	}
	const RawDelta d = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return d;
}

int MouseHandler::getX() const noexcept { return x; }
int MouseHandler::getY() const noexcept { return y; }
bool MouseHandler::isInWindow() const noexcept { return inWindow; }
bool MouseHandler::lmbPressed() const noexcept { return lmbDown; }
bool MouseHandler::rmbPressed() const noexcept { return rmbDown; }

std::optional<MouseHandler::Event> MouseHandler::read() noexcept
{
	if (buf.size() > 0u) {
		MouseHandler::Event e = buf.front();
		buf.pop();
		return e;
	}
	return {};
}

void MouseHandler::clear() noexcept { buf = std::queue<Event>(); }
void MouseHandler::enableRaw() noexcept { rawEnabled = true; }
void MouseHandler::disableRaw() noexcept { rawEnabled = false; }
bool MouseHandler::rawOn() const noexcept { return rawEnabled; }

void MouseHandler::onMove(int newx, int newy) noexcept
{
	x = newx, y = newy;
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::MOVE, *this));
	trimBuffer();
}

void MouseHandler::onWindowLeave() noexcept
{
	inWindow = false;
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::WINDOW_LEAVE, *this));
	trimBuffer();
}

void MouseHandler::onWindowEnter() noexcept
{
	inWindow = true;
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::WINDOW_ENTER, *this));
	trimBuffer();
}

void MouseHandler::onRawDelta(int dx, int dy) noexcept
{
	rawDeltaBuffer.push({ dx,dy });
	trimBuffer();
}

void MouseHandler::onLMBDown(int x, int y) noexcept
{
	lmbDown = true;
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::LMB_DOWN, *this));
	trimBuffer();
}

void MouseHandler::onLMBUp(int x, int y) noexcept
{
	lmbDown = false;
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::LMP_UP, *this));
	trimBuffer();
}

void MouseHandler::onRMBDown(int x, int y) noexcept
{
	rmbDown = true;
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::RMB_DOWN, *this));
	trimBuffer();
}

void MouseHandler::onRMBUp(int x, int y) noexcept
{
	rmbDown = false;
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::RMB_UP, *this));
	trimBuffer();
}

void MouseHandler::onMWheelUp(int x, int y) noexcept
{
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::MWHEEL_UP, *this));
	trimBuffer();
}

void MouseHandler::onMWheelDown(int x, int y) noexcept
{
	buf.push(MouseHandler::Event(MouseHandler::Event::Type::MWHEEL_DOWN, *this));
	trimBuffer();
}

void MouseHandler::trimBuffer() noexcept
{
	while (buf.size() > bufferSize) {
		buf.pop();
	}
}

void MouseHandler::trimRawInputBuf() noexcept
{
	while (rawDeltaBuffer.size() > bufferSize) {
		rawDeltaBuffer.pop();
	}
}

void MouseHandler::onMWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA) {
		wheelDeltaCarry -= WHEEL_DELTA;
		onMWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA) {
		wheelDeltaCarry += WHEEL_DELTA;
		onMWheelDown(x, y);
	}
}