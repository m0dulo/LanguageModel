#ifndef LANGUAGE_MODEL_SRC_COMPUTION_GRAPH_H_
#define LANGUAGE_MODEL_SRC_COMPUTION_GRAPH_H_

#include "hyper_params.h"
#include "model_params.h"

struct GraphBuilder {
    DynamicLSTMBuilder encoder;
    std::vector<Node *> lookup_nodes;
    std::vector<Node *> word_nodes;

    void forward(Graph &graph, ModelParams &mode_params, HyperParams &hyper_params,
            const Feature &feature, bool is_trainning) {

        BucketNode *hidden_bucket(new BucketNode);
        hidden_bucket -> init(hyper_params.hidden_size);
        hidden_bucket -> forward(graph);

        for (const string &word : feature.sentence) {
            LookupNode *lookup_node(new LookupNode);
            lookup_node -> init(hyper_params.word_dim);
            lookup_node -> setParam(model_params.lookup_table);
            lookip_node -> forward(graph, word);
            
            DropoutNode *dropout_node(new DropoutNode(hyper_params.drop_prob, is_trainning));
            dropout_node -> init(hyper_params.word_dim);
            dropout_node -> forward(graph, *lookup_node);

            lookup_nodes.push_back(dropout_node);
        }

        for (Node *node : lookup_nodes) {
            encoder.forward(graph, model_params.lstm_params, *node, *hidden_bucket, 
                        *hidden_bucket, hyper_params.drop_prob, is_trainning);
        }

        for (Node *node : encoder._hiddens) {
            UniNode *uni_node(new UniNode);
            uni_node -> init(hyper_params.word_dim);
            uni_node -> setParam(*model_params.linear);

            LinearWordVectorNode *word_node(new LinearWordVectorNode);
            word_node -> init(model_params.lookup_table.nVSize);
            word_node -> setParam(model_params.lookup_table.E);
            word_node -> forward(graph, *word_node);

            word_nodes.push_back(word_node);
        }

    }
};

#endif // LANGUAGE_MODEL_SRC_COMPUTION_GRAPH_H_