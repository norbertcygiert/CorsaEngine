#include "Exceptions.h"
#include <sstream>
AstraException::AstraException(int line, const char* file) noexcept : line(line), file(file){}

const char* AstraException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl << getOriginString();
	buf = oss.str();
	return buf.c_str();
}

const char* AstraException::getType() const noexcept
{
	return "nullptr";
}

int AstraException::getLine() const noexcept
{
	return line;
}

const std::string& AstraException::getFile() const noexcept
{
	return file;
}

std::string AstraException::getOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "File: " << file << "\nLine: " << line;
	return oss.str();
}
