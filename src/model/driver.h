#ifndef LANGUAGE_MODEL_SRC_DRIVER_H_
#define LANGUAGE_MODEL_SRC_DRIVER_H_

#include <iostream>
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
        model_params_.exportModelParams(_ada);
        setUpdateParameters(hyper_params_.nn_regular, hyper_params_.ada_alpha, hyper_params_.ada_eps);
    }

    dtype train(Graph &graph, const std::vector<Example> &examples, int iter) {
        dtype cost = 0.0;
        int example_num = examples.size();
        std::vector<Node *> &word_nodes
        for (int count = 0; cout < example_num; count++) {
            const Example &example = examples.at(count);
            GraphBuilder builder;
            builder.forward(graph, model_params_, hyper_params_, example.m_feature, true, word_nodes);
            
        }
    }
    
}
#endif // LANGUAGE_MODEL_SRC_DRIVER_H_