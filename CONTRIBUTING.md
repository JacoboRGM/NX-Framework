# Contributing

Thanks for considering contributing to this driver. It targets flash/RAM-constrained embedded MCUs, so a few conventions matter more here than in a typical application-level codebase.

## Before You Start

- Read [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) first — most changes should fit cleanly into one of the five existing layers (Type System → Object Model → Transport → Device/Routing → Public API). If your change doesn't fit any layer, that's worth a discussion in an issue before opening a PR.
- Check open issues for known limitations already tracked (shared scratch buffer reentrancy, event-dispatch duplication, etc.) before reporting them again.

## Ground Rules for Core Changes (`lib/core/`)

1. **No dynamic memory.** Every structure in `lib/core` is statically allocated today. Don't introduce `malloc`/`free` — if you need variable-length storage, size it at compile time via a `#define` constant (following the pattern of `NX_MAX_VECTOR_LEN` / `NX_FIFO_DEPTH`).
2. **Validate pointers before dereferencing, always, at the top of the function.** Look at the existing guard-clause style (`if (x == NULL) return NX_ERROR_...;`) before any other logic — including logic that just *reads* a field to decide on a clamp or branch. A null check placed *after* a dereference doesn't protect anything.
3. **Never mix `NX_Status_t` and `NX_Type_t` (or any other enum) return values.** Each enum has its own `_MAX` sentinel and its own error semantics; don't repurpose a value from one enum as a "this failed" signal for a function that returns a different enum type.
4. **`snprintf` return values must be validated before use**, not passed straight through as a length. Check for both `< 0` (encoding error) and `>= sizeof(buffer)` (truncation) *before* narrowing to a smaller integer type — validating after an implicit `int → uint8_t` conversion silently loses negative values.
5. **New Nextion component types** go in `nx_component.c`: add a blueprint constant with its `NX_ATTR_BIT(...)` mask, register it in `NX_DB[]`, and add the corresponding `NX_TYPE_*` entry in `nx_types.h`. Don't special-case a new type inside `nx_api.c` setters unless the behavior genuinely can't be expressed as a supported-attribute check.
6. **Comment language:** keep source comments in English or plain ASCII Spanish — avoid non-ASCII accented characters in files that might be saved with a different encoding than the rest of the repo (this project has had `¿`/`í`-style characters get mangled into mojibake in ISO-8859-encoded files in the past).

## Code Style

- Match the existing formatting: 4-space indentation, Allman-ish brace style as used in each file, `NX_` prefix on every public symbol, `snake_case` for local variables, `PascalCase` for function names.
- Every public function needs a Doxygen-style comment block (`@brief`, `@param[in]`, `@return`) matching the pattern already used throughout `nx_api.h`.
- Keep the doc comment's parameter names in sync with the actual signature — several existing headers reference an old `transport` parameter name for functions that now take `screen`; don't repeat that drift in new code.

## Testing Changes

This is a hardware-dependent embedded project without a hosted unit test harness yet. At minimum, before opening a PR:

1. Build clean in MPLAB X with XC16 (no new warnings).
2. Exercise the changed function(s) against real Nextion hardware (or at minimum a UART logic capture) and confirm the exact byte sequence sent matches the Nextion Instruction Set documentation for that command.
3. If you touched attribute clamping ranges (`NX_SetDis`, `NX_SetTim`, etc.), double check the range against the Nextion Editor documentation for every component type affected — these ranges are type-dependent and easy to get subtly wrong.

## Pull Requests

- Keep PRs scoped to one layer or one bug — a PR that fixes a `nx_api.c` command mismatch shouldn't also refactor the FIFO.
- Reference the specific Nextion Instruction Set command(s) affected (with a link or page reference) when the change affects wire protocol behavior.
- Update `docs/API.md` in the same PR if you add, rename, or change the behavior of a public function.

## Reporting Bugs

Please include:
- The exact function called and arguments used.
- The `NX_Status_t` returned (if any).
- A UART capture of the actual bytes sent/received, if the issue is protocol-related.
- Nextion panel series/model (Basic/Enhanced/Intelligent) — some commands are tier-gated.
