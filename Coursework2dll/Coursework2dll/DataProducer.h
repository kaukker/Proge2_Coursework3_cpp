#pragma once

class Item;

using InitializeFunc = void (*)();
using GetItemFunc = Item* (*)();
using GetTestItemFunc = Item* (*)(int);
using StartFunc = void (*)(char, void*);
