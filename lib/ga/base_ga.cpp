#include "ga/base_ga.h"

#include <algorithm>
#include <cassert>
#include <chrono>

using chrono_clock = std::chrono::high_resolution_clock;

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

    chrono_clock::time_point starting_point;
    std::chrono::duration<double> step_total_time;
    for (int i = 0; i < num_steps; i++) {
        starting_point = chrono_clock::now();
        evaluate_fitness();

        double best_fitness = 0, avg_fitness = 0;
        int best_score = 0;
        double avg_score = 0;

        for (int j = 0; j < population_size; j++) {
            best_fitness = std::max(best_fitness, population[j].fitness);
            avg_fitness += population[j].fitness;
            avg_score += population[j].score;
            best_score = std::max(best_score, population[j].score);
        }
        avg_fitness /= population_size;
        avg_score /= population_size;

        selection_step();
        // save the best individual
        if(i == num_steps - 1)population[0].save(exp_name);

        double median_fitness = population[population_size / 2].fitness;
        int median_score = population[population_size / 2].score;

        update_population();

        step_total_time = chrono_clock::now() - starting_point;
        logger.log_iteration(best_fitness, avg_fitness, median_fitness, best_score, avg_score, median_score, step_total_time.count());
#ifdef DEBUG
        if (i % print_interval == 0) {
            logger.print_iteration_summary();
        }
#endif
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