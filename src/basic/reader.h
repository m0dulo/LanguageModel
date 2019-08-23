#ifndef BASIC_READER_H
#define BASIC_READER_H

#include <fstream>
#include <iostream>

#include "instance.h"

class Reader {
protected:
    std::ifstream m_inf_;
    int m_instance_num_;
    Instance m_instance_;

public:
    Reader() = default;
    virtual ~Reader() {
        if (!m_inf_.is_open())
            m_inf_.close();
    }

    int startReading(const char *filename) {
        if(m_inf_.is_open()) {
            m_inf_.close();
            m_inf_.clear();
        }

        m_inf_.open(filename);

        if (!m_inf_.is_open()) {
            std::cout << "Reader::startReading() open file err: " << filename << std::endl;
            return -1;
        }

        return 0;
    }

    void finishReading() {
        if (m_inf_.is_open()) {
            m_inf_.close();
            m_inf_.clear();
        }
    }

    virtual Instance *getNext() = 0;
};
#endif // BASIC_READER_H