#include "Exceptions.h"
#include <sstream>
CorsaException::CorsaException(int line, const char* file) noexcept : line(line), file(file){}

const char* CorsaException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl << getOriginString();
	buf = oss.str();
	return buf.c_str();
}

const char* CorsaException::getType() const noexcept { return "Corsa Engine: Critical Error (Top Level Exception thrown)"; }
int CorsaException::getLine() const noexcept { return line; }
const std::string& CorsaException::getFile() const noexcept { return file; }

std::string CorsaException::getOriginString() const noexcept{
	std::ostringstream oss;
	oss << "Error thrown from:\n" << "[File] " << file << "\n[Line] " << line;
	return oss.str();
}
