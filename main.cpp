#include <fstream>
#include <iostream>
#include "IO.cpp"
#include "Neural.cpp"
#include "generator.cpp"

const int MATRIX_WIDTH = 4;
const int MATRIX_HEIGHT = 6;

template <class T>
Lobaev::Math::Vector<T> vectorize_matrix(const Lobaev::Math::Matrix<T> &matrix);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: lab_1 <input filename>" << std::endl;
        return 1;
    }
    
    //(MATRIX_HEIGHT, MATRIX_WIDTH);
    
    std::basic_ifstream<char, std::char_traits<char>> input(argv[1]);
    Lobaev::Math::Matrix<int> input_matrix = Lobaev::IO::read_matrix<int>(input);
    input.close();
    
    if (input_matrix.rows_count() != MATRIX_HEIGHT || input_matrix.columns_count() != MATRIX_WIDTH) {
        std::cerr << "Expected input matrix size to be " << MATRIX_HEIGHT << "x" << MATRIX_WIDTH << std::endl;
        return 1;
    }
    
    std::map<std::vector<int>, int> dataset;
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 5; j++) {
            input = std::basic_ifstream<char, std::char_traits<char>>("dataset/" + std::to_string(i) + "." + std::to_string(j) + ".txt");
            auto matrix = Lobaev::IO::read_matrix<int>(input);
            Lobaev::Math::Vector<int> vector = vectorize_matrix(matrix);
            
            std::vector<int> v(vector.size());
            for (size_t k = 0; k < vector.size(); k++) {
                v[k] = vector(k);
            }
            dataset[v] = (int) i;
            
            input.close();
        }
    }
    
    const Lobaev::Math::Vector<int> input_vector = vectorize_matrix<int>(input_matrix);
    
    Lobaev::Math::Vector<double> random_weights(MATRIX_WIDTH * MATRIX_HEIGHT);
    for (size_t i = 0; i < random_weights.size(); i++) {
        random_weights(i) = ((double) rand() / (RAND_MAX));
    }
    
    const std::pair<int, double> result = predict_number(input_vector, random_weights, dataset);
    std::cout << "Predicted number: " << result.first << std::endl
    << "Probability: " << (1 - result.second) * 100 << "%" << std::endl;
    
    return 0;
}

template <class T>
Lobaev::Math::Vector<T> concat(const Lobaev::Math::Vector<T> &vector1, const Lobaev::Math::Vector<T> &vector2) {
    Lobaev::Math::Vector<T> result(vector1.size(), vector2.size());
    for (size_t i = 0; i < vector1.size(); i++) {
        result(i) = vector1(i);
    }
    for (size_t i = 0; i < vector2.size(); i++) {
        result(vector1.size() + i) = vector2(i);
    }
}

template <class T>
Lobaev::Math::Vector<T> vectorize_matrix(const Lobaev::Math::Matrix<T> &matrix) {
    Lobaev::Math::Vector<T> vector(matrix.rows_count() * matrix.columns_count());
    for (size_t i = 0; i < matrix.rows_count(); i++) {
        for (size_t j = 0; j < matrix.columns_count(); j++) {
            vector(i * matrix.columns_count() + j) = matrix(i, j);
        }
    }
    return vector;
}
