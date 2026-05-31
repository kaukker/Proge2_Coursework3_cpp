#pragma once
#include <stop_token>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <sstream>

#include "Item.h"

using namespace std;

class Control_A
{
public:
    stop_token stop;
    mutex mx;
    condition_variable_any cva;
    queue<Item*> Items;
};

class Control_GUI
{
public:
    mutex mx;
    condition_variable cv;
    stringstream buffer;
    bool stop = false;
};
