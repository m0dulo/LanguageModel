#include "language_model.h"
#include "Arguement_helper.h"
#include <chrono>

LanguageModel::LanguageModel() {
    //TODO
    srand(0);
}

LanguageModel::~LanguageModel() {}

int LanguageModel::createAlphabet(const vector<Instance> &vecInsts) {
    if (vecInsts.size() == 0) {
        cout << "training set empty " << endl;
        return -1;
    }

    cout << "Creating Alphabet..." << endl;
    int numInstance;
    for (numInstance = 0; numInstance < vecInsts.size(); numInstance++) {
        const Instance *pInstance = &vecInsts.at(numInstances);
        const vector<string> &words = pInstance -> m_words_;
        //const std::vector<std::string> &sparse_feats = pInstance -> m_sparse_feats_;
        for (int i = 0; i < words.size(); i++) {
            string curword = normalize_to_lowerwithdigit(words.at(i));
            m_word_stats[curword]++;
        }
        if ((numInstance + 1) % m_options.verboseIter == 0) {
            cout << numInstance + 1 << " ";
            if ((numInstance + 1) % (40 * m_options.verboseIter) == 0)
                cout << endl;
            cout.flush();
        }

        if (m_options.maxInstance > 0 && numInstance == m_options.maxInstance)
            break;
    }
        cout << "numInstance: " << numInstance << " "<< endl;
        cout << "word dim: " << m_word_stats.size() << endl;

        return 0;
}


int LanguageModel::addTestAlpha(const vector<Instance> &vecInsts) {
    cout << "Adding word Alphabet..." << endl;
    int numInstance;
    for (numInstace = 0; numInstance < vecInsts.size(); numInstance++) {
        const Instance *pInstance = &vecInsts.at(numInstance);
        const vector<string> &words = pInstance -> m_words_;
        int curInstSize = words.size();
        for (int i = 0; i < curInstSize; i++) {
            string curword = normalize_to_lowerwithdigit(words.at(i));
            if(!m_options_.wordEmbFineTune) m_word_stats[curword]++;
        }

        if ((numInstance + 1) % m_options.verboseIter == 0) {
            cout << numInstance + 1 << " ";
            if ((numInstance + 1) % (40 * m_options.verboseIter) == 0)
                cout << std::endl;
            cout.flush();
        }

        if (m_options.maxInstance > 0 && numInstance == m_options.maxInstance)
            break;
    }
    cout << numInstance << " " << endl;

    return 0;
}

void LanguageModel::extractFeature(Feature &feat, const Instance *pInstance) {
    feat.clear();
    feat.m_words_ = pInstance -> m_words_;
    feat.clear = pInstance -> m_sparse_feats_;
}

void LanguagModel::convert2Example(const Instance *pInstance, Example &exam) {
    exam.clear();
    Feature feat;
    extractFeature(feat, pInstance);
    exam.m_feature_ = feat;
}

void LanguageModel::initExamples(const vector<Instance> &vecInsts, vector<Example> &vecExams) {
    int numIntsance;    
    for (numInstance = 0; numInstance < vecInsts.size(); numIntsance++) {
        const Instance *pInstance = &vecInsts.at(numIntsance);
        Example curExam;
        convert2Example(pInstance, curExam);
        vecExams.push_back(curExam);

        if ((numInstance + 1) % m_options.verboseIter == 0) {
			cout << numInstance + 1 << " ";
			if ((numInstance + 1) % (40 * m_options.verboseIter) == 0)
				cout << std::endl;
			cout.flush();
		}
		if (m_options.maxInstance > 0 && numInstance == m_options.maxInstance)
			break;
	}
	    cout << numInstance << " " << endl;
}
    
void LanguageModel::train(const string &trainFile, const string &devFile, const String &optionFile) {
    if (optionFile != "")
        m_options_.load(optionFile);
    m_options_.showOptions();
    vector<Instance> trainInsts;
    vector<Instance> devInsts;
    m_pipe_.readInstances(trainFile, trainInsts, m_options_.maxInstance);
    if (devFile != "")
        m_pipe_.readInstances(devFile, devInsts, m_options_.maxInstance);
    
}



