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


/* `&array[i]` must give the element's address. The element access used to load
 * the element's value even under the address-of operator, so `q = &ints[1]`
 * was 0 whenever ints[1] was 0. Any nonzero exit is a failure. */
struct address_item
{
	int n;
	int m;
};

static int ints[4];
static struct address_item items[4];

int main(void)
{
	int *q;
	struct address_item *p;

	ints[1] = 11;
	q = &ints[1];
	if(q == 0) return 1;
	if(*q != 11) return 2;
	*q = 12;
	if(ints[1] != 12) return 3;

	items[2].n = 7;
	p = &items[2];
	if(p == 0) return 4;
	if(p->n != 7) return 5;
	p->n = 8;
	if(items[2].n != 8) return 6;

	/* An element that is zero still has a non-null address. */
	ints[3] = 0;
	q = &ints[3];
	if(q == 0) return 7;
	if(*q != 0) return 8;

	return 0;
}
