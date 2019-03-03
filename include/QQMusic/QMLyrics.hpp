#ifndef QMLYRICS_HPP
#define QMLYRICS_HPP
#include <string>
namespace QMT{
  class QMLyrics{
    public:
      QMLyrics()=default;
      std::string decryptQRC(std::string content);
  };
}
#endif
