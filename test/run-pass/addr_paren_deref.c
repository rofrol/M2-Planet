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

/* In C `&*E` is E: the address-of a parenthesized dereference is the address
 * the dereference names. It gave the pointee's value instead, so
 * `&*(p + 1) != p + 1` and a store through the result wrote elsewhere. */

/* &*(p + 1) on a char pointer, so an offset is a byte offset and a store
 * wider than the cell would show in its neighbours. */
int char_pointer(void)
{
	char a[3];
	char* p;
	char* r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	p = a;
	r = &*(p + 1);
	if(r != p + 1) return 11;
	if(&*(p + 2) != a + 2) return 12;
	*r = 7;
	if(a[1] != 7) return 13;
	if(a[0] != 1) return 14;
	if(a[2] != 9) return 15;
	if(p != a) return 16;
	return 0;
}

/* &*(q) on an int pointer. */
int int_pointer(void)
{
	int a[3];
	int* q;
	int* r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	q = &a[1];
	r = &*(q);
	if(r != q) return 21;
	if(&*(q) != &a[1]) return 22;
	*r = 7;
	if(a[1] != 7) return 23;
	if(a[0] != 1) return 24;
	if(a[2] != 9) return 25;
	if(q != &a[1]) return 26;
	return 0;
}

int main(void)
{
	int rc;
	rc = char_pointer();
	if(0 != rc) return rc;
	rc = int_pointer();
	if(0 != rc) return rc;
	return 0;
}
