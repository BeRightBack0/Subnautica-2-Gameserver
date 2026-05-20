#pragma once
#include "pch.h"
#include "Finders.h"
namespace NetDriverEOS {

	DefineCall(bool, IpNetDriverInitBase, Finders::FindNetDriverInitBase(), __int64, bool, __int64, const FURL&, bool, FString&);
	bool InitBase(__int64 a1, bool bInitAsClient, __int64 InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error);


	DefineCall(bool, IpNetDriverInitListen, Finders::FindNetDriverInitListen(), __int64, __int64, FURL&, bool, FString&);
	bool InitListen(__int64 a1, __int64 InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error);


	void Hook();
};