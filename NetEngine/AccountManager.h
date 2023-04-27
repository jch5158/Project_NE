#pragma once

class AccountManager
{
private:

    USE_LOCK;

public:

    void AccountThenPlayer();
    void Lock();
};

extern AccountManager GAccountManager;

