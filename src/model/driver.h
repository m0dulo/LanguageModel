#ifndef LANGUAGE_MODEL_SRC_DRIVER_H_
#define LANGUAGE_MODEL_SRC_DRIVER_H_

#include <iostream>
#include <math.h>
#include <tuple>
#include "compution_graph.h"

class Driver {
public:
    ModelParams model_params_;
    HyperParams hyper_params_;
    // Metric eval_;
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

    std::tuple<dtype, double, dtype> train(Graph &graph, const std::vector<Example> &examples, int iter) {
        dtype cost = 0.0;
        int correct_num = 0;
        int total_num = 0;
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
            correct_num += getCorrectNum(examples.at(i).m_words_id_, loss.second);
            total_num += examples.at(i).m_words_id_.size();
            cost += loss.first;
        }
        graph.backward();
        double accuracy = correct_num * 1.0 / total_num;
        dtype perplexity = exp(cost);
        
        return std::make_tuple(cost, accuracy, perplexity);
    }

    std::tuple<dtype, double, dtype>predict(Graph &graph, const Feature &feature, std::vector<int> &answer) { 
        std::vector<Node *> word_nodes;
        GraphBuilder builder;
        builder.forward(graph, model_params_, hyper_params_, feature, false, word_nodes);
        graph.compute();
        std::pair<dtype, std::vector<int>> loss = MaxLogProbabilityLoss(word_nodes, answer, 1);
        int correct_num = getCorrectNum(answer, loss.second);
        int total_num = word_nodes.size();
        dtype cost = loss.first;
        double accuracy = correct_num * 1.0 / total_num;
        dtype perplexity = exp(loss.first);
        return std::make_tuple(cost, accuracy, perplexity);
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
    // void resetEval() {
    //     eval_.reset();
    // }

    int getCorrectNum(std::vector<int> &answer, std::vector<int> &predict) {
        int correct_num = 0;
        for (int i = 0; i < predict.size(); i++) {
            if (predict.at(i) == answer.at(i))
                correct_num += 1;
        }
        return correct_num;
    }

    void setUpdateParameters(dtype nnRegular, dtype adaAlpha, dtype adaEps) {
        ada_._alpha = adaAlpha;
        ada_._eps = adaEps;
        ada_._reg = nnRegular;
    }

};

#endif // LANGUAGE_MODEL_SRC_DRIVER_H_