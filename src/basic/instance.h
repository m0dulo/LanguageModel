#ifndef BASIC_INSTANCE_H_
#define BASIC_INSTANCE_H_

#include <vector>
#include <string>

class Instance {
public:
    std::vector<std::string> m_words_;
    std::vector<int> m_words_id_;
    std::vector<std::string> m_sparse_feats_;

    int size() const {
        return m_words_.size();
    } 

    void clear() {
        m_words_.clear();
        m_words_id_.clear();
        m_sparse_feats_.clear();
    }

    void allocate(int length) {
		clear();
		m_words_.resize(length);
	}
    
    void copyValuesFrom(const Instance& anInstance) {
		allocate(anInstance.size());
		//m_label_ = anInstance.m_label_;
		m_words_id_ = anInstance.m_words_id_;
		//m_chars_ = anInstance.m_chars_;
		m_sparse_feats_ = anInstance.m_sparse_feats_;
	}
};
#endif // BASIC_INSTANCE_H_