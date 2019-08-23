#ifndef LANGUAGE_MODEL_H_
#define LANGUAGE_MODEL_H_


#include "driver.h"
#include "options.h"
#include "instance.h"
#include "pipe.h"
#include "utf.h"

using namespace nr;
using namespace std;

class LanguageModel {


  public:
    unordered_map<string, int> m_word_stats;
    // unordered_map<string, int> m_feat_stats;

  public:
    Options m_options_;

    Driver m_driver_;

    Pipe m_pipe_;


  public:
    LanguageModel(int memsize);
    virtual ~LanguageModel() = default;

  public:

    int createAlphabet(const vector<Instance>& vecTrainInsts);
    int addTestAlpha(const vector<Instance>& vecInsts);

    void extractFeature(Feature& feat, const Instance* pInstance);

    void convert2Example(const Instance* pInstance, Example& exam);
    void initialExamples(const vector<Instance>& vecInsts, vector<Example>& vecExams);

  public:
    void train(const string& trainFile, const string& devFile, const string& testFile, const string& modelFile, const string& optionFile);
    int predict(const Feature& feature, string& output);
    void test(const string& testFile, const string& outputFile, const string& modelFile);

    void writeModelFile(const string& outputModelFile);
    void loadModelFile(const string& inputModelFile);

};

#endif // LANGUAGE_MODEL_H_