#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace xf {

void CopyToClipboard(const char* lpStr);

std::string GetCurTimeStr();

// IO
std::string GetModuleDir();
bool PathExist(std::string path);
std::string BrowseForFolder(std::string initDir);

// String

template<class T> inline
std::string tostr(T var, std::streamsize nwidth = 0, char filler = '0')
{
	std::stringstream s;
	s << std::setw(nwidth) << std::setfill(filler) << var;
	return s.str();
}

template<class T>
std::string GetXYDataString(std::vector<T> x,
                            std::vector<T> y, 
                            std::string separator, 
                            int precision_x = 0, 
                            int precision_y = 0,
                            BOOL fixed_x = false,
                            BOOL fixed_y = false);

template std::string GetXYDataString<double>(std::vector<double> x,
                                             std::vector<double> y,
                                             std::string separator,
                                             int precision_x,
                                             int precision_y,
                                             BOOL fixed_x,
                                             BOOL fixed_y);

}
