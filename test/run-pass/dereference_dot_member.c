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

/* *s.p and *q->p dereference the pointer the member holds. */
enum E { E0 = 0, E1 = 1 };
typedef enum E E_t;
struct ce_t { enum E a; E_t b; int c; };
struct cep_t { enum E* a; E_t* b; int* c; };
int main(void)
{
	struct ce_t ce;
	struct cep_t cep;
	struct cep_t* pp;
	enum E* q;
	int* r;
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
	return 0;
}
