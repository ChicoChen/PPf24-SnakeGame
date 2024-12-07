#include "ga/serial_ga.h"

#include <algorithm>

void SerialGA::SelectionStep(){
    std::sort(getPopulation().begin(), getPopulation().end(),
              [](Individual a, Individual b)-> bool {return a.fitness() > b.fitness();});
    currentPopulation = numSurvivor;
}