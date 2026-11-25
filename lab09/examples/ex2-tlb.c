TLB = translation lookaside buffer
no page faults

search TLB - 20 ns
search physical memory - 100 ns

TLB hit ratio = 80% = 0.8
TLB miss ratio = 20% = 0.2

EMAT (effective memory access)
EMAT = h(t + m) + (1 - h)(t + m + m)
EMAT = 0.8(20 + 100) + 0.2(20 + 100 + 100)
EMAT = 96 + 44 = 140 ns

