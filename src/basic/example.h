#ifndef LANGUAGE_MODEL_SRC_EXAMPLE_H_
#define LANGUAGE_MODEL_SRC_EXAMPLE_H_

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
    std::vector<int> m_label_;
}
#endif // LANGUAGE_MODEL_SRC_EXAMPLE_H_