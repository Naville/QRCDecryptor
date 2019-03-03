#include <QQMusic/QMLyrics.hpp>
#include <QQMusic/DES.hpp>
#include <gzip/decompress.hpp>
#include <iostream>
#include <mutex>
#include <vector>
using namespace std;
static mutex Lock;
static char *Key1 = "!@#)(NHLiuy*$%%^&";
static char *Key2 = "123ZXC!@#)(*$%%^&";
static char *Key3 = "!@#)(*$%%^&abcDEF";
namespace QMT {
static inline char hexCharToInt(char a1) {
  char v2; // [rsp+2h] [rbp-4h]

  if (a1 < 48 || a1 > 57) {
    if (a1 < 65 || a1 > 70) {
      if (a1 < 97 || a1 > 102)
        v2 = 0;
      else
        v2 = a1 - 97 + 10;
    } else {
      v2 = a1 - 65 + 10;
    }
  } else {
    v2 = a1 - 48;
  }
  return v2;
}
string QMLyrics::decryptQRC(string hex) {
  // De-hex first
  if (hex.length() % 2 != 0) {
    hex = "0" + hex;
  }
  cerr << "Decoding Hex String with length:" << hex.length() << "\n";
  vector<char> bytesVector;
  bytesVector.reserve(hex.length() * 2);
  for (unsigned int i = 0; i < hex.length(); i += 2) {
    char msb = hexCharToInt(hex.at(i));
    char lsb = hexCharToInt(hex.at(i + 1));
    bytesVector.push_back(msb * 16 + lsb);
  }

  char *data = bytesVector.data();
  decltype(bytesVector.size()) datalength = bytesVector.size();
  cerr << "Decrypting with data length:" << datalength << "\n";
  {
    std::lock_guard<mutex> guard(Lock);
    DES des;
    des.Ddes(data,Key1, datalength);
    des.des(data,Key2, datalength);
    des.Ddes(data,Key3, datalength);
  }
  return gzip::decompress(data, datalength);
}
} // namespace QMT
