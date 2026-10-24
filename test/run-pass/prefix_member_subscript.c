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

/* Prefix ++ and -- on a member or on an element reached through a member
 * change the cell they name and yield the value after: s.x, gs.x, q->p[i],
 * q->a[i], s.p[i], s.a[i] and *b[i]. The cells before and after the target
 * and the pointers stay as they were. They segfaulted, upstream 12fc8a9702
 * too: the member or the element was loaded before the operator and the
 * operator used its value as an address. */
struct s_t { int x; int y; int* p; int a[3]; int z; };
struct s_t gs;

/* s.x on a local struct. */
int dot_local(void)
{
	struct s_t s;
	int r;
	s.x = 5;
	s.y = 9;
	r = ++s.x;
	if(r != 6) return 11;
	if(s.x != 6) return 12;
	if(s.y != 9) return 13;
	r = --s.x;
	if(r != 5) return 14;
	if(s.x != 5) return 15;
	if(s.y != 9) return 16;
	if(++s.y != 10) return 17;
	if(--s.y != 9) return 18;
	if(s.x != 5) return 19;
	return 0;
}

/* gs.x on a global struct. */
int dot_global(void)
{
	int r;
	gs.x = 5;
	gs.y = 9;
	r = ++gs.x;
	if(r != 6) return 21;
	if(gs.x != 6) return 22;
	if(gs.y != 9) return 23;
	r = --gs.x;
	if(r != 5) return 24;
	if(gs.x != 5) return 25;
	if(gs.y != 9) return 26;
	return 0;
}

/* q->p[i] through a pointer member. */
int arrow_pointer_subscript(void)
{
	int c[3];
	struct s_t s;
	struct s_t* q;
	int r;
	c[0] = 1;
	c[1] = 5;
	c[2] = 9;
	s.p = c;
	q = &s;
	r = ++q->p[1];
	if(r != 6) return 31;
	if(c[1] != 6) return 32;
	if(c[0] != 1) return 33;
	if(c[2] != 9) return 34;
	r = --q->p[1];
	if(r != 5) return 35;
	if(c[1] != 5) return 36;
	if(c[0] != 1) return 37;
	if(c[2] != 9) return 38;
	if(s.p != c) return 39;
	if(q != &s) return 40;
	return 0;
}

/* q->a[i] on an array member. */
int arrow_array_subscript(void)
{
	struct s_t s;
	struct s_t* q;
	int r;
	s.y = 3;
	s.a[0] = 1;
	s.a[1] = 5;
	s.a[2] = 9;
	s.z = 13;
	q = &s;
	r = ++q->a[1];
	if(r != 6) return 41;
	if(s.a[1] != 6) return 42;
	if(s.a[0] != 1) return 43;
	if(s.a[2] != 9) return 44;
	r = --q->a[1];
	if(r != 5) return 45;
	if(s.a[1] != 5) return 46;
	if(s.a[0] != 1) return 47;
	if(s.a[2] != 9) return 48;
	if(++q->a[2] != 10) return 49;
	if(s.z != 13) return 50;
	if(s.y != 3) return 51;
	if(q != &s) return 52;
	return 0;
}

/* s.p[i] and s.a[i] through a dot member. */
int dot_subscript(void)
{
	int c[3];
	struct s_t s;
	int r;
	c[0] = 1;
	c[1] = 5;
	c[2] = 9;
	s.p = c;
	s.a[0] = 2;
	s.a[1] = 6;
	s.a[2] = 10;
	s.z = 13;
	r = ++s.p[1];
	if(r != 6) return 61;
	if(c[1] != 6) return 62;
	if(c[0] != 1) return 63;
	if(c[2] != 9) return 64;
	r = --s.p[1];
	if(r != 5) return 65;
	if(c[1] != 5) return 66;
	if(s.p != c) return 67;
	r = ++s.a[1];
	if(r != 7) return 68;
	if(s.a[1] != 7) return 69;
	if(s.a[0] != 2) return 70;
	if(s.a[2] != 10) return 71;
	r = --s.a[1];
	if(r != 6) return 72;
	if(s.a[1] != 6) return 73;
	if(s.z != 13) return 74;
	return 0;
}

/* *b[i] through an array of pointers. */
int deref_subscript(void)
{
	int a[3];
	int* b[2];
	int r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	b[0] = &a[1];
	b[1] = &a[2];
	r = ++*b[0];
	if(r != 6) return 81;
	if(a[1] != 6) return 82;
	if(a[0] != 1) return 83;
	if(a[2] != 9) return 84;
	r = --*b[0];
	if(r != 5) return 85;
	if(a[1] != 5) return 86;
	if(b[0] != &a[1]) return 87;
	if(b[1] != &a[2]) return 88;
	return 0;
}

int main(void)
{
	int rc;
	rc = dot_local();
	if(0 != rc) return rc;
	rc = dot_global();
	if(0 != rc) return rc;
	rc = arrow_pointer_subscript();
	if(0 != rc) return rc;
	rc = arrow_array_subscript();
	if(0 != rc) return rc;
	rc = dot_subscript();
	if(0 != rc) return rc;
	rc = deref_subscript();
	if(0 != rc) return rc;
	return 0;
}
