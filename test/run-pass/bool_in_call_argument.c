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

/* A short-circuit operator inside a function call's argument list. On aarch64
 * the branch emitters load the label's address into REGISTER_TEMP (X16), which
 * at that moment holds the call's new base pointer: the call then restored SP
 * from a code address and every later stack access faulted. Any nonzero exit
 * is a failure. */

int bool_arg_ident(int value)
{
	return value;
}

int bool_arg_sum(int a, int b, int c)
{
	return a + b + c;
}

int bool_arg_bump(int* n)
{
	*n = *n + 1;
	return 7;
}

int main(void)
{
	int one = 1;
	int zero = 0;
	int n = 0;
	int v;

	/* The first argument, which decides the base pointer's fate. */
	v = bool_arg_ident(one && one);
	if(v != 1) return 1;
	v = bool_arg_ident(zero && one);
	if(v != 0) return 2;
	v = bool_arg_ident(one || one);
	if(v != 1) return 3;
	v = bool_arg_ident(zero || zero);
	if(v != 0) return 4;

	/* Arguments after the one with the operator, and several operators. */
	v = bool_arg_sum(one && one, zero || one, one && zero);
	if(v != 2) return 5;
	v = bool_arg_sum(one, zero, zero || one);
	if(v != 2) return 6;

	/* Nested operators, with a call on the right-hand side. */
	v = bool_arg_ident(one && (zero || bool_arg_bump(&n)));
	if(v != 1) return 7;
	if(n != 1) return 8;
	v = bool_arg_ident(zero || (one && bool_arg_bump(&n)));
	if(v != 1) return 9;
	if(n != 2) return 10;

	/* The operators still short-circuit inside an argument. */
	v = bool_arg_ident(zero && bool_arg_bump(&n));
	if(v != 0) return 11;
	if(n != 2) return 12;
	v = bool_arg_ident(one || bool_arg_bump(&n));
	if(v != 1) return 13;
	if(n != 2) return 14;

	/* A condition in a parameter still works after the call returns. */
	v = bool_arg_ident(one && one) + bool_arg_ident(zero || one);
	if(v != 2) return 15;

	return 0;
}
