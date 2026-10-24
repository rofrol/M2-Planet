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

/* Prefix ++ and -- on a parenthesized dereference change the cell it names
 * and yield the value after: (*p), ((*p)), (*s.p), (*q->p), and on a char
 * pointer *(p + 1) and (*(p + 1)). ++a[i] on a char array yields the
 * element alone. The cells before and after the target and the pointers stay
 * as they were. The parenthesized shapes segfaulted: the last dereference saw
 * the `(` before it, not the operator, so it loaded the cell and the operator
 * used its value as an address.
 *
 * The same holds for a member or an element in the parentheses, (s.x),
 * (q->x), (a[i]) and (c[i]) on a char array, and for a chain that goes on
 * after the `)`: (*p).x, (s).x, (*pp)[i], (q)[i], (*pc)[i] on char cells and
 * (*qq)->x. These segfaulted, (*p).x failed to compile, and (*pp)[0] and
 * (q)[0] only worked because an int is as wide as a pointer: the operand
 * before the subscript kept its address and the subscript loaded it. */
struct s_t { int* p; };
struct m_t { int x; int y; };

/* (*p) and ((*p)) on an int pointer. */
int plain(void)
{
	int a[3];
	int* p;
	int r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	p = &a[1];
	r = ++(*p);
	if(r != 6) return 11;
	if(a[1] != 6) return 12;
	if(a[0] != 1) return 13;
	if(a[2] != 9) return 14;
	r = --(*p);
	if(r != 5) return 15;
	if(a[1] != 5) return 16;
	if(a[0] != 1) return 17;
	if(a[2] != 9) return 18;
	r = ++((*p));
	if(r != 6) return 19;
	if(a[1] != 6) return 20;
	if(a[2] != 9) return 21;
	if(p != &a[1]) return 22;
	return 0;
}

/* (*s.p) through a dot member. */
int dot_member(void)
{
	int a[3];
	struct s_t s;
	int r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	s.p = &a[1];
	r = ++(*s.p);
	if(r != 6) return 31;
	if(a[1] != 6) return 32;
	if(a[0] != 1) return 33;
	if(a[2] != 9) return 34;
	r = --(*s.p);
	if(r != 5) return 35;
	if(a[1] != 5) return 36;
	if(s.p != &a[1]) return 37;
	return 0;
}

/* (*q->p) through an arrow member. */
int arrow_member(void)
{
	int a[3];
	struct s_t s;
	struct s_t* q;
	int r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	s.p = &a[1];
	q = &s;
	r = ++(*q->p);
	if(r != 6) return 41;
	if(a[1] != 6) return 42;
	if(a[0] != 1) return 43;
	if(a[2] != 9) return 44;
	r = --(*q->p);
	if(r != 5) return 45;
	if(a[1] != 5) return 46;
	if(q->p != &a[1]) return 47;
	if(q != &s) return 48;
	return 0;
}

/* *(p + 1) and (*(p + 1)) on a char pointer, so the offset needs no
 * scaling and a load wider than the cell would show its neighbours in the
 * value yielded. */
int char_expr(void)
{
	char a[4];
	char* p;
	int r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	a[3] = 7;
	p = a;
	r = ++*(p + 1);
	if(r != 6) return 51;
	if(a[1] != 6) return 52;
	if(a[0] != 1) return 53;
	if(a[2] != 9) return 54;
	r = --*(p + 1);
	if(r != 5) return 55;
	if(a[1] != 5) return 56;
	r = ++(*(p + 2));
	if(r != 10) return 57;
	if(a[2] != 10) return 58;
	if(a[1] != 5) return 59;
	if(a[3] != 7) return 60;
	r = --(*(p + 2));
	if(r != 9) return 61;
	if(a[2] != 9) return 62;
	if(p != a) return 63;
	return 0;
}

/* a[i] on a char array. */
int char_subscript(void)
{
	char a[4];
	int r;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	a[3] = 7;
	r = ++a[2];
	if(r != 10) return 71;
	if(a[2] != 10) return 72;
	if(a[1] != 5) return 73;
	if(a[3] != 7) return 74;
	r = --a[2];
	if(r != 9) return 75;
	if(a[2] != 9) return 76;
	if(++a[1] != 6) return 77;
	if(a[0] != 1) return 78;
	return 0;
}

/* (s.x), (q->x), (a[i]) and (c[i]): a member or an element in the
 * parentheses. */
int paren_member(void)
{
	struct m_t s;
	struct m_t* q;
	int a[3];
	char c[4];
	int r;
	s.x = 1;
	s.y = 9;
	q = &s;
	a[0] = 4;
	a[1] = 6;
	a[2] = 8;
	c[0] = 1;
	c[1] = 5;
	c[2] = 9;
	c[3] = 7;
	r = ++(s.x);
	if(r != 2) return 81;
	if(s.x != 2) return 82;
	if(s.y != 9) return 83;
	r = --(s.y);
	if(r != 8) return 84;
	if(s.y != 8) return 85;
	if(s.x != 2) return 86;
	r = ++(q->x);
	if(r != 3) return 87;
	if(s.x != 3) return 88;
	if(s.y != 8) return 89;
	if(q != &s) return 90;
	r = ++(a[1]);
	if(r != 7) return 91;
	if(a[1] != 7) return 92;
	if(a[0] != 4) return 93;
	if(a[2] != 8) return 94;
	r = --(a[1]);
	if(r != 6) return 95;
	if(a[1] != 6) return 96;
	r = ++(c[2]);
	if(r != 10) return 97;
	if(c[2] != 10) return 98;
	if(c[1] != 5) return 99;
	if(c[3] != 7) return 100;
	return 0;
}

/* (*p).x and (s).x: a member access after the parentheses. */
int paren_dot(void)
{
	struct m_t s;
	struct m_t* p;
	int r;
	s.x = 4;
	s.y = 9;
	p = &s;
	r = ++(*p).x;
	if(r != 5) return 111;
	if(s.x != 5) return 112;
	if(s.y != 9) return 113;
	r = --(*p).y;
	if(r != 8) return 114;
	if(s.y != 8) return 115;
	if(s.x != 5) return 116;
	if((*p).y != 8) return 117;
	r = ++(s).x;
	if(r != 6) return 118;
	if(s.x != 6) return 119;
	if(s.y != 8) return 120;
	if(p != &s) return 121;
	return 0;
}

/* (*pp)[i], (q)[i], (*pc)[i] and (*qq)->x: a subscript or an arrow after the
 * parentheses. */
int paren_chain(void)
{
	int a[3];
	int* p;
	int** pp;
	int* q;
	char c[4];
	char* pc0;
	char** pc;
	struct m_t s;
	struct m_t* sp;
	struct m_t** qq;
	int r;
	a[0] = 7;
	a[1] = 3;
	a[2] = 5;
	p = a;
	pp = &p;
	q = a;
	c[0] = 1;
	c[1] = 5;
	c[2] = 9;
	c[3] = 7;
	pc0 = c;
	pc = &pc0;
	s.x = 2;
	s.y = 9;
	sp = &s;
	qq = &sp;
	r = ++(*pp)[0];
	if(r != 8) return 131;
	if(a[0] != 8) return 132;
	r = ++(*pp)[1];
	if(r != 4) return 133;
	if(a[1] != 4) return 134;
	if(a[0] != 8) return 135;
	if(a[2] != 5) return 136;
	if(p != a) return 137;
	if(pp != &p) return 138;
	r = --(q)[2];
	if(r != 4) return 139;
	if(a[2] != 4) return 140;
	if(a[1] != 4) return 141;
	if(q != a) return 142;
	r = ++(*pc)[2];
	if(r != 10) return 143;
	if(c[2] != 10) return 144;
	if(c[1] != 5) return 145;
	if(c[3] != 7) return 146;
	if(pc0 != c) return 147;
	r = ++(*qq)->x;
	if(r != 3) return 148;
	if(s.x != 3) return 149;
	if(s.y != 9) return 150;
	if(sp != &s) return 151;
	return 0;
}

int main(void)
{
	int rc;
	rc = plain();
	if(0 != rc) return rc;
	rc = dot_member();
	if(0 != rc) return rc;
	rc = arrow_member();
	if(0 != rc) return rc;
	rc = char_expr();
	if(0 != rc) return rc;
	rc = char_subscript();
	if(0 != rc) return rc;
	rc = paren_member();
	if(0 != rc) return rc;
	rc = paren_dot();
	if(0 != rc) return rc;
	rc = paren_chain();
	if(0 != rc) return rc;
	return 0;
}
