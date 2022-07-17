#ifndef FORMAT_H
#define FORMAT_H

#include <string>
//To resolve formatting (from feedback)
#include <iomanip>

namespace Format {
std::string ElapsedTime(long times);  // See src/format.cpp
};                                    // namespace Format

#endif