#include <map>
#include <vector>
#include "Matrix.cpp"

std::pair<int, double> predict_number(const Lobaev::Math::Vector<int> &input_vector,
                                      const Lobaev::Math::Vector<double> &weights,
                                      const std::map<std::vector<int>, int> &dataset) {
    double best_err = INT_MAX;
    int result = -1;
    for (const auto &d : dataset) {
        double err = 0;
        for (size_t i = 0; i < d.first.size(); i++) {
            double val = d.first[i] == input_vector(i) ? -1 : 1;
            err += val / (double) input_vector.size() * weights(i);
        }
        
        std::cout << "Integer " << d.second << " has loss " << err + 0.5 << std::endl;
        
        if (best_err > err) {
            best_err = err;
            result = d.second;
        }
    }
    return std::make_pair(result, best_err + 0.5);
}
