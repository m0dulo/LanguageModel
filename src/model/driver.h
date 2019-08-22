#ifndef LANGUAGE_MODEL_SRC_DRIVER_H_
#define LANGUAGE_MODEL_SRC_DRIVER_H_

#include <iostream>
#include <math.h>
#include "compution_graph.h"

class Driver {
public:
    ModelParams model_params_;
    HyperParams hyper_params_;
    Metric eval_;
    ModelUpdate ada_;

    void init() {
        if (!hyper_params_.bValid()) {
            std::cout << "hyper parameter initialization Error, Please check!" << std::endl;
            return;
        }
        if (!model_params_.init(hyper_params_)){
            std::cout << "model parameter initialization Error, Please check!" << std::endl;
			return;
        }
        model_params_.exportModelParams(ada_);
        setUpdateParameters(hyper_params_.nn_regular, hyper_params_.ada_alpha, hyper_params_.ada_eps);
    }

    dtype train(Graph &graph, const std::vector<Example> &examples, int iter) {
        dtype cost = 0.0;
        std::vector<std::vector<Node *>> batch_word_nodes;
        for (auto example : examples){
            std::vector<Node *> word_nodes;
            word_nodes.clear();
            GraphBuilder builder;
            builder.forward(graph, model_params_, hyper_params_, example.m_feature_, true, word_nodes);
            batch_word_nodes.push_back(word_nodes);
        }
        graph.compute();
        for (int i = 0; i < examples.size(); i++) {
            std::pair<dtype, std::vector<int>> loss = MaxLogProbabilityLoss(batch_word_nodes.at(i), examples.at(i).m_words_id_, examples.size());
            cost += loss.first;
        }
        graph.backward();
        dtype perplexity = exp(cost);
        return perplexity;
    }

    dtype predict(Graph &graph, const Feature &feature) {
        std::vector<Node *> word_nodes;
        GraphBuilder builder;
        builder.forward(graph, model_params_, hyper_params_, feature, true, word_nodes);
        graph.compute();
        std::pair<dtype, std::vector<int>> loss = MaxLogProbabilityLoss(word_nodes, instance.m_words_id_, 1);
        dtype perplexity = exp(loss.first);
        return perplexity;
    }

    void updateModel() {
        ada_.updateAdam(10);
    }

    void checkgrad(const std::vector<Example> &examples, int iter) {
        ostringstream out;
        out << "Interation: " << iter;
        //TODO
    }

private:
    void resetEval() {
        eval_.reset();
    }

    void setUpdateParameters(dtype nnRegular, dtype adaAlpha, dtype adaEps) {
        ada_._alpha = adaAlpha;
        ada_._eps = adaEps;
        ada_._reg = nnRegular;
    }

};

#endif // LANGUAGE_MODEL_SRC_DRIVER_H_