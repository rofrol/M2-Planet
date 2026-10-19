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


/* `&&` and `||` are not the bitwise `&` and `|`: the right operand is
 * evaluated only when the left one does not decide the result, and the result
 * is 0 or 1 whatever the operands are. Any nonzero exit is a failure. */
struct logical_test
{
	int x;
};

int logical_bump(int* n)
{
	*n = *n + 1;
	return 2;
}

int main(void)
{
	struct logical_test* p = 0;
	int n = 0;
	int v;

	/* A bitwise emitter gave 1 & 2 = 0 and 0 | 2 = 2. */
	v = 1 && 2;
	if(v != 1) return 1;
	v = 0 || 2;
	if(v != 1) return 2;
	v = 0 && 2;
	if(v != 0) return 3;
	v = 0 || 0;
	if(v != 0) return 4;

	/* The right operand must not run when the left decides. */
	v = 0 && logical_bump(&n);
	if(v != 0) return 5;
	if(n != 0) return 6;
	v = 1 || logical_bump(&n);
	if(v != 1) return 7;
	if(n != 0) return 8;

	/* It must run when the left does not decide. */
	v = 1 && logical_bump(&n);
	if(v != 1) return 9;
	if(n != 1) return 10;
	v = 0 || logical_bump(&n);
	if(v != 1) return 11;
	if(n != 2) return 12;

	/* The NULL guard the old emitter turned into a dereference. */
	if(p == 0 || p->x == 1)
	{
		v = 1;
	}
	else
	{
		v = 0;
	}
	if(v != 1) return 13;

	return 0;
}
