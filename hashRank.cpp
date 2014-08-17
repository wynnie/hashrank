/*
hashRank.cpp : By wynnie, Aug-2008

hashRank is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation.

hashRank is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

The actual hash function and associated lookups were generated using Bob Jenkins hash generator.
http://burtleburtle.net/bob/hash/perfect.html

The bit count table was borrowed from from Keith Rule's HandEvaluator 
http://www.codeproject.com/KB/game/pokerhandevaldoc.aspx

*/
#include "hashRank.h"

//The hash function
inline ub4 phash(ub4 val)
{
  ub4 a, b, rsl;
  val += 0xcfa2b1c8;
  val ^= (val >> 16);
  val += (val << 8);
  val ^= (val >> 4);
  b = val & 0x1fff;
  a = (val + (val << 15)) >> 16;
  rsl = rankLookup[(a^scramble[tab[b]])];
  return rsl;
}

uint32 getRank (uint32 * cardU, uint64 handMask)
{
	
	uint32 cardSpades = (uint32)((handMask) & 0x00001fffUL); 
	uint32 cardHearts = (uint32)((handMask >> 13) & 0x00001fffUL);
	uint32 cardDice   = (uint32)((handMask >> 26) & 0x00001fffUL);
	uint32 cardClubs  = (uint32)((handMask >> 39) & 0x00001fffUL);
	
	if (nBitsTable[cardSpades] >= 5) //5 or more spades
	{
		return flushes[cardSpades];
	}
	else if (nBitsTable[cardHearts] >= 5)
	{
		return flushes[cardHearts];
	}
	else if (nBitsTable[cardDice] >= 5)
	{
		return flushes[cardDice];
	}
	else if (nBitsTable[cardClubs] >= 5)
	{
		return flushes[cardClubs];
	}
	else
	{
		return (uint32)phash(cardU[0] + cardU[1] + cardU[2] + cardU[3] + cardU[4] + cardU[5] + cardU[6]);	
	}
	/*
	uint32 flushRank;
	if ((flushRank = flushes[cardSpades]) != 5000)
	{
		return flushRank;
	}
	else if ((flushRank = flushes[cardHearts]) != 5000)
	{
		return flushRank;
	}
	else if ((flushRank = flushes[cardDice]) != 5000)
	{
		return flushRank;
	}
	else if ((flushRank = flushes[cardClubs]) != 5000)
	{
		return flushRank;
	}
	else
	{
		return (uint32)phash(cardU[0] + cardU[1] + cardU[2] + cardU[3] + cardU[4] + cardU[5] + cardU[6]);	
	}
	*/
	
}
