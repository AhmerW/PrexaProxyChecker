#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include "httplib.h"



std::string delimiter = ":";

// compile cmd g++ prexa.cpp -o prexa.exe -lws2_32 && prexa.exe

const char *logo = "";
const int options[10] = {99, 1, 2, 3, 4, 5, 6};
const std::string option_values[10] = {
    "exit", 
    "Start proxy checker", 
    "Load proxies", 
    "Load urls",
    "View loaded proxies",
    "View loaded urls"
    "settings"
};
const int total = sizeof(option_values)/sizeof(option_values[0]);

void createGui(){
    system("cls");
    for(int i = 0; i < 30; i++){
        std::cout << '-';
    }
    std::cout << std::endl;
    for(int i = 0; i < total; i++){
        std::cout << "[" << options[i] << "] - " << option_values[i] << std::endl; 
    }
    std::cout << std::endl;
    for(int i = 0; i < 30; i++){
        std::cout << '-';
    }
}

int getOption(){
    int result;
    std::cout << "\nPick an option: ";
    while(!(std::cin >> result)){
        std::cout << "Please pick a valid option: ";
        std::cin.clear();
        std::cin.ignore(123, '\n');
    }
    
    return result;
}

void startChecker(std::vector<std::string> proxies, std::vector<std::string> urls){
    
    const int url_size = urls.size();
    for(std::string proxy : proxies){
        std::string proxy_host;
        std::string token;
        std::string values[5];
        int proxy_port; 
        size_t pos = 0;
        int i = 0;

        while ((pos = proxy.find(delimiter)) != std::string::npos) {
            token = proxy.substr(0, pos);
            values[i] = token;
            proxy.erase(0, pos + delimiter.length());
        }
        proxy_port = atoi(values[1].c_str());
        proxy_host = values[0];
        std::cout << proxy_port << ',' << proxy_host << std::endl;

        std::random_device dev;
        
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, url_size);
        std::string rand_url = urls[dist6(rng)];

        httplib::Client cli(rand_url.c_str());
        cli.set_proxy(proxy_host.c_str(), proxy_port);
        auto res = cli.Get("/");
        if(res->status <= 200){
            std::cout << "[Success] " << proxy << std::endl;
        }

    }
}

bool loadFromFile(std::vector<std::string> &container){
    
    std::string path;
    std::cout << "File name/path ?: ";
    while(!(std::cin >> path)){
        std::cout << "Please enter a valid path" << std::endl;
    }
    std::ifstream f(path);
    if(!f){
        std::cout << "\nInvalid path provided.\n" << std::endl;
        system("pause");
        return false;
    }
    container.clear();
    int i = 0;
    std::string line;
    while(getline(f, line)){
        container.push_back(line);
    }

    std::cout << "\nSuccessfully loaded the data from file." << std::endl;
    system("pause");
    return true;
}



int main(int argc , char *argv[])
{
	
    bool connected = false;
    bool loaded_proxies = false;
    bool loaded_urls = false;
    std::vector<std::string> proxies;
    std::vector<std::string> urls;



    connected = true;
    while(connected){
        createGui();
        int option = getOption();
        switch (option)
        {
        case 99:
            connected = false;
            exit(1);
            break;
        case 2:
            if(loadFromFile(proxies)){
                loaded_proxies = true;
            }
            break;
        case 3:
            if(loadFromFile(urls)){
                loaded_urls = true;
            }
            break;
        case 1:
            std::cout << "Launching proxy checker.." << std::endl;
            if(!loaded_proxies || !loaded_urls){
                std::cout << "Proxies // Urls have not been loaded.\nPlease load the files first before starting the checker" << std::endl;
                system("pause");
                break;
            }
            startChecker(proxies, urls);
        
        default:
            break;
        }
        
    }


	return 0;
}