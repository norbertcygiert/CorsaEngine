#include "KeyboardHandler.h"


bool KeyboardHandler::keyDown(unsigned char keycode) const noexcept {
	return keyStates[keycode];
}

std::optional<KeyboardHandler::Event> KeyboardHandler::readKey() noexcept {
	if (keyBuffer.size() > 0u) {
		KeyboardHandler::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	else return KeyboardHandler::Event();
};

bool KeyboardHandler::keyEmpty() const noexcept { return keyBuffer.empty(); }

std::optional<char> KeyboardHandler::readChar() noexcept{
	if (charBuffer.size() > 0u){
		unsigned char charcode = charBuffer.front();
		charBuffer.pop();
		return charcode;
	}
	return {};
}

bool KeyboardHandler::charEmpty() const noexcept{ return charBuffer.empty(); }
void KeyboardHandler::clearKey() noexcept { keyBuffer = std::queue<Event>(); }
void KeyboardHandler::clearChar() noexcept{ charBuffer = std::queue<char>(); }
void KeyboardHandler::clear() noexcept{ clearChar(); clearKey();}
void KeyboardHandler::enableAutorepeat() noexcept { autorepeat = true; }
void KeyboardHandler::disableAutorepeat() noexcept { autorepeat = false; }
bool KeyboardHandler::autorepeatOn() const noexcept{ return autorepeat; }

void KeyboardHandler::onKeyDown(unsigned char keycode) noexcept{
	keyStates[keycode] = true;
	keyBuffer.push(KeyboardHandler::Event(KeyboardHandler::Event::Type::DOWN, keycode));
	trimBuffer(keyBuffer);
}

void KeyboardHandler::onKeyUp(unsigned char keycode) noexcept{
	keyStates[keycode] = false;
	keyBuffer.push(KeyboardHandler::Event(KeyboardHandler::Event::Type::UP, keycode));
	trimBuffer(keyBuffer);
}

void KeyboardHandler::onChar(char character) noexcept{ charBuffer.push(character); trimBuffer(charBuffer); }
void KeyboardHandler::clearState() noexcept { keyStates.reset(); }

template<typename T>
void KeyboardHandler::trimBuffer(std::queue<T>& buffer) noexcept{
	while (buffer.size() > bufSize){
		buffer.pop();
	}
}

