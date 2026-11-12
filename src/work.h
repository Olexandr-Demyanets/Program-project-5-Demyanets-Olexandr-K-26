#pragma once

#include <chrono>
#include <string>

void compute(const std::string& name, std::chrono::seconds duration);

void slow(const std::string& name);

void quick(const std::string& name);

void work();
