#ifndef BASIC_EXAMPLE_H_
#define BASIC_EXAMPLE_H_

#include <iostream>
#include <vector>
#include <string>

class Feature {
public:
    std::vector<std::string> m_words_;
    std::vector<std::string> m_sparse_feats;

    void clear() {
        m_words_.clear();
        m_sparse_feats.clear();
    }
};

class Example {
public:
    Feature m_feature_;
    std::vector<int> m_words_id_;

    void clear() {
        m_feature.clear();
        m_words_id.clear();
    }
};

#endif // BASIC_EXAMPLE_H_