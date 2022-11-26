#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>

namespace AN
{
    const std::string WHITESPACE = " \n\r\t\f\v";

    inline void rtrim(std::string &s)
    {
        size_t end = s.find_last_not_of(WHITESPACE);
        s = (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    template <typename T, int MaxLen, typename Container = std::deque<T>>
    class queue_data : std::queue<T, Container>
    {
    public:
        void push(const T &value)
        {
            if (this->size() == MaxLen)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                this->c.pop_front();
            }
            std::queue<T, Container>::push(value);
        }

        void clean()
        {
            while (this->size() > 0)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                this->c.pop_front();
            }
        }

        T receive()
        {
            T out_data;
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!this->c.empty())
                out_data = this->c.front();
            return out_data;
        }

    private:
        mutable std::mutex m_mutex;
    };
}
#endif // _UTILS_H_