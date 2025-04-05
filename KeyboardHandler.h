#pragma once
#include <queue>
#include <bitset>
#include <optional>
//This class is also a singleton
class KeyboardHandler {
	friend class Window;

public:
	class Event {
	public:
		enum Type {
			DOWN, UP, INVALID
		};
		Event() : type(type), code(code) {};
		Event(Type type, unsigned char code) noexcept : type(type), code(code) {};
		bool isDown() const noexcept { return type == Type::DOWN; }
		bool isUp() const noexcept { return type == Type::UP; }
		bool isValid() const noexcept { return type != Type::INVALID; }
		unsigned char getCode() { return code; };
	private:
		Type type;
		unsigned char code;
	};

	KeyboardHandler() = default;
	KeyboardHandler(const KeyboardHandler&) = delete;
	KeyboardHandler& operator=(const KeyboardHandler&) = delete;

	bool keyDown(unsigned char keycode) const noexcept;
	std::optional<Event> readKey() noexcept;
	bool keyEmpty() const noexcept;
	void clearKey() noexcept;

	std::optional<char> readChar() noexcept;
	bool charEmpty() const noexcept;
	void clearChar() noexcept;
	void clear() noexcept;

	void enableAutorepeat() noexcept;
	void disableAutorepeat() noexcept;
	bool autorepeatOn() const noexcept;

private:
	void onKeyDown(unsigned char keycode) noexcept;
	void onKeyUp(unsigned char keycode) noexcept;
	void onChar(char c) noexcept;
	void clearState() noexcept;
	template<typename T>
	static void trimBuffer(std::queue<T>& buf) noexcept;

	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufSize = 16u;
	bool autorepeat = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};