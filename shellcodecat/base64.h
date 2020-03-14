#pragma once

#include <vector>
#include <string>
typedef unsigned char BYTE;

BYTE* ReadBase64(std::string &b64code, size_t &bufSize);
BYTE* ReadBase64(char* b64code, size_t &bufSize);
std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);

