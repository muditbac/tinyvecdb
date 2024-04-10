#include <iostream>
#include "ds.h"

class NullBuffer : public std::streambuf {
public:
  int overflow(int c) { return c; }
};

NullBuffer _null_buf;
std::ostream _null_stream(&_null_buf);

std::ostream& get_null_stream(){
    return _null_stream;
}