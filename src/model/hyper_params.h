#ifndef LANGUAGE_MODEL_SRC_COMPUTION_HYPER_PARAMS_H_
#define LANGUAGE_MODEL_SRC_COMPUTION_HYPER_PARAMS_H_

#include "N3LDG.h"
#include "options.h"
#include "example.h"

struct HyperParams {
private:
    bool bAssigned;
    
public:
    int batch_size;

    dtype nn_regular;
    dtype ada_alpha;
    dtype ada_eps;

    int hidden_size;
    int word_context;
    int word_window;
    int window_output;
    dtype drop_prob;

    int word_dim;
    int label_size;
    int input_size;
    
    HyperParams() {
        bAssigned = false;
        batch_size = 1;
    }

    void setParams(Options &op) {
        nn_regular = op.regParameter;
        ada_alpha = op.adaAlpha;
        ada_eps = op.adaEps;
        hidden_size = op.hiddenSize;
        word_context = op.wordcontext;
        drop_prob = op.dropProb;
        batch_size = op.batchSize;

        bAssigned = true;
    }

    void clear() {
        bAssigned = false;
    }

    bool bValid() {
        return bAssigned;
    }
};
#endif // LANGUAGE_MODEL_SRC_COMPUTION_HYPER_PARAMS_H_