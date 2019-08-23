#ifndef BASIC_INSTANCE_READER_H_
#define BASIC_INSTANCE_READER_H_

#include "reader.h"
#include "N3LDG.h"
#include <sstream>
#include "driver.h"
#include <vector>
#include <string>

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
        std::vector<int> ids;
        for (std::string &word : m_instance_.m_words_) {
            ids.push_back(read_driver_.model_params_.word_alpha.from_string(word));
        }
        m_instance_.m_words_id_.clear();
        m_instance_.m_words_id_ = ids;
        return &m_instance_;
    }
};
#endif // BASIC_INSTANCE_READER_H_