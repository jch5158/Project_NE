#include "pch.h"
#include "PlayerManager.h"
#include "AccountManager.h"

PlayerManager GPlayerManager;

void PlayerManager::PlayerThenAccount()
{
    READ_LOCK;

    GAccountManager.Lock();
}

void PlayerManager::Lock()
{
    READ_LOCK;
}