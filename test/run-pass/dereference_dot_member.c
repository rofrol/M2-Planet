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

/* *s.p and *q->p dereference the pointer the member holds; ++q->x and
 * --q->x change the member q points at. */
enum E { E0 = 0, E1 = 1 };
typedef enum E E_t;
struct ce_t { enum E a; E_t b; int c; };
struct cep_t { enum E* a; E_t* b; int* c; };
struct n_t { int x; };
struct s_t { int* p; int** pp; int* a[2]; int x; struct n_t* n; };
struct s_t gs;
struct s_t* gq;

int id(int v)
{
	return v;
}

/* The shapes on a local struct. */
int dot_local(void)
{
	struct s_t s;
	int c;
	int d;
	int e;
	int* t;
	c = 5;
	d = 6;
	e = 7;
	t = &d;
	s.p = &c;
	s.pp = &t;
	s.a[0] = &d;
	s.a[1] = &e;
	s.x = 3;
	if(*s.p != 5) return 71;
	*s.p = 9;
	if(c != 9) return 72;
	*s.p += 2;
	if(c != 11) return 73;
	++*s.p;
	if(c != 12) return 74;
	/* `(*p)++` is not covered: it segfaults on upstream 12fc8a9702 too */
	if(**s.pp != 6) return 76;
	**s.pp = 8;
	if(d != 8) return 77;
	if(&*s.p != s.p) return 78;
	if(sizeof(*s.p) != sizeof(int)) return 79;
	if(id(*s.p) != 12) return 80;
	if(!(*s.p && *s.p == 12)) return 81;
	if(*s.a[1] != 7) return 82;
	*s.a[1] = 4;
	if(e != 4) return 83;
	if(*(s.p) != 12) return 84;
	return 0;
}

/* The same shapes on a global struct. */
int dot_global(void)
{
	int c;
	int d;
	int e;
	int* t;
	c = 5;
	d = 6;
	e = 7;
	t = &d;
	gs.p = &c;
	gs.pp = &t;
	gs.a[0] = &d;
	gs.a[1] = &e;
	gs.x = 3;
	if(*gs.p != 5) return 91;
	*gs.p = 9;
	if(c != 9) return 92;
	*gs.p += 2;
	if(c != 11) return 93;
	++*gs.p;
	if(c != 12) return 94;
	/* `(*p)++` is not covered: it segfaults on upstream 12fc8a9702 too */
	if(**gs.pp != 6) return 96;
	**gs.pp = 8;
	if(d != 8) return 97;
	if(&*gs.p != gs.p) return 98;
	if(sizeof(*gs.p) != sizeof(int)) return 99;
	if(id(*gs.p) != 12) return 100;
	if(!(*gs.p && *gs.p == 12)) return 101;
	if(*gs.a[1] != 7) return 102;
	*gs.a[1] = 4;
	if(e != 4) return 103;
	if(*(gs.p) != 12) return 104;
	return 0;
}

/* The shapes through -> of a parameter, and ++ and -- on its members. */
int arrow_param(struct s_t* q, int* c, int* d)
{
	if(*q->p != 5) return 111;
	*q->p = 9;
	if(*c != 9) return 112;
	*q->p += 2;
	if(*c != 11) return 113;
	++*q->p;
	if(*c != 12) return 114;
	/* `(*p)++` is not covered: it segfaults on upstream 12fc8a9702 too */
	if(**q->pp != 6) return 116;
	**q->pp = 8;
	if(*d != 8) return 117;
	if(&*q->p != q->p) return 118;
	if(id(*q->p) != 12) return 119;
	if(*q->a[1] != 7) return 120;
	if(q->n->x != 21) return 121;
	++q->x;
	if(q->x != 4) return 122;
	--q->x;
	if(q->x != 3) return 123;
	q->x++;
	if(q->x != 4) return 124;
	++q->n->x;
	if(q->n->x != 22) return 125;
	--q->n->x;
	if(q->n->x != 21) return 126;
	if(++q->x != 5) return 127;
	if(--q->x != 4) return 128;
	return 0;
}

/* ++ and -- through a global pointer. */
int arrow_global(void)
{
	struct n_t n;
	n.x = 31;
	gs.x = 13;
	gs.n = &n;
	gq = &gs;
	++gq->x;
	if(gs.x != 14) return 131;
	--gq->x;
	if(gs.x != 13) return 132;
	++gq->n->x;
	if(n.x != 32) return 133;
	return 0;
}

int main(void)
{
	struct ce_t ce;
	struct cep_t cep;
	struct cep_t* pp;
	enum E* q;
	int* r;
	struct s_t s;
	struct n_t n;
	int c;
	int d;
	int e;
	int* t;
	int rc;
	ce.a = 7;
	ce.b = 9;
	ce.c = 11;
	q = &ce.a;
	if(q != &ce.a) return 10;
	if(*q != 7) return 11;
	q = &ce.b;
	if(*q != 9) return 12;
	cep.a = q;
	if(cep.a != &ce.b) return 13;
	if(*cep.a != 9) return 14;
	cep.a = &ce.a;
	if(cep.a != &ce.a) return 20;
	q = cep.a;
	if(*q != 7) return 21;
	if(*cep.a != 7) return 22;
	cep.b = &ce.b;
	if(cep.b != &ce.b) return 30;
	if(*cep.b != 9) return 31;
	cep.c = &ce.c;
	if(cep.c != &ce.c) return 40;
	r = cep.c;
	if(*r != 11) return 41;
	if(*cep.c != 11) return 42;

	/* The same through ->. */
	pp = &cep;
	if(pp->a != &ce.a) return 50;
	if(*pp->a != 7) return 51;
	if(*pp->b != 9) return 52;
	if(*pp->c != 11) return 53;

	/* Stores through the dereferenced member. */
	*cep.c = 12;
	if(ce.c != 12) return 60;
	*pp->b = 13;
	if(ce.b != 13) return 61;
	*cep.c += 2;
	if(ce.c != 14) return 62;

	rc = dot_local();
	if(0 != rc) return rc;
	rc = dot_global();
	if(0 != rc) return rc;
	c = 5;
	d = 6;
	e = 7;
	t = &d;
	n.x = 21;
	s.p = &c;
	s.pp = &t;
	s.a[0] = &d;
	s.a[1] = &e;
	s.x = 3;
	s.n = &n;
	rc = arrow_param(&s, &c, &d);
	if(0 != rc) return rc;
	if(s.x != 4) return 129;
	if(n.x != 21) return 130;
	rc = arrow_global();
	if(0 != rc) return rc;
	return 0;
}
