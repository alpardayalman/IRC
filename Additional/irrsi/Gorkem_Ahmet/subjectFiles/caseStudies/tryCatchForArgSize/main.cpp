
#include <iostream>
#include <vector>


int main()
{
    std::vector<std::string> TEST;
    
    TEST.push_back("MODE");
    TEST.push_back("#channel");
    TEST.push_back("+k");
    TEST.push_back("444");
    try
    {
        std::cout << TEST.at(5) << std::endl;
    }
    catch (const std::out_of_range &e)
    {
       std::cerr << "Hata: " << e.what() << std::endl;
    }
    return 0;
}
