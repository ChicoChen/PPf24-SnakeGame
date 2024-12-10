#include "ga/serial_ga.h"

#include <iostream>
#include <algorithm>

SerialGA::SerialGA(int populationSize, int numSteps):
            BaseGA(populationSize, numSteps){
                #ifdef DEBUG
                std::cout << "initialize new SerialGA" << std::endl;
                #endif
            }

void SerialGA::SelectionStep() {
    #ifdef DEBUG
    std::cout << "[SerialGA] perform one step of selection" << std::endl;
    #endif

    std::sort(getPopulation().begin(), getPopulation().end(),
              [](Individual &a, Individual &b)-> bool {return a.fitness() > b.fitness();});
    currentPopulation = numSurvivor;
}