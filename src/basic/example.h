#ifndef BASIC_EXAMPLE_H_
#define BASIC_EXAMPLE_H_

#include <iostream>
#include <vector>
#include <string>

class Feature {
public:
    std::vector<std::string> m_post_;
    std::vector<std::string> m_response_;

    void clear() {
        m_post_.clear();
        m_response_.clear();
    }
};

class Example {
public:
    Feature m_feature_;
    std::vector<int> m_words_id_;
};
#endif // BASIC_EXAMPLE_H_