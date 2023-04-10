
// idea: since we dumped luaopen_base we can try to look fro pushclosure of xpcall
// by reversing I found
/*
	// 3 ptrobf in a row make them harder to be deobfuscated without a decompiler
	*(_DWORD *)(v351 + 0x10) = (char *)sub_20A4600 + v351 + 0x10;
	*(_DWORD *)(v351 + 0x14) = (unsigned int)sub_20A4860 ^ (v351 + 0x14);
	*(_DWORD *)(v351 + 0x18) = &aPcall[-v351 - 0x18];
*/


// search this inside luaopen_base to find pushkclosure
// 6A 01 6A 20 6A 48
// push    1
// push    20h; ' '
// push    48h; 'H'
// aka (preparation to newpage call for a closure object)
// newpage(72, 32, 1);