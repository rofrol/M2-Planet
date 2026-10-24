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

/* *s.fp((1)), *s.fp(i) and *s.fp((i)) dereference the pointer the call
 * through the member returns. The arguments' own expressions dropped the
 * pending `*`, so the case read the pointer instead of the cell it names.
 *
 * &(*(p)).b, &(*p).b and &(s).b take the member's address, as &p->b does.
 * The `&` applies to the chain after the `)`, but the operand's own primary
 * expression cleared it: the member was loaded by value, and before that
 * &(*(p)).b failed to compile because the dereference loaded the struct. */
struct f_t { int* (*fp)(int); };
struct m_t { int a; int b; };

int g[3];

int* get(int i)
{
	return &g[i];
}

/* *s.fp((1)), *s.fp(i) and *s.fp((i)). */
int deref_member_call(void)
{
	struct f_t s;
	int i;
	int v;
	g[0] = 4;
	g[1] = 5;
	g[2] = 6;
	s.fp = get;
	i = 1;
	v = *s.fp((1));
	if(v != 5) return 1;
	v = *s.fp(i);
	if(v != 5) return 2;
	v = *s.fp((i));
	if(v != 5) return 3;
	i = 2;
	v = *s.fp(i);
	if(v != 6) return 4;
	return 0;
}

/* &(*(p)).b, &(*p).b and &(s).b. */
int paren_member_address(void)
{
	struct m_t s;
	struct m_t* p;
	int* q;
	s.a = 1;
	s.b = 2;
	p = &s;
	q = &(*(p)).b;
	if(q != &s.b) return 11;
	*q = 5;
	if(s.b != 5) return 12;
	if(s.a != 1) return 13;
	q = &(*p).b;
	if(q != &s.b) return 14;
	q = &(*p).a;
	if(q != &s.a) return 15;
	q = &(s).b;
	if(q != &s.b) return 16;
	if(p != &s) return 17;
	return 0;
}

int main(void)
{
	int rc;
	rc = deref_member_call();
	if(0 != rc) return rc;
	rc = paren_member_address();
	if(0 != rc) return rc;
	return 0;
}
