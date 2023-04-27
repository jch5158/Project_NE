#include "pch.h"
#include <iostream>
#include <atomic>
#include <future>
#include <Windows.h>
#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"

int main()
{ 
    GThreadManager->Launch([]()
        {
            while (true)
            {
                std::cout << "PlayerThenAccount\n";
                GPlayerManager.PlayerThenAccount();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });


    GThreadManager->Launch([]()
        {
            while (true)
            {
                std::cout << "AccountThenPlayer\n";
                GAccountManager.AccountThenPlayer();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });

    GThreadManager->Join();

    return 0;
} 