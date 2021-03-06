#include "Spinlock.h"

namespace core
{
Spinlock::Spinlock() : lock_(0), reader_(0)
{}

Spinlock::~Spinlock()
{
}

void Spinlock::Lock()
{
	for (;;)
	{
		if (lock_.exchange(1) == 0)
		{
			while (reader_.load() != 0) {}
			return;
		}
		Sleep(1);
	}
}

void Spinlock::Unlock()
{
	lock_.exchange(0);

}

void Spinlock::ReadLock()
{
	for (;;)
	{
		if (!lock_) {
			++reader_;
			return;
		}
		Sleep(1);
	}
}

void Spinlock::ReadUnlock()
{
	--reader_;
}

ExclusiveLockHolder::ExclusiveLockHolder(Spinlock & spinlock) : spinlock_(spinlock)
{
	spinlock_.Lock();
}

ExclusiveLockHolder::~ExclusiveLockHolder()
{
	spinlock_.Unlock();
}

SharedLockHolder::SharedLockHolder(Spinlock & spinlock) : spinlock_(spinlock)
{
	spinlock_.ReadLock();
}

SharedLockHolder::~SharedLockHolder()
{
	spinlock_.ReadUnlock();
}

}