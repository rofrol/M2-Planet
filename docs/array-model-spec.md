# M2-Planet array/type model — design spec for the array-of-pointers fix

v2, 2026-10-21: reviewed as text before implementation (round
20261021-003518-28a6, all four models; v1's gaps are folded in below). It
pins WHERE arrayness lives and WHO consumes it, because four implementation
candidates invented four different flawed mechanisms (size heuristics,
sticky flags, diagnostic-string-triggered side tables) and all were
rejected in review.

## The data model today (facts, cc.h and the parse paths)

`struct type` (cc.h:102): `size`, `offset`, `is_signed`, `indirect`,
`members` (member list), `type` (the pointee of a pointer; for non-pointer
types `type` points to the type itself), `name`, `options` (TO_* flags).
Struct MEMBERS are `struct type` nodes in the `members` list
(build_member, cc_types.c:496): `i->type` = the declared member type,
`i->size` = the storage size, `i->offset` = the byte offset.

`struct token_list` (cc.h:130) is the SYMBOL record (locals, arguments,
globals): `type`, `depth`, `array_modifier` (element count; sym_declare
defaults it to 1 even for scalars — cc_core.c:92), `options` (TLO flags:
TLO_LOCAL_ARRAY for local arrays, TLO_ARGUMENT, TLO_GLOBAL, TLO_STATIC...).

## The gap and the defect

Arrayness is declarator information living in inconsistent places:
TLO_LOCAL_ARRAY (locals only), `array_modifier` (count with the ambiguous
default 1), and nowhere for members (candidates re-derived it from
`i->size != i->type->size`, which misreads a one-element pointer array).
The defect in one sentence: **the element type of an array is its DECLARED
element type (the declarator's base type), and the code takes `x->type`
(the pointee of a pointer) instead** — at build_member's size rule
(cc_types.c:551: `constant_expression() * member_type->type->size`) and at
postfix_expr_array's element decision (`if(type_is_pointer(array)) element
= array->type`).

## The mechanism (the fix)

1. Markers (flags, not count semantics — the count convention stays exactly
   as it is, default 1, so no scalar consumer changes):
   - symbols: a new TLO_ARRAY option bit, set by every declarator path that
     parses `[` (locals cc_core.c:2263+, statics 2803+, globals 3583+;
     TLO_LOCAL_ARRAY stays as it is). Parameters never get it: `f(T a[])`
     decays to `T *` at declaration, count semantics untouched.
   - members: one new int field on `struct type`, e.g. `is_array`, set ONLY
     on the per-member node `i` in build_member's array branch — never on
     `member_type`, which may be a shared primitive type node (setting it
     there would turn every `int` into an array).
2. `is_array(base)` is derived from those markers at each use site: a
   symbol with TLO_ARRAY, or a member node with `is_array`. It is read from
   the base expression's symbol/member node, which the expression path must
   keep on the C call stack across nested parses (the subscript index, a
   cast operand, a parenthesized subexpression) — never in file-scope
   mutable state, never carried across an expression boundary as a flag.
3. Consumption sites (the only places that read the predicate):
   a. postfix_expr_array: if the base is an array, the element type is the
      base's DECLARED element type (the symbol's `type`, the member node's
      `i->type`) and the load width for `a[i]` as an rvalue is that type's
      size; if the base is a scalar pointer, the element type is the
      pointee (`->type`), exactly as today. Scale = element size.
   b. the store-width and compound-assignment selection for `a[i] = v`
      (and `a[i]++`/`--`: load AND store width) uses the same element type.
   c. `sizeof`: an array name or member is `count * element size` (the
      existing count fields); sizeof suppresses decay.
   d. decay: **the value type of an array is a pointer to its declared
      element type** (`T *a[N]` decays to `T **`, `int m[N]` to `int *`).
      Pointer arithmetic (`words + i`), `&words[i]` (pointer to element),
      call arguments and member decay (`s.arr` as a value is its address)
      all follow from this one sentence.
   e. member layout (build_member): `i->size = N * member_type->size` — the
      DECLARED element type's size (today `member_type->type->size`, the
      pointee).
4. Scalar contracts (unchanged counts keep them working; state them so a
   reviewer can check): cc_core.c:1273-1275, 2300 and 3330 multiply by
   `array_modifier`, which stays 1 for scalars — they must not be touched
   except where they choose an element type (use the declared one). The
   local `array_modifier == 0` unsized-array error path stays as it is.
5. Initializer emission: pointer elements of an array initializer are
   emitted pointer-width (global_array_initializer_list and the static
   initializer paths read the same element type rule as 3a).
6. State discipline: no new file-scope mutable state; any per-parse state
   is saved/restored on the C call stack around nested parses. No size
   heuristics (`size != type->size`), no state keyed on diagnostic strings
   or token pointers, no positional or designated struct initializers.

## Bootstrap constraint (the previous release builds this source)

The chain compiles the fork's own C with the previous release's M2-Planet,
whose build_member rule mis-sizes member arrays of pointers. The fix's
source must therefore contain NO member array of pointers (and no new
array-of-pointer shape the old compiler miscompiles); the fix's own
binaries are built by the old rules and must still be correct.

## What must not change (pinned)

The GPL header, general_recursion's EOF guard, the two __va_arg
diagnostics, RISCV64's `rd_a0 rs1_a0 lwu` load, and the five behavior cases
in test/run-pass/ (four pins, one guard). Multidimensional arrays stay out
of the fork's subset, as today.

## Files the fix may touch

cc.h (the new member field and the TLO bit), cc_core.c (declarator paths,
expression paths, sizeof, initializers), cc_types.c (build_member).
Nothing else.
