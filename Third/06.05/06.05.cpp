#include <iostream>
#include <string>
#include <functional>
#include <boost/dll.hpp>

int main()
{
    std::cout << "Enter library name (or 'quit' to exit):\n";
    std::string libName;
    while (std::getline(std::cin, libName) && libName != "quit")
    {
        try
        {
            std::function<void()> func = boost::dll::import_symbol<void()>(
                libName, "test",
                boost::dll::load_mode::default_mode);
            func();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
        std::cout << "\nEnter library name (or 'quit' to exit):\n";
    }
}
