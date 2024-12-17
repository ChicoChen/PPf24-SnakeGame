#include "ga/logger.h"

#include <fstream>
#include <iostream>

Logger::Logger(const std::string& filename, int num_steps) : filename(filename) {
    best_fitness.reserve(num_steps);
    avg_fitness.reserve(num_steps);
    best_score.reserve(num_steps);
    avg_score.reserve(num_steps);
}

void Logger::log_start() { start_time = chrono_clock::now(); }
void Logger::log_finish() {
    total_time = chrono_clock::now() - start_time;
    std::cout << "[Logger] Total time elapsed: " << total_time.count() << "s" << std::endl;
}

void Logger::log_iteration(double best_fitness, double avg_fitness, double med_fitness,
                           int best_score, double avg_score, int med_score, double step_time) {
    this->best_fitness.push_back(best_fitness);
    this->avg_fitness.push_back(avg_fitness);
    this->med_fitness.push_back(med_fitness);

    this->best_score.push_back(best_score);
    this->avg_score.push_back(avg_score);
    this->med_score.push_back(med_score);

    this->step_times.push_back(step_time);
}

void Logger::print_iteration_summary() {
    std::cout << "[Logger] Iteration " << best_fitness.size() - 1 << ": " << step_times.back() << "s" << std::endl;
    std::cout << "[Logger] Fitness: Best, Average, Median : "
              << best_fitness.back() << ", " << avg_fitness.back() << ", " << med_fitness.back() << std::endl;
    std::cout << "[Logger] Score: Best, Average, Median : "
              << best_score.back() << ", " << avg_score.back() << ", " << med_score.back() << std::endl;
}

void Logger::export_log() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Logger] Failed to open file " << filename << std::endl;
        return;
    }
    
    file << total_time.count() << std::endl;
    for (int i = 0; i < best_fitness.size(); i++) {
        file << i << ',' << best_fitness[i] << ',' << avg_fitness[i] << ',' << med_fitness[i] << ','
                         << best_score[i] << ',' << avg_score[i] << ',' << med_score[i]  << ',' << step_times[i] << std::endl;
    }
    std::cout << "[Logger] Exported log file " << filename << std::endl;
}
