#include "pch.h"
#include "AccountManager.h"
#include "PlayerManager.h"

AccountManager GAccountManager;

void AccountManager::AccountThenPlayer()
{
    READ_LOCK;

    GPlayerManager.Lock();
}

void AccountManager::Lock()
{
    READ_LOCK;
}