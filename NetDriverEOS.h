#pragma once
#include "pch.h"

namespace NetDriverEOS {

	DefineCall(bool, IpNetDriverInitBase, ImageBase + 0x52C1340, __int64, bool, __int64, const FURL&, bool, FString&);
	bool InitBase(__int64 a1, bool bInitAsClient, __int64 InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error);


	DefineCall(bool, IpNetDriverInitListen, ImageBase + 0x52C2B30, __int64, __int64, FURL&, bool, FString&);
	bool InitListen(__int64 a1, __int64 InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error);


	void Hook();
};