#pragma once

#include <string>

namespace kurt {

class PCMReader final
{
public:
    PCMReader() = default;
    ~PCMReader() = default;

    void read(const std::string &file_path);
};

} // namespace kurt
