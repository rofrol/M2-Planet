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

/* Postfix ++ and -- on a parenthesized dereference change the cell it names
 * and yield the value before: (*p), (*s.p), (*q->p), (*(p + 1)) and
 * ((*p)). The cells before and after the target stay as they were. They
 * segfaulted, upstream 12fc8a9702 too: the last dereference loaded the cell
 * and the operator used its value as an address. */
struct s_t { int* p; };

/* (*p) on an int pointer. */
int plain(void)
{
	int a[3];
	int* p;
	int old;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	p = &a[1];
	old = (*p)++;
	if(old != 5) return 11;
	if(a[1] != 6) return 12;
	if(a[0] != 1) return 13;
	if(a[2] != 9) return 14;
	old = (*p)--;
	if(old != 6) return 15;
	if(a[1] != 5) return 16;
	if(a[0] != 1) return 17;
	if(a[2] != 9) return 18;
	if(p != &a[1]) return 19;
	return 0;
}

/* (*s.p) through a dot member. */
int dot_member(void)
{
	int a[3];
	struct s_t s;
	int old;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	s.p = &a[1];
	old = (*s.p)++;
	if(old != 5) return 21;
	if(a[1] != 6) return 22;
	if(a[0] != 1) return 23;
	if(a[2] != 9) return 24;
	old = (*s.p)--;
	if(old != 6) return 25;
	if(a[1] != 5) return 26;
	if(a[0] != 1) return 27;
	if(a[2] != 9) return 28;
	if(s.p != &a[1]) return 29;
	return 0;
}

/* (*q->p) through an arrow member. */
int arrow_member(void)
{
	int a[3];
	struct s_t s;
	struct s_t* q;
	int old;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	s.p = &a[1];
	q = &s;
	old = (*q->p)++;
	if(old != 5) return 31;
	if(a[1] != 6) return 32;
	if(a[0] != 1) return 33;
	if(a[2] != 9) return 34;
	old = (*q->p)--;
	if(old != 6) return 35;
	if(a[1] != 5) return 36;
	if(a[0] != 1) return 37;
	if(a[2] != 9) return 38;
	if(q->p != &a[1]) return 39;
	return 0;
}

/* (*(p + 1)) on a char pointer, so the offset needs no scaling. */
int paren_expr(void)
{
	char a[3];
	char* p;
	int old;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	p = a;
	old = (*(p + 1))++;
	if(old != 5) return 41;
	if(a[1] != 6) return 42;
	if(a[0] != 1) return 43;
	if(a[2] != 9) return 44;
	old = (*(p + 1))--;
	if(old != 6) return 45;
	if(a[1] != 5) return 46;
	if(a[0] != 1) return 47;
	if(a[2] != 9) return 48;
	if(p != a) return 49;
	return 0;
}

/* ((*p)): the inner parentheses close just before the outer ones. */
int nested(void)
{
	int a[3];
	int* p;
	int old;
	a[0] = 1;
	a[1] = 5;
	a[2] = 9;
	p = &a[1];
	old = ((*p))++;
	if(old != 5) return 51;
	if(a[1] != 6) return 52;
	if(a[0] != 1) return 53;
	if(a[2] != 9) return 54;
	old = ((*p))--;
	if(old != 6) return 55;
	if(a[1] != 5) return 56;
	if(a[0] != 1) return 57;
	if(a[2] != 9) return 58;
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
	rc = paren_expr();
	if(0 != rc) return rc;
	rc = nested();
	if(0 != rc) return rc;
	return 0;
}
