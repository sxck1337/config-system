# C++ Config System

This is a simple one header-only C++ Config System library that provides a way to save and load configuration settings from a file.

## Usage

To use this library, simply include the "config.hpp" header file and create a Config object. Then, you can use the `Set()` and `Get()` member functions to store and retrieve configuration values, respectively. For example:

```c++

#include "config.hpp"

int main()
{
    Config config;

    config.Set("window.title", "My App");
    config.Set("window.width", 800);
    config.Set("window.height", 600);

    config.SaveToFile("config.txt");

    Config loadedConfig;
    loadedConfig.LoadFromFile("config.txt");
    std::string title = loadedConfig.Get<std::string>("window.title");
    int width = loadedConfig.Get<int>("window.width");
    int height = loadedConfig.Get<int>("window.height");
}

```

## Subcategories

You can also use subcategories to organize your configuration values. To create a subcategory, simply call the `GetSubcategory()` member function with the name of the subcategory. Then, you can use the `Set()` and `Get()` member functions on the subcategory object to store and retrieve configuration values within that subcategory, respectively. For example:

```c++

Config config;

auto window = config.GetSubcategory("window");
window.Set("title", "My App");
window.Set("width", 800);
window.Set("height", 600);

config.SaveToFile("config.txt");

Config loadedConfig;
loadedConfig.LoadFromFile("config.txt");
auto loadedWindow = loadedConfig.GetSubcategory("window");
std::string title = loadedWindow.Get<std::string>("title");
int width = loadedWindow.Get<int>("width");
int height = loadedWindow.Get<int>("height");

```

## Supported Types

This library supports the following types for configuration values: `bool`, `int`, `float`, `double`, `std::string`, `char`, and `unsigned long long int`. Any other types will result in a compile-time error.

## Contributing

Improvements and suggestions are always welcome! If you find a bug or have an idea for a new feature, please open an issue or submit a pull request.

