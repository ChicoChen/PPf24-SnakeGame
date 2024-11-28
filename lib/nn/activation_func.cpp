#include "nn/activation_func.h"

#include <array>
#include <cmath>

constexpr std::array<const char*, 3> activation_names = {
    "sigmoid",
    "relu",
    "tanh"
};

Activation from_string(const std::string& str) {
    for (int i = 0; i < 3; i++) {
        if (str == activation_names[i]) {
            return static_cast<Activation>(i);
        }
    }
    return Activation::SIGMOID;
}

std::string to_string(Activation activation) {
    return activation_names[static_cast<int>(activation)];
}

float(*get_func(Activation activation))(float) {
    switch (activation) {
    case Activation::SIGMOID:
        return sigmoid;
    case Activation::RELU:
        return relu;
    case Activation::TANH:
        return std::tanh;
    }
    return sigmoid;
}

inline float sigmoid(float x) {
    return 1 / (1 + exp(-x));
}

inline float relu(float x) {
    return x > 0 ? x : 0;
}