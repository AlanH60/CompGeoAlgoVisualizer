#pragma once
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <memory>
#include <chrono>
#include <thread>

#include <string>
#include <vector>
#include <queue>
#include <bitset>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#define NOMINMAX
#include <Windows.h>

#define EPSILONF 0.05f

#define EQUALF(f1, f2) (abs(f1 - f2) < EPSILONF)
#define LESSF(f1, f2) (f1 < f2 && f2 - f1 > EPSILONF)
#define GREATERF(f1, f2) (f1 > f2 && f1 - f2 > EPSILONF)