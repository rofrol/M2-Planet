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

/* A cast of a subscript, (long)cells[2], reads cells[2] at the element's
 * scale and width. When the cast type was set before the subscript ran, the
 * subscript scaled by sizeof(long): on a 64-bit target (long)cells[2] read
 * eight bytes at cells + 2*8. Every cell holds a different sentinel, so a read
 * at the cast type's scale, or one wider than the element, gives a value
 * other than the element's. */
static int global_cells[8];

/* A local int array: cells + 2*8 is cells[4] and cells[5]. */
int local_int(void)
{
	int cells[8];
	int i;
	i = 0;
	while(i < 8)
	{
		cells[i] = 100 + i;
		i = i + 1;
	}
	if((long)cells[2] != 102) return 11;
	if((unsigned long)cells[3] != 103) return 12;
	cells[2] = -5;
	if((long)cells[2] != -5) return 13;
	if((long)cells[1] != 101) return 14;
	if((long)cells[4] != 104) return 15;
	i = 2;
	if((long)cells[i] != -5) return 16;
	return 0;
}

/* A file-scope int array. */
int global_int(void)
{
	int i;
	i = 0;
	while(i < 8)
	{
		global_cells[i] = 200 + i;
		i = i + 1;
	}
	if((long)global_cells[2] != 202) return 21;
	if((long)global_cells[3] != 203) return 22;
	i = 2;
	if((long)global_cells[i] != 202) return 23;
	return 0;
}

/* A local char array: cells + 2*8 is cells[16], and a long-wide read at
 * cells + 2 takes in cells[3] to cells[9]. */
int local_char(void)
{
	char cells[24];
	int i;
	i = 0;
	while(i < 24)
	{
		cells[i] = 30 + i;
		i = i + 1;
	}
	if((long)cells[2] != 32) return 31;
	if((int)cells[3] != 33) return 32;
	if((long)cells[16] != 46) return 33;
	i = 2;
	if((long)cells[i] != 32) return 34;
	return 0;
}

int main(void)
{
	int rc;
	rc = local_int();
	if(0 != rc) return rc;
	rc = global_int();
	if(0 != rc) return rc;
	rc = local_char();
	if(0 != rc) return rc;
	return 0;
}
