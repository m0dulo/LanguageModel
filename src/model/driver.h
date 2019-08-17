#ifndef LANGUAGE_MODEL_SRC_DRIVER_H_
#define LANGUAGE_MODEL_SRC_DRIVER_H_

#include <iostream>
#include <math.h>
#include "example.h"
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
        std::vector<Node *> word_nodes {nullptr};
        word_nodes.clear();
        for (auto example : examples){
            GraphBuilder builder;
            builder.forward(graph, model_params_, hyper_params_, example.m_feature_, true, word_nodes);
        }
        graph.compute();
        for (auto example : examples) {
            example.m_words_id_.clear();
            std::pair<dtype, std::vector<int>> loss= MaxLogProbabilityLoss(word_nodes, example.m_words_id_, examples.size());
            cost += loss.first;
        }
        graph.backward();
        return exp(cost);
    }

    dtype accuracy()


    
};

#endif // LANGUAGE_MODEL_SRC_DRIVER_H_