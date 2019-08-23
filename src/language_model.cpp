#include "language_model.h"
#include "Arguement_helper.h"
#include <chrono>

LanguageModel::LanguageModel () {
    //TODO
    srand(0);
}

LanguageModel::~LanguageModel () {}

int LanguageModel::createAlphabet(const std::vector<Instance> &vecInsts) {
    if (vecInsts.size() == 0) {
        std::cout << "training set empty " << std::endl;
        return -1;
    }

    std::cout << "Creating Alphabet..." << std::endl;

    for (int instance_num = 0; instance_num < vecInsts.size(); instance_num++) {
        const Instance *pInstance = &vecInsts.at(instance_num);
        const std::vector<std::string> &words = pInstance -> m_words_;
        const std::vector<std::string> &sparse_feats = pInstance -> m_sparse_feats_;
        const std::vector<int> &word_ids = pInstance -> m_words_id_；


    }
}