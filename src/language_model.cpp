#include "language_model.h"
#include "Arguement_helper.h"
#include <chrono>
#include <algorithm>

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
    // feat.m_sparse_feats = pInstance -> m_sparse_feats_;
}

void LanguagModel::convert2Example(const Instance *pInstance, Example &exam) {
    exam.clear();
    exam.m_words_id_ = pInstance -> m_words_id_;
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
    vector<Instance> train_instances;
    vector<Instance> dev_instances;
    m_pipe_.readInstances(trainFile, train_instances, m_options_.maxInstance);
    if (devFile != "")
        m_pipe_.readInstances(devFile, dev_instannces, m_options_.maxInstance);
    createAlphabet(train_instances);
    addTestAlpha(dev_instances);
    m_word_stats[unknownkey] = m_options_.wordCutOff + 1;

    m_driver_.model_params_.word_alpha.init(m_word_stats, m_options_.wordCutOff);
    for (auto &instance : train_insatnces) {
        vector<int> ids;
        ids.push_back(m_driver_.model_params_.word_alpha.from_string(instance));
        instance.m_words_id_ = ids;
    }

    for (auto &instance : dev_instances) {
        vector<int> ids;
        ids.push_back(m_driver_.model_params_.word_alpha.from_string(instance));
        instance.m_words_id_ = ids;
    }

    if (m_options_.wordFile != "") {
        m_driver_.model_params_.lookup_table.init(m_driver_.model_params_.word_alpha, m_options_.wordFile, m_options_.wordEmbFineTune);
    } else {
        m_driver_.model_params_.lookup_table.init(m_driver_.model_params_.word_alpha, m_options_.wordEmbSize, m_options_.wordEmbFineTune);
    }
    vector<Example> train_examples;
    vector<Example> dev_examples;
    initExamples(train_instances, train_examples);
    initExamples(dev_instances, dev_examples);

    m_driver_.hyper_params_.setParams(m_options_);
    m_driver_.init();

    int input_size = train_examples.size();

    int batch_block = input_size / m_options_.batchSize;
    if (input_size % m_options_.batchSize != 0)
        batch_block++;
    
    vector<int> indexes;
    for (int i =0; i < input_size; i++) 
        indexes.push_back(i);

    static vector<Example> sub_examples;
    
    int dev_nums = dev_examples.size();

    for (int iter = 0; iter < m_options_.maxIter; ++iter) {
        cout << "###### Epcho " << iter << endl;

        random_shuffle(indexes.begin(), indexes.end());

        auto time_start = chrono::high_resolution_clock::now();

        for (int updateIter = 0; updateIter < batch_block; updateIter++) {  
            Graph graph;
            sub_examples.clear();
            int start_pos = updateIter * m_options_.batchSize;
            int end_pos = (updateIter + 1) * m_options_.batchSize;
            if (end_pos > input_size)
                end_pos = input_size;
            
            for (int idy = start_pos; idy < end_pos; idy++) {
                sub_examples.push_back(train_examples.at(indexes[idy]));
            }

            int curUpdateIter = iter * batch_block + updateIter;

            auto train_result = m_driver_.train(graph, sub_examples, curUpdateIter);

            if ((curUpdateIter + 1) % m_options_.verboseIter == 0) {
                cout << "current: " << updateIter + 1 << ", total block: " << batch_block << endl;
                cout << "loss = " << std::get<0>(train_result)
                     << "accuracy = " << std::get<1>(train_result)
                     << "perplexity = " << std::get<2>(train_result) << endl;
            } 
        }
    }
}



