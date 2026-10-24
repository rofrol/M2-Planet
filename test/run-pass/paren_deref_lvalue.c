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

/* A parenthesized dereference as the target of =, += and -=, and under a
 * subscript as the operand of ++ and the target of =, stores to the cell it
 * names: (*p), (*s.p), (*q->p) and (*b[i]). The cells before and after the
 * target and the pointer stay as they were. They segfaulted: the last
 * dereference saw the `)`, not the operator, so it loaded the cell and the
 * operator used its value as an address. */
struct s_t { int* p; };

/* (*p) on an int pointer. */
int int_pointer(void)
{
	int a[3];
	int* p;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	p = &a[1];
	(*p) = 7;
	if(a[1] != 7) return 11;
	if(a[0] != 1) return 12;
	if(a[2] != 9) return 13;
	(*p) += 2;
	if(a[1] != 9) return 14;
	if(a[0] != 1) return 15;
	if(a[2] != 9) return 16;
	(*p) -= 4;
	if(a[1] != 5) return 17;
	if(a[0] != 1) return 18;
	if(a[2] != 9) return 19;
	if(p != &a[1]) return 20;
	return 0;
}

/* (*p) on a char pointer, so a store wider than the cell would show in its
 * neighbours. */
int char_pointer(void)
{
	char a[3];
	char* p;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	p = a + 1;
	(*p) = 7;
	if(a[1] != 7) return 21;
	if(a[0] != 1) return 22;
	if(a[2] != 9) return 23;
	(*p) += 2;
	if(a[1] != 9) return 24;
	if(a[0] != 1) return 25;
	if(a[2] != 9) return 26;
	(*p) -= 4;
	if(a[1] != 5) return 27;
	if(a[0] != 1) return 28;
	if(a[2] != 9) return 29;
	if(p != a + 1) return 30;
	return 0;
}

/* (*s.p) through a dot member. */
int dot_member(void)
{
	int a[3];
	struct s_t s;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	s.p = &a[1];
	(*s.p) = 7;
	if(a[1] != 7) return 31;
	if(a[0] != 1) return 32;
	if(a[2] != 9) return 33;
	if(s.p != &a[1]) return 34;
	return 0;
}

/* (*q->p) through an arrow member. */
int arrow_member(void)
{
	int a[3];
	struct s_t s;
	struct s_t* q;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	s.p = &a[1];
	q = &s;
	(*q->p) = 7;
	if(a[1] != 7) return 41;
	if(a[0] != 1) return 42;
	if(a[2] != 9) return 43;
	if(q->p != &a[1]) return 44;
	if(q != &s) return 45;
	return 0;
}

/* (*b[0])++ and (*b[1]) = v through an array of pointers. */
int subscript(void)
{
	int a[4];
	int* b[2];
	int old;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	a[3] = 13;
	b[0] = &a[1];
	b[1] = &a[2];
	old = (*b[0])++;
	if(old != 5) return 51;
	if(a[1] != 6) return 52;
	if(a[0] != 1) return 53;
	if(a[2] != 9) return 54;
	(*b[1]) = 7;
	if(a[2] != 7) return 55;
	if(a[1] != 6) return 56;
	if(a[3] != 13) return 57;
	if(b[0] != &a[1]) return 58;
	if(b[1] != &a[2]) return 59;
	return 0;
}

int main(void)
{
	int rc;
	rc = int_pointer();
	if(0 != rc) return rc;
	rc = char_pointer();
	if(0 != rc) return rc;
	rc = dot_member();
	if(0 != rc) return rc;
	rc = arrow_member();
	if(0 != rc) return rc;
	rc = subscript();
	if(0 != rc) return rc;
	return 0;
}
