#pragma once

class PlayerManager
{
private:

    USE_LOCK;

public:

    PlayerManager() {}

    ~PlayerManager() {}

    void PlayerThenAccount();

    void Lock();
};

extern PlayerManager GPlayerManager;

