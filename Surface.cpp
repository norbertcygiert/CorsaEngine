#define INCLUDE_ALL_WIN
#include "Surface.h"
#include <algorithm>
namespace Gdiplus {
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment(lib,"gdiplus.lib") //Link the gdiplus library

Surface::Surface(unsigned int width, unsigned int height) noexcept :
	pBuffer(std::make_unique<Color[]>(width * height)),
	width(width),
	height(height) {}

Surface& Surface::operator=(Surface&& source) noexcept {
	width = source.width;
	height = source.height;
	pBuffer = std::move(source.pBuffer);
	source.pBuffer = nullptr;
	return *this;
}

Surface::Surface(Surface&& source) noexcept :
	pBuffer(std::move(source.pBuffer)),
	width(source.width),
	height(source.height)
{}

Surface::~Surface() {}

void Surface::clear(Color fillValue) noexcept {
	memset(pBuffer.get(), fillValue.DWORD32, width * height * sizeof(Color));
}

void Surface::putPixel(unsigned int x, unsigned int y, Color c) noexcept(!IS_DEBUG) {
	assert(x >= 0 && "Surface::putPixel() : X parameter must be bigger than 0");
	assert(y >= 0 && "Surface::putPixel() : Y parameter must be bigger than 0");
	assert(x < width && "Surface::putPixel() : X cannot be bigger than surface's width");
	assert(y < height && "Surface::putPixel() : Y cannot be bigger than surface's height");
	pBuffer[static_cast<size_t>(y) * width + x] = c;
}

Surface::Color Surface::getPixel(unsigned int x, unsigned int y) const noexcept(!IS_DEBUG) {
	assert(x >= 0 && "Surface::getPixel() : X parameter must be bigger than 0");
	assert(y >= 0 && "Surface::getPixel() : Y parameter must be bigger than 0");
	assert(x < width && "Surface::getPixel() : X cannot be bigger than surface's width");
	assert(y < height && "Surface::getPixel() : Y cannot be bigger than surface's height");
	return pBuffer[static_cast<size_t>(y) * width + x];
}

unsigned int Surface::getWidth() const noexcept { return width; }

unsigned int Surface::getHeight() const noexcept { return height; }

Surface::Color* Surface::getBufferPtr() noexcept { return pBuffer.get();  }

const Surface::Color* Surface::getBufferPtr() const noexcept { return pBuffer.get(); }

const Surface::Color* Surface::getBufferPtrConst() const noexcept { return pBuffer.get();}

Surface Surface::fromFile(const std::string& filename) {
	unsigned int width = 0;
	unsigned int height = 0;
	std::unique_ptr<Color[]> pBuffer = nullptr; 
	{
		// convert Filename to wide string (for Gdiplus)
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, filename.c_str(), _TRUNCATE);


		Gdiplus::Bitmap bitmap(wideName);
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok) {
			std::stringstream ss;
			ss << "Surface::fromFile() -> File: " << filename << ": failed to load image.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}
		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		pBuffer = std::make_unique<Color[]>(width * height);

		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pBuffer[y * width + x] = c.GetValue();
			}
		}
	}

	return Surface(width, height, std::move(pBuffer));
}

void Surface::save(const std::string& filename) const {
	auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void {
			unsigned int numOfImgEncoders = 0;	// Image encoders
			unsigned int size = 0;	// Bytes of image encoder array
			Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

			Gdiplus::GetImageEncodersSize(&numOfImgEncoders, &size);
			if (size == 0) {
				std::stringstream ss;
				ss << "Surface::save() -> File: " << filename << ": failed to get encoder; size == 0.";
				throw Exception(__LINE__, __FILE__, ss.str());
			}

			pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
			if (pImageCodecInfo == nullptr) {
				std::stringstream ss;
				ss << "Surface::save() -> File: " << filename << ": failed to get encoder; failed to allocate memory.";
				throw Exception(__LINE__, __FILE__, ss.str());
			}

			GetImageEncoders(numOfImgEncoders, size, pImageCodecInfo);

			for (unsigned int j = 0; j < numOfImgEncoders; ++j) {
				if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
					*pClsid = pImageCodecInfo[j].Clsid;
					free(pImageCodecInfo);
					return;
				}
			}

			free(pImageCodecInfo);
			std::stringstream ss;
			ss << "Surface::save() -> File: " << filename << ": failed to get encoder; failed to find matching encoder.";
			throw Exception(__LINE__, __FILE__, ss.str());
		};

	CLSID bmpID;
	GetEncoderClsid(L"image/bmp", &bmpID);

	// Convert filename to wide string (for Gdiplus)
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, filename.c_str(), _TRUNCATE);

	Gdiplus::Bitmap bitmap(width, height, width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)pBuffer.get());
	if (bitmap.Save(wideName, &bmpID, nullptr) != Gdiplus::Status::Ok) {
		std::stringstream ss;
		ss << "Surface::save() -> File: " << filename << ": failed to save.";
		throw Exception(__LINE__, __FILE__, ss.str());
	}
}

void Surface::copy(const Surface& src) noexcept(!IS_DEBUG) {
	assert(width == src.width && "Surface::copy(): Provided width doesn't match");
	assert(height == src.height && "Surface::copy(): Provided height doesn't match");
	memcpy(pBuffer.get(), src.pBuffer.get(), width * height * sizeof(Color));
}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept : width(width), height(height), pBuffer(std::move(pBufferParam)){}

// Exceptions
Surface::Exception::Exception(int line, const char* file, std::string note) noexcept : AstraException(line, file), note(std::move(note)) {}

const char* Surface::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << AstraException::what() << std::endl
		<< "[Note] " << getNote();
	buf = oss.str();
	return buf.c_str();
}

const char* Surface::Exception::getType() const noexcept { return "Astra Engine - Surface Graphics Exception"; }
const std::string& Surface::Exception::getNote() const noexcept { return note; }