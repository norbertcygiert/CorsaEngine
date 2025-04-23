#pragma once
#include "BetterWin.h"
#include "Exceptions.h"
#include <string>
#include <cassert>
#include <memory>


class Surface {
public:
	class Color {
	public:
		unsigned int DWORD32; 
	public:
		constexpr Color() noexcept : DWORD32() {}
		constexpr Color(const Color& col) noexcept : DWORD32(col.DWORD32) {}
		constexpr Color(unsigned int dw) noexcept : DWORD32(dw) {}
		constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept : DWORD32((x << 24u) | (r << 16u) | (g << 8u) | b){}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept : DWORD32((r << 16u) | (g << 8u) | b) {}
		constexpr Color(Color col, unsigned char x) noexcept : Color((x << 24u) | col.DWORD32) {}
		Color& operator =(Color color) noexcept {
			DWORD32 = color.DWORD32;
			return *this;
		}
		constexpr unsigned char GetX() const noexcept { return DWORD32 >> 24u; }
		constexpr unsigned char GetA() const noexcept { return GetX(); }
		constexpr unsigned char GetR() const noexcept { return (DWORD32 >> 16u) & 0xFFu; }
		constexpr unsigned char GetG() const noexcept { return (DWORD32 >> 8u) & 0xFFu; }
		constexpr unsigned char GetB() const noexcept { return DWORD32 & 0xFFu; }
		void setX(unsigned char x) noexcept { DWORD32 = (DWORD32 & 0xFFFFFFu) | (x << 24u); }
		void setA(unsigned char a) noexcept { setX(a); }
		void setR(unsigned char r) noexcept { DWORD32 = (DWORD32 & 0xFF00FFFFu) | (r << 16u); }
		void setG(unsigned char g) noexcept { DWORD32 = (DWORD32 & 0xFFFF00FFu) | (g << 8u); }
		void setB(unsigned char b) noexcept { DWORD32 = (DWORD32 & 0xFFFFFF00u) | b; }
	};
public:
	class Exception : public CorsaException {
	private:
		std::string note;
	public:
		Exception(int line, const char* file, std::string note) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		const std::string& getNote() const noexcept;
	};

	Surface(unsigned int width, unsigned int height) noexcept;
	Surface(Surface&& source) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& source) noexcept;
	Surface& operator=(const Surface&) = delete;
	~Surface();
	void clear(Color fillValue) noexcept;
	void putPixel(unsigned int x, unsigned int y, Color c) noexcept(!IS_DEBUG);
	Color getPixel(unsigned int x, unsigned int y) const noexcept(!IS_DEBUG);
	unsigned int getWidth() const noexcept;
	unsigned int getHeight() const noexcept;
	Color* getBufferPtr() noexcept;
	const Color* getBufferPtr() const noexcept;
	const Color* getBufferPtrConst() const noexcept;
	static Surface fromFile(const std::string& name);
	void save(const std::string& filename) const;
	void copy(const Surface& src) noexcept(!IS_DEBUG);

private:
	std::unique_ptr<Color[]> pBuffer;
	unsigned int width;
	unsigned int height;
	Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;
};