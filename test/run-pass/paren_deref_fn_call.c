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

/* A call through a parenthesized dereference of a function pointer,
 * (*(fn))(x), calls fn as (*fn)(x) does: a function pointer dereferences to
 * itself. (*(fn))() segfaulted: the dereference loaded the function's first
 * instruction and the call jumped to it. (*fn)() is the control. */
int add_one(int x)
{
	return x + 1;
}

int times_three(int x)
{
	return x * 3;
}

int (*global_fn)(int);

/* A local function pointer. */
int local_pointer(void)
{
	int (*fn)(int);
	fn = add_one;
	if((*fn)(6) != 7) return 11;
	if((*(fn))(6) != 7) return 12;
	if((*(*(fn)))(6) != 7) return 13;
	fn = times_three;
	if((*fn)(5) != 15) return 14;
	if((*(fn))(5) != 15) return 15;
	if((*(fn))(2) + (*fn)(3) != 15) return 16;
	if((*(fn))((*(fn))(2)) != 18) return 17;
	return 0;
}

/* A file-scope function pointer. */
int global_pointer(void)
{
	global_fn = add_one;
	if((*global_fn)(41) != 42) return 21;
	if((*(global_fn))(41) != 42) return 22;
	global_fn = times_three;
	if((*(global_fn))(4) != 12) return 23;
	return 0;
}

int main(void)
{
	int rc;
	rc = local_pointer();
	if(0 != rc) return rc;
	rc = global_pointer();
	if(0 != rc) return rc;
	return 0;
}
