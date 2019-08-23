#ifndef BASIC_INSTANCE_READER_H_
#define BASIC_INSTANCE_READER_H_

#include "reader.h"
#include "N3LDG.h"
#include <sstream>
#include <vector>
#include <string>
#include "utf.h"

class InstanceReader : public Reader {
public:
    Driver read_driver_;
    Instance *getNext() {
        m_instance_.clear();
        string strLine1;
        if ( (!my_getline(m_inf_, strLine1)) || (strLine1.empty())) {
            return nullptr;
        } 
        split_bychar(strLine1, m_instance_.m_words_, (const char *)(' '));
        return &m_instance_;
    }
};
#endif // BASIC_INSTANCE_READER_H_