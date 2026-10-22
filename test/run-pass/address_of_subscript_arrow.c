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

/* &a[i]->m on an array of struct pointers. */
struct S { int m; int k; };
struct S s0;
struct S *a[2];
int main(void)
{
	int *q;
	a[0] = &s0;
	q = &a[0]->k;
	*q = 7;
	if(s0.k != 7) return 1;
	if(s0.m != 0) return 2;
	return 0;
}
