# vaslang

Flag: cctf{Y0U_could_HavE_So1V3D_tH15_tHe_FuN_w4y}

Finding: the interpreter is fine. The program file just has its functions in the
wrong order. The interpreter resolves every call at definition time, so each
function must be defined before it is used. I topologically sorted the definitions.

Locations:
- No ssh container for this one: solved locally from the given sources (vaslang.c + inp.vas), on my own machine.
- Interpreter: vaslang.c (a small stack machine; at the end it prints the value
  stack as text until a 0 byte). Not changed.
- Where it fails: xdef() in vaslang.c looks up each call target only among functions
  defined so far (for i, funcs+i < funcp). If not found it does
  errorf("func %s undefined", term) and the definition dies.
- Broken input: inp.vas = 1914 "name: body" definitions + main!, in a bad order.
  Running it as-is gives many "func X undefined", ending with "func main undefined".
- Fix: reorder.py builds the call graph, checks it is a DAG (no cycles, all names
  exist, no duplicates) and sorts callees before callers -> inp_fixed.vas.

Commands:
  python3 reorder.py inp.vas inp_fixed.vas
  gcc -D_GNU_SOURCE -O2 -o vas vaslang.c
  ./vas < inp_fixed.vas

Output (hello demo, then main!, no ERROR lines):
  hello
  cctf{Y0U_could_HavE_So1V3D_tH15_tHe_FuN_w4y}
