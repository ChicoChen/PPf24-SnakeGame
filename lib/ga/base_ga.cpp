#include "ga/base_ga.h"

#include <algorithm>
#include <cassert>

// TODO: build an env file to handle hyperparameters
BaseGA::BaseGA(std::string exp_name, int population_size, int num_steps, int thread_num)
    : exp_name(exp_name),
      population(population_size),
      population_size(population_size),
      current_population(population_size),
      num_steps(num_steps),
      num_survivor(population_size * SURVIVAL_RATE),
      thread_num(thread_num),
      logger(exp_name + ".log", num_steps) {
    // construct one generator for each thread. default is serial
    std::random_device rd;
    for (int i = 0; i < thread_num; i++) {
        generators.emplace_back(rd());
    }
}

void BaseGA::execute(int print_interval) {
    logger.log_start();

    for (int i = 0; i < num_steps; i++) {
        evaluate_fitness();

        int best_idx = 0;  // best individual with highest fitness
        double best_fitness = 0, avg_fitness = 0;
        int best_score = 0;
        double avg_score = 0;

        for (int j = 0; j < population_size; j++) {
            if (population[j].fitness > best_fitness) {
                best_fitness = population[j].fitness;
                best_idx = j;
            }
            avg_fitness += population[j].fitness;
            avg_score += population[j].score;
            best_score = std::max(best_score, population[j].score);
        }

        avg_fitness /= population_size;
        avg_score /= population_size;

        logger.log_iteration(best_fitness, avg_fitness, best_score, avg_score);

        // save the best individual
        population[best_idx].save(exp_name);

#ifdef DEBUG
        if (i % print_interval == 0) {
            logger.print_iteration_summary();
        }
#endif
        selection_step();
        update_population();
    }

    logger.print_iteration_summary();
    logger.log_finish();

    logger.export_log();
}

void BaseGA::update_population() {
    double sum = 0;
    for (int i = 0; i < num_survivor; i++) {
        sum += population[i].fitness;
    }

    // shuffle survivors to randomize parent slection
    std::shuffle(population.begin(), population.begin() + num_survivor, generators[0].gen);
    while (current_population < population_size) {
        std::vector<int> par = select_parents(sum, generators[0].gen);
        Individual &father = population[par[0]];
        Individual &mother = population[par[1]];

        for (auto &child : father.crossover(generators[0].gen, mother)) {
            child.mutate(generators[0].gen);
            population[current_population++] = std::move(child);
            // edge condition: only need to add one child
            if (current_population == population_size) break;
        }
    }
}

std::vector<int> BaseGA::select_parents(double sum, std::mt19937 &generator) {
    assert(num_survivor >= 2 && sum >= 0);

    static int idx;
    std::vector<int> parents;
    std::uniform_real_distribution<> dis(0.0, sum);

    for (int i = 0; i < 2; i++) {
        double threshold = dis(generator);
        double count = 0;
        for (int candidate = 0; candidate < num_survivor; candidate++) {
            count += population[candidate].fitness;
            if (count >= threshold) {
                parents.push_back(candidate);
                break;
            }
        }
    }

    return parents;
}