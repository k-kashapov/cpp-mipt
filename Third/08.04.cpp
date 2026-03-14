#include <iostream>
#include <random>
#include <string>
#include <vector>

const std::string TARGET = "methinksitislikeaweasel";
const size_t LENGTH = 23;
const size_t POPULATION_SIZE = 100;
const double MUTATION_PROB = 0.05;

char random_letter(std::default_random_engine &engine) {
    static std::uniform_int_distribution<int> dist(0, 25);
    return 'a' + dist(engine);
}

char random_different_letter(char current, std::default_random_engine &engine) {
    char new_char;
    do {
        new_char = random_letter(engine);
    } while (new_char == current);
    return new_char;
}

size_t hamming_distance(const std::string &a, const std::string &b) {
    size_t dist = 0;
    for (size_t i = 0; i < LENGTH; ++i) {
        if (a[i] != b[i]) {
            ++dist;
        }
    }
    return dist;
}

int main() {
    std::random_device rd;
    std::default_random_engine engine(rd());

    std::string current;
    for (size_t i = 0; i < LENGTH; ++i) {
        current += random_letter(engine);
    }

    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    size_t generation = 0;
    std::cout << "Generation " << generation << ": " << current << std::endl;

    size_t last_best = std::numeric_limits<size_t>::max();

    while (true) {
        std::string best_offspring;
        size_t best_distance = std::numeric_limits<size_t>::max();

        for (size_t i = 0; i < POPULATION_SIZE; ++i) {
            std::string child = current;
            for (size_t j = 0; j < LENGTH; ++j) {
                if (prob_dist(engine) < MUTATION_PROB) {
                    child[j] = random_different_letter(child[j], engine);
                }
            }
            size_t distance = hamming_distance(child, TARGET);
            if (distance < best_distance) {
                best_distance = distance;
                best_offspring = child;
            }
        }

        if (best_distance == 0) {
            std::cout << "Generation " << generation + 1 << ": " << best_offspring
                      << std::endl;
            break;
        }
        if (best_distance < last_best) {
            current = best_offspring;
            last_best = best_distance;
        }

        ++generation;
        std::cout << "Generation " << generation << ": " << current << std::endl;
    }

    return 0;
}
