#ifndef LANGUAGE_MODEL_SRC_COMPUTION_MODEL_PARAMS_H_
#define LANGUAGE_MODEL_SRC_COMPUTION_MODEL_PARAMS_H_

#include "hyper_params.h"

struct ModelParams {
    Alphabet word_alpha;
    LookupTable lookup_table;
    LSTM1Params lstm_params;
    UniParams linear;

    bool init(HyperParams &op) {
        if(lookup_table.nVSize <= 0) {
            return false;
        }
        op.word_dim = lookup_table.nDim;
        lstm_params.init(op.hidden_size, op.word_dim);
        linear.init(op.word_dim, op.hidden_size, false);

        return true;
    }

    void exportModelParams(ModelUpdate &ada) {
        lookup_table.exportAdaParams(ada);
        lstm_params.exportAdaParams(ada);
        linear.exportAdaParams(ada);
    }

    Json::Value toJson() const override {
        Json::Value json;
        json["word_alpha"] = word_alpha.toJson();
        json["lookup_table"] = lookup_table.toJson();
        json["lstm_params"] = lstm_params.toJson();
        json["linear"] = linear.toJson();
        return json;
    }

    void fromJson(const Json::Value &json) override {
        word_alpha.fromJson(json["word_alaph"]);
        lookup_table.fromJson(json["lookup_table"]);
        lstm_params.fromJson(json["lstm_params"]);
        linear.fromJson(json["linear"]);
    }
};

#endif //  LANGUAGE_MODEL_SRC_COMPUTION_MODEL_PARAMS_H_