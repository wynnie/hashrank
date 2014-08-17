/*
main.cpp : By wynnie, Aug-2008

hashRank is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation.

hashRank is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>

#include "hashRank.h"
#include "indianaFast.h"

/*Card-number mapping where the sum is a unique 32 bit number for a valid poker hand*/
#define UN_2 1
#define UN_3 2
#define UN_4 9
#define UN_5 40
#define UN_6 185
#define UN_7 858
#define UN_8 3985
#define UN_9 18512
#define UN_T 86001
#define UN_J 399538
#define UN_Q 1856153
#define UN_K 8623224
#define UN_A 40061353


const char HandRanks[][16] = {"BAD!!","High Card","Pair","Two Pair","Three of a Kind","Straight","Flush","Full House","Four of a Kind","Straight Flush"};
__int64 timings, endtimings, freq;

int getUniq(int card)
{
	switch (card)
	{
	case 0 : return UN_2;
	case 1 : return UN_3;
	case 2 : return UN_4;
	case 3 : return UN_5;
	case 4 : return UN_6;
	case 5 : return UN_7;
	case 6 : return UN_8;
	case 7 : return UN_9;
	case 8 : return UN_T;
	case 9 : return UN_J;
	case 10 : return UN_Q;
	case 11 : return UN_K;
	case 12 : return UN_A;
	}
}
int performanceHashRank()
{
	
	uint32 virginDeck[52];
	uint64 virginMask[52];
	//uint32 rnk;
	uint32 sum=0;
	int c0, c1, c2, c3, c4, c5, c6;
	uint32 hand[7];
	uint64 m0, m1, m2, m3, m4, m5, m6;

	for (int c = 0; c < 52; c++) {
        virginDeck[c] = getUniq(c%13);
		virginMask[c] = (uint64)0x1<<c;
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&timings);
	for (c0 = 0; c0 < 46; c0++) {
		hand[0] = virginDeck[c0];
		m0 = virginMask[c0];
		for (c1 = c0+1; c1 < 47; c1++) {
			hand[1] = virginDeck[c1];
			m1 = virginMask[c1] | m0;
 			for (c2 = c1+1; c2 < 48; c2++) {
				hand[2] = virginDeck[c2];
				m2 = virginMask[c2] | m1;
				for (c3 = c2+1; c3 < 49; c3++) {
					hand[3] = virginDeck[c3];
					m3 = virginMask[c3] | m2;
 					for (c4 = c3+1; c4 < 50; c4++) {
						hand[4] = virginDeck[c4];
						m4 = virginMask[c4] | m3;
						for (c5 = c4+1; c5 < 51; c5++) {
							hand[5] = virginDeck[c5];
							m5 = virginMask[c5] | m4;
 							for (c6 = c5+1; c6 < 52; c6++) {
                                hand[6] = virginDeck[c6];
								m6 = virginMask[c6] | m5;
								//rnk = getRank(hand,m6);
								sum += getRank(hand,m6);
								/*if (rnk >= 5000) // Something wrong !!
									handTypeSum[0]++;
								else if (rnk >=4418) //High Card
									handTypeSum[1]++;
								else if (rnk >= 2948) // Pair
									handTypeSum[2]++;
								else if (rnk >= 2185) // Two Pair
									handTypeSum[3]++;
								else if (rnk >= 1610) // Trips
									handTypeSum[4]++;
								else if (rnk >= 1600) // Straight
									handTypeSum[5]++;
								else if (rnk >= 323) // Flush
									handTypeSum[6]++;
								else if (rnk >= 167) // Full House
									handTypeSum[7]++;
								else if (rnk >= 11) // Quads 
									handTypeSum[8]++;
								else				// Straight Flush
									handTypeSum[9]++; 
								*/
							}
						}
					}
				}
			}
		}
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&endtimings);
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	double timer = (endtimings - timings) / (double)freq;
	printf(" --- Hands per second: [b]%.0f[/b], hands 133784560, checksum %d, total time: %.0f",133784560/((double)timer),sum,(double)timer*1000);
	return 0;
}
void deal7Cards(uint32* out)
{
	int rcnt = 0;
	bool useddeck[52] = {false};
	while (rcnt != 7 )
	{
		int c = rand()%52;
		while (useddeck[c])
			c = rand()%52;
		out[rcnt++] = c;	
		useddeck[c] = true;
	}
}
int correctnessHashRank()
{
	uint32 virginDeck[52];
	uint64 virginMask[52];
	uint32 hand[7];
	uint32 randomNums0[7],randomNums1[7], indianaHand[7];
	uint64 mask0, mask1;

	uint32 hrank0, hrank1;
	long irank0, irank1;
	int maxIter = 1000000;
	

	for (int c = 0; c < 52; c++) {
        virginDeck[c] = getUniq(c%13);
		virginMask[c] = (uint64)0x1<<c;
	}
	initialize();
	
	for (int numIter=0;numIter<maxIter;numIter++)
	{
		deal7Cards(randomNums0);
		deal7Cards(randomNums1);
		
		//-----
		hand[0] = virginDeck[randomNums0[0]];
		hand[1] = virginDeck[randomNums0[1]];
		hand[2] = virginDeck[randomNums0[2]];
		hand[3] = virginDeck[randomNums0[3]];
		hand[4] = virginDeck[randomNums0[4]];
		hand[5] = virginDeck[randomNums0[5]];
		hand[6] = virginDeck[randomNums0[6]];

		mask0 = virginMask[randomNums0[0]]|virginMask[randomNums0[1]]|virginMask[randomNums0[2]]|virginMask[randomNums0[3]]|virginMask[randomNums0[4]]|virginMask[randomNums0[5]]|virginMask[randomNums0[6]];
		hrank0 = getRank(hand,mask0);
		for(int i=0;i<7;i++)
		{
			indianaHand[i] = (randomNums0[i]%13)*4 + (int)randomNums0[i]/13;
		}
		irank0 = defineHand((int*)indianaHand,7);
		//-----
		hand[0] = virginDeck[randomNums1[0]];
		hand[1] = virginDeck[randomNums1[1]];
		hand[2] = virginDeck[randomNums1[2]];
		hand[3] = virginDeck[randomNums1[3]];
		hand[4] = virginDeck[randomNums1[4]];
		hand[5] = virginDeck[randomNums1[5]];
		hand[6] = virginDeck[randomNums1[6]];
		mask1 = virginMask[randomNums1[0]]|virginMask[randomNums1[1]]|virginMask[randomNums1[2]]|virginMask[randomNums1[3]]|virginMask[randomNums1[4]]|virginMask[randomNums1[5]]|virginMask[randomNums1[6]];
		hrank1 = getRank(hand,mask1);
		for(int i=0;i<7;i++)
		{
			indianaHand[i] = (randomNums1[i]%13)*4 + (int)randomNums1[i]/13;
		}
		irank1= defineHand((int*)indianaHand,7);
		//-----
		if ((uint64)irank0 > (uint64)irank1) {
			if (hrank0 >= hrank1)
				printf("ERROR in correctness > check for %I64x %I64x %d %d!!!\n",mask0,mask1,hrank0,hrank1);
		} else if ((uint64)irank0 < (uint64)irank1) {
			if (hrank0 <= hrank1)
				printf("ERROR in correctness < check for %I64x %I64x %d %d!!!\n",mask0,mask1,hrank0,hrank1);
		} else {
			if (hrank0 != hrank1)
				printf("ERROR in correctness = check for %I64x %I64x %d %d %ld %ld!!!\n",mask0,mask1,hrank0,hrank1, irank0, irank1);
		}
		printf("\r%d%%..",(((numIter+1)*100)/maxIter));
	}
	printf("Done\n");
	return 0;
}
int main()
{
	performanceHashRank();
	//correctnessHashRank();
	int d;
	scanf("%d",&d);
	return 0;
}
