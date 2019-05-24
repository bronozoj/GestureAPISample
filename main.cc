#include <iostream>
#include <thread>
#include <windows.h>
#include"gestureapi.hpp"

class waitThread{
    bool shouldExit = false;
    std::thread workerThread;
    gestureapi::GestureInput client;

    void Entry(){
        INPUT leftkey, rightkey, upkey;
        leftkey.type = INPUT_KEYBOARD;
        leftkey.ki.wVk = VK_LEFT;
        leftkey.ki.wScan = 0;
        leftkey.ki.dwFlags = 0;
        leftkey.ki.time = 0;
        leftkey.ki.dwExtraInfo = 0;
        rightkey.type = INPUT_KEYBOARD;
        rightkey.ki.wVk = VK_RIGHT;
        rightkey.ki.wScan = 0;
        rightkey.ki.dwFlags = 0;
        rightkey.ki.time = 0;
        rightkey.ki.dwExtraInfo = 0;
        upkey.type = INPUT_KEYBOARD;
        upkey.ki.wVk = VK_UP;
        upkey.ki.wScan = 0;
        upkey.ki.dwFlags = 0;
        upkey.ki.time = 0;
        upkey.ki.dwExtraInfo = 0;
        if(!client.begin()){
            std::cout << "cannot connect" << std::endl;
            return;
        }
        std::cout << "connected" << std::endl;
        while(!shouldExit){
            int counter = client.input();
            if(counter == -1){
                std::cout << "internal api error" << std::endl;
                return;
            }
            if(client.isClockwise(counter) || client.isCounterClockwise(counter)){
                SendInput(1, &upkey, sizeof(INPUT));
                std::cout << "rotating" << std::endl;
            }
            else if(client.isLeft(counter)){
                SendInput(1, &leftkey, sizeof(INPUT));
                std::cout << "going left" << std::endl;
            }
            else if(client.isRight(counter)){
                SendInput(1, &rightkey, sizeof(INPUT));
                std::cout << "going right" << std::endl;
            }
            else if(counter){
                std::cout << std::hex << counter << std::dec << std::endl;
            }
        }
    }

    public:
    void Start(){
        workerThread = std::thread(&waitThread::Entry, this);
    }

    void Delete(){
        shouldExit = true;
        workerThread.join();
    }
};

int main(void){
    waitThread processor;
    std::string hi;
    processor.Start();
    std::cout << "Powerpoint clicker v0.1" << std::endl;
    std::cout << "simple keyboard mapper" << std::endl;
    std::cin >> hi;
    processor.Delete();
    //SendInput(1, &leftkey, sizeof(INPUT));
    return 0;
}