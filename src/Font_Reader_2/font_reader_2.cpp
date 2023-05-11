#include "font_reader.h"

int main(int argc, char** argv)
{
    std::vector<std::string> args;

	for (int i = 0; i < argc; i++) {
		args.push_back(argv[i]);
	}
    
    std::map<std::string,bool> arg_true_false = {
        std::pair<std::string,bool>("0",false),
        std::pair<std::string,bool>("1",true),

        std::pair<std::string,bool>("false",false),
        std::pair<std::string,bool>("true",true),

        std::pair<std::string,bool>("False",false),
        std::pair<std::string,bool>("True",true),
    }; 
    std::cout << "args: " << args[1] << " " << args[2] << " " << args[3] << " " << args[4] << std::endl;
    std::cout << storeCharsInfoInJSON(args[1].c_str(), args[2].c_str(),arg_true_false[args[3]],stringToInt(args[4])) << std::endl;
}