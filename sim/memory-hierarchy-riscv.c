/*
	Copyright (c) 2019, Vasileios Tsoutsouras (author)
 
	All rights reserved.

	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written 
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.
*/
#include "sf.h"

tuck void
riscVwritebyte(Engine *E, State *S, ulong vaddr, ulong xdata)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	uchar		data = xdata & 0xFF;
	
	paddr	= vaddr;
	inram	= 0;

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND))
	{
		inram = 1;
		latency = S->mem_w_latency;
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		devportwritebyte(E, S, vaddr, data);

		return;
	}
	
	if (inram)
	{
		S->MEM[paddr - S->MEMBASE] = data;
	}
	
	// if (!S->superH->cache_activated || !trans.cacheable)
	// {
	// 	S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
	// }

	return;
}

tuck void 	
riscVwriteword(Engine *E, State *S, ulong vaddr, ulong xdata)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	ushort		data = xdata & 0xFFFF;
	
	paddr	= vaddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0001)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND-1)) /* -1 for 2nd byte */
	{
		inram = 1;
		latency = S->mem_w_latency;
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		devportwriteword(E, S, vaddr, data);

		return;
	}

	
	
	if (inram)
	{
		write_2(S, data, &S->MEM[paddr - S->MEMBASE]);
		
		/*S->MEM[paddr - S->MEMBASE] = (uchar)((data>>8)&0xFF);
		S->MEM[paddr+1 - S->MEMBASE] = (uchar)data&0xFF;*/
	}
	
	// if (!S->superH->cache_activated || !trans.cacheable)
	// {
	// 	S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
	// }

	return;
}

tuck void 	
riscVwritelong(Engine *E, State *S, ulong vaddr, ulong data)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	
	paddr	= vaddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0011)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND - 3)) /* -3 for 4th byte */
	{
		inram = 1;
		latency = S->mem_w_latency;
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		devportwritelong(E, S, vaddr, data);

		return;
	}

	if (inram)
	{
		write_4(S, data, &(S->MEM[paddr - S->MEMBASE]));
		
		/*S->MEM[paddr - S->MEMBASE] = (uchar)((data>>24)&0xFF);
		S->MEM[paddr+1 - S->MEMBASE] = (uchar)((data>>16)&0xFF);
		S->MEM[paddr+2 - S->MEMBASE] = (uchar)((data>>8)&0xFF);
		S->MEM[paddr+3 - S->MEMBASE] = (uchar)data&0xFF;*/
	}
	
	// if (!S->superH->cache_activated || !trans.cacheable)
	// {
	// 	S->stallaction(E, S, paddr, MEM_WRITE_STALL, latency);
	// }

	return;
}


tuck uchar 	
riscVreadbyte(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	uchar		data = 0;
	
	paddr	= vaddr;
	inram	= 0;

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND))
	{
		inram = 1;
		latency = S->mem_r_latency;
		data = S->MEM[paddr - S->MEMBASE];
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return devportreadbyte(E, S, vaddr);
	}

	// if (!S->superH->cache_activated || !trans.cacheable)
	// {
	// 	S->stallaction(E, S, paddr, MEM_READ_STALL, latency);
	// }

	return data;
}

tuck ushort
riscVreadword(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	ushort		data = 0;
	
	paddr	= vaddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0001)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND - 1)) /* -1 for second byte */
	{
		inram = 1;
		latency = S->mem_r_latency;
		read_2(S, &S->MEM[paddr - S->MEMBASE], &data);
		//data = (ushort)(S->MEM[paddr - S->MEMBASE]<<8)|S->MEM[paddr+1 - S->MEMBASE];
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return devportreadword(E, S, vaddr);
	}

	// if (!S->superH->cache_activated || !trans.cacheable)
	// {
	// 	S->stallaction(E, S, paddr, MEM_READ_STALL, latency);
	// }

	return data;
}

tuck ulong 	
riscVreadlong(Engine *E, State *S, ulong vaddr)
{
	int		inram, latency = 0;
	TransAddr	trans;
	ulong		paddr;
	ulong		data = 0;

	paddr	= vaddr;
	inram	= 0;

	/*								*/
	/*	An address error if word data is written to an 		*/
	/*	address other than 2n or if longword data is written	*/
	/*	to an address other than 4n				*/
	/*								*/
	if (paddr & B0011)
	{
		/*	raise address error	*/
	}

	if ((paddr >= S->MEMBASE) && (paddr < S->MEMEND - 3)) /* -3 for 4th byte */
	{
		inram = 1;
		latency = S->mem_r_latency;
		read_4(S, &(S->MEM[paddr - S->MEMBASE]), &data);
		/*data = (ulong)(S->MEM[paddr - S->MEMBASE]<<24)|\
				(S->MEM[paddr+1 - S->MEMBASE]<<16)|\
				(S->MEM[paddr+2 - S->MEMBASE]<<8)|\
				S->MEM[paddr+3 - S->MEMBASE];*/
	}

	if (!inram)
	{
		/*							*/
		/*	   Pass this address off to the devices.	*/
		/*	We perform the necessary bitflip analysis in	*/
		/*	devport. If addr not found in devport, try	*/
		/*	arch-specific dev if not, fail with sfatal.	*/
		/*							*/
		return devportreadlong(E, S, vaddr);
	}

	// if (!S->superH->cache_activated || !trans.cacheable)
	// {
	// 	S->stallaction(E, S, paddr, MEM_READ_STALL, latency);
	// }

	return data;
}
