/* Copyright (C) 2026 Roman Frołow
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

/* A local array used as a value decays to its address. Its storage is the
 * variable's own slot, so the name must not be loaded: the old emitter read the
 * first register-sized chunk of the array's contents as a pointer, which turned
 * zeroed storage into NULL and made `p = mine` or `fill(mine)` fault on the
 * first write. Any nonzero exit is a failure. */

int decay_store(unsigned char* set, int index, int value)
{
	set[index] = value;
	return 0;
}

int decay_sum(const int* xs, int count)
{
	int k;
	int total = 0;

	for(k = 0; k < count; k++)
	{
		total = total + xs[k];
	}
	return total;
}

int main(void)
{
	unsigned char bytes[8];
	int numbers[4];
	unsigned char* p;
	int* q;
	int k;

	for(k = 0; k < 8; k++)
	{
		bytes[k] = 0;
	}
	for(k = 0; k < 4; k++)
	{
		numbers[k] = 0;
	}

	/* Decay into a local pointer, then write and read through it. */
	p = bytes;
	p[3] = 9;
	if(bytes[3] != 9) return 1;
	if(p != &bytes[0]) return 2;

	/* Decay as a parameter. */
	if(decay_store(bytes, 5, 6) != 0) return 3;
	if(bytes[5] != 6) return 4;
	if(decay_store(p, 6, 7) != 0) return 5;
	if(bytes[6] != 7) return 6;

	/* Elements that were not written stay zero: no write went out of bounds. */
	if(bytes[0] != 0) return 7;
	if(bytes[7] != 0) return 8;

	/* An int array decays the same way and pointer arithmetic scales by the
	 * element size. */
	numbers[0] = 1;
	numbers[1] = 2;
	numbers[2] = 3;
	numbers[3] = 4;
	q = numbers;
	if(q[2] != 3) return 9;
	if(q[3] != 4) return 10;
	if(decay_sum(numbers, 4) != 10) return 11;

	/* Dereferencing the decayed pointer still reads the first element. */
	if(*p != 0) return 12;

	/* The decayed pointer keeps working after other locals were used. */
	bytes[1] = 5;
	if(p[1] != 5) return 13;

	return 0;
}
