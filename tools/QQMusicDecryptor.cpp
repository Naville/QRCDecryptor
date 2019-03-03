#include <QQMusic/QMLyrics.hpp>
#include <fstream>
#include <gzip/decompress.hpp>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
using namespace QMT;
static char *Key1 = "!@#)(NHLiuy*$%%^&";
static char *Key2 = "123ZXC!@#)(*$%%^&";
static char *Key3 = "!@#)(*$%%^&abcDEF";
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

vector<char> HexToBytes(const string &hex) {
  vector<char> bytes;
  bytes.reserve(hex.length() * 2);

  for (unsigned int i = 0; i < hex.length(); i += 2) {
    char msb = hexCharToInt(hex.at(i));
    char lsb = hexCharToInt(hex.at(i + 1));
    bytes.push_back(msb * 16 + lsb);
  }

  return bytes;
}
int main(int argc, char const *argv[]) {
  if (argc != 3) {
    cerr << "Usage: %s /PATH/TO/INPUT /PATH/TO/OUTPUT\n";
    exit(-1);
  }
  cerr << "Reading QRC From:" << argv[1] << "\n";
  ifstream in(argv[1]);
  std::stringstream buffer;
  buffer << in.rdbuf();
  string hex = buffer.str();
  QMLyrics qml;
  string decompressed_data = qml.decryptQRC(hex);
  std::ofstream out(argv[2]);
  out << decompressed_data;
  out.close();
  return 0;
}
